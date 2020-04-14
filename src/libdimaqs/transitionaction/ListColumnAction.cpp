/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "ListColumnAction.h"
#include "../DatabasePetriNetQuery.h"

#include "sql_lex.h"
#include "regex"

namespace transitionaction{

ListColumnAction::ListColumnAction()
{

}

bool ListColumnAction::isMatching(const petrinet::AbstractPetriNetQuery& query)
{
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);
    if( q.getActionType() == SQLCOM_SHOW_FIELDS)
        return true;

    if( q.getActionType() == SQLCOM_SELECT ){

        if(     !q.getLex()->select_lex
           ||   !q.getLex()->select_lex->get_table_list() )
        {
            return false;
        }

        {
            std::regex regx_db("INFORMATION_SCHEMA", std::regex_constants::icase | std::regex_constants::ECMAScript);
            if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_db_name(),regx_db))
            {
                // it's a table of the INFORMATION_SCHEMA
                std::regex regx_columns("(COLUMNS|KEY_COLUMN_USAGE)", std::regex_constants::icase | std::regex_constants::ECMAScript);
                if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_table_name(),regx_columns))
                    return true;
            }
        }

    }

    return false;
}

data::ListColumnData* ListColumnAction::getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query)
{
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);

    std::string schema, table;
    std::cmatch m;

    {
        std::regex regx_db("INFORMATION_SCHEMA", std::regex_constants::icase | std::regex_constants::ECMAScript);
        if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_db_name(),regx_db))
        {
            // it's a table of the INFORMATION_SCHEMA
            schema += "INFORMATION_SCHEMA";
            std::regex regx_columns("(COLUMNS|KEY_COLUMN_USAGE)", std::regex_constants::icase | std::regex_constants::ECMAScript);
            if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_table_name(),m,regx_columns))
                table += m[0];
        }
    }

    data::ListColumnData* data = new data::ListColumnData();
    data->setDatabase(schema);
    data->setTable(table);
    data->setQueryType(q.getQueryType());

    return data;
}

}
