/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "DatabasePetriNetQuery.h"
#include "sql_lex.h"

DatabasePetriNetQuery::DatabasePetriNetQuery()
    : lex_(0)
{
}

enum_sql_command DatabasePetriNetQuery::getActionType() const {

    if( !dbname_.empty() && !tablename_.empty() && inserted_values_.size() > 0)
        return SQLCOM_INSERT;

    if( !lex_ )
        return SQLCOM_END;

    if( lex_->sql_command == SQLCOM_INSERT || lex_->sql_command == SQLCOM_INSERT_SELECT )
        return SQLCOM_END;

    return lex_->sql_command;
}

std::string DatabasePetriNetQuery::getQuery() const {
    return query_;
}

std::string DatabasePetriNetQuery::getQueryType() const {
    std::string info;

    switch (getActionType()){
    case SQLCOM_SHOW_DATABASES:
        info = "ShowDatabases";
        break;
    case SQLCOM_SHOW_TABLES:
        info = "ShowTables";
        break;
    case SQLCOM_SHOW_FIELDS:
        info = "ShowFields";
        break;
    case SQLCOM_SHOW_VARIABLES:
        info = "ShowVariables";
        break;
    case SQLCOM_CREATE_TABLE:
        info = "CreateTable";
        break;
    case SQLCOM_DROP_DB:
        info = "DropDatabase";
        break;
    case SQLCOM_DROP_TABLE:
        info = "DropTable";
        break;
    case SQLCOM_ALTER_TABLE:
        info = "AlterTable";
        break;
    case SQLCOM_SELECT:
        info = "Select";
        break;
    case SQLCOM_SHOW_OPEN_TABLES:
        info = "ShowOpenTables";
        break;
    case SQLCOM_INSERT:
    case SQLCOM_INSERT_SELECT:
        info = "Insert";
        break;
    case SQLCOM_SHOW_TABLE_STATUS:
        info = "ShowTableStatus";
        break;
    case SQLCOM_SET_OPTION:
        info = "SetOption";
        break;
    }

    return info;
}

void DatabasePetriNetQuery::setInsertValues(const std::string& dbname, const std::string& tablename, const std::vector<std::string>& values)
{
    dbname_ = dbname;
    tablename_ = tablename;
    inserted_values_ = values;
}
