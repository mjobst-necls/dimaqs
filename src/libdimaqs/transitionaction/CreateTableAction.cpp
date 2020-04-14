/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "CreateTableAction.h"
#include "../DatabasePetriNetQuery.h"
#include "../Controller.h"
#include "sql_lex.h"
#include "field.h"
#include "sql_table.h"

#include "regex"

bool iequals(const std::string& a, const std::string& b)
{
    unsigned int sz = a.size();
    if (b.size() != sz)
        return false;
    for (unsigned int i = 0; i < sz; ++i)
        if (toupper(a[i]) != toupper(b[i]))
            return false;
    return true;
}

namespace transitionaction{

CreateTableAction::CreateTableAction(std::list<std::string>& malicious_table_names)
    : malicious_table_names_(malicious_table_names)
{

}

bool CreateTableAction::isMatching(const petrinet::AbstractPetriNetQuery& query)
{
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);
    if( q.getActionType() == SQLCOM_CREATE_TABLE ){

        // check if user is of category user

        if( q.getLex()->create_info.options & HA_LEX_CREATE_TMP_TABLE ){
            return false;
        }

        if( malicious_table_names_.size() == 0)
            return true;

        // check table name for malicious table names
        std::string tablename = q.getLex()->select_lex->get_table_list()->get_table_name();

       if( malicious_table_names_.size() == 0)
           return true;

       for (auto it = std::begin(malicious_table_names_); it!=std::end(malicious_table_names_); ++it)
       {
           std::regex regx(*it, std::regex_constants::icase | std::regex_constants::ECMAScript);

            if (std::regex_match (tablename,regx))
                return true;
       }


    }

    return false;
}

data::CreateTableData* CreateTableAction::getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query)
{
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);

    data::CreateTableData* data = new data::CreateTableData();
    data->setTablename(q.getLex()->select_lex->get_table_list()->get_table_name());
    data->setDatabase(q.getLex()->select_lex->get_table_list()->get_db_name());

    return data;
}

}
