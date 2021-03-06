/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "ListTableAction.h"
#include "../DatabasePetriNetQuery.h"

#include "sql_lex.h"
#include "regex"
#include "LiteralCollector.h"

namespace transitionaction{

ListTableAction::ListTableAction()
{

}

bool ListTableAction::isMatching(const petrinet::AbstractPetriNetQuery& query)
{  
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);
    if(     q.getActionType() == SQLCOM_SHOW_TABLES
       ||   q.getActionType() == SQLCOM_SHOW_OPEN_TABLES
       ||   q.getActionType() == SQLCOM_SHOW_TABLE_STATUS)
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

                std::regex regx_columns("(COLUMNS|FILES|KEY_COLUMN_USAGE|PARTITIONS|TABLES)", std::regex_constants::icase | std::regex_constants::ECMAScript);
                if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_table_name(),regx_columns))
                    return true;
            }
        }

        {
            std::regex regx_db("performance_schema", std::regex_constants::icase | std::regex_constants::ECMAScript);
            if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_db_name(),regx_db))
            {
                // it's a table of the performance_schema

                std::regex regx_columns("file_instances|file_summary_by_instance|objects_summary_global_by_type|table_handles|table_io_waits_summary_by_table|table_lock_waits_summary_by_table", std::regex_constants::icase | std::regex_constants::ECMAScript);
                if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_table_name(),regx_columns))
                    return true;
            }
        }

    }

    return false;
}

data::ListTableData* ListTableAction::getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query)
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
            std::regex regx_columns("(COLUMNS|FILES|KEY_COLUMN_USAGE|PARTITIONS|TABLES)", std::regex_constants::icase | std::regex_constants::ECMAScript);
            if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_table_name(),m,regx_columns))
                table += m[0];

        }
    }

    {
        std::regex regx_db("performance_schema", std::regex_constants::icase | std::regex_constants::ECMAScript);
        if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_db_name(),regx_db))
        {
            // it's a table of the performance_schema

            schema += "performance_schema";
            std::regex regx_columns("file_instances|file_summary_by_instance|objects_summary_global_by_type|table_handles|table_io_waits_summary_by_table|table_lock_waits_summary_by_table", std::regex_constants::icase | std::regex_constants::ECMAScript);
            if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_table_name(),m,regx_columns))
                table += m[0];
        }
    }

    data::ListTableData* data = new data::ListTableData();
    data->setDatabase(schema);
    data->setTable(table);
    data->setQueryType(q.getQueryType());

    return data;
}

}
