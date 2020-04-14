/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "QueryRewriter.h"
#include "DatabasePetriNetQuery.h"

#include "sql_rename.h"
#include "thr_lock.h"
#include "mysql/mysql/service_parser.h"

#include "Controller.h"

#include "TableFunctions.h"

#include "sql_table.h"
#include "sql_trigger.h"
#include "dmaqs_global.h"
#include "services.h"
#include "sql_class.h"
#include "placeaction/RewriteQueryAction.h"

#include "transitionaction/ListDatabaseAction.h"
#include "regex"
#include "../regex/my_regex.h"
#include "item_cmpfunc.h"
#include "sql_select.h"
#include "sql_base.h"

QueryRewriter::QueryRewriter(const std::string& object_prefix, const std::string& storagespace)
    : object_prefix_(object_prefix),
      storagespace_(storagespace)
{
}

bool parse(MYSQL_THD thd, const std::string &query, bool is_prepared)
{
  MYSQL_LEX_STRING query_str= make_lex_string(query);
  return mysql_parser_parse(thd, query_str, is_prepared, NULL, NULL);
}

void QueryRewriter::rewriteItemField(const DatabasePetriNetQuery& q, const std::string& notLikeValue, const std::string& fieldName){
    CHARSET_INFO *cs= system_charset_info;
    Item  *where_cond = q.getLex()->select_lex->where_cond();

    Item_field* item_field = new Item_field(&q.getLex()->select_lex->context, NullS, NullS, fieldName.c_str());
    Item_string* item_mask = new Item_string(notLikeValue.c_str(), notLikeValue.length(), cs);
    Item_string* item_escape = new Item_string("\\",1,&my_charset_latin1);

    Item* item_cond = new (q.getThd()->mem_root) Item_func_like(item_field,item_mask,item_escape, FALSE);
    Item* item_cond_not = new (q.getThd()->mem_root) Item_func_not(item_cond);

    if( where_cond ){
        q.getLex()->select_lex->set_where_cond(and_items(q.getLex()->select_lex->where_cond(), item_cond_not));
    } else {
        q.getLex()->select_lex->set_where_cond(item_cond_not);
    }

    String str;
    q.getLex()->select_lex->print(q.getThd(), &str, QT_ORDINARY);
    Controller::s_printMessageDirect(std::string(str.c_ptr(), str.length()));

    parse(q.getThd(), std::string(str.c_ptr(), str.length()), false);
}

void QueryRewriter::rewriteSingleColumn(const DatabasePetriNetQuery& q, const std::string& tmp_colname, const std::string& notLikeValue){


    ST_SCHEMA_TABLE* schema_table = q.getLex()->select_lex->get_table_list()->schema_table;
    ST_FIELD_INFO *fields_info= schema_table->fields_info;


    Item_field* item_field = NULL;

    if( !schema_table )
        Controller::s_printMessage("No Schema Table");

    for (; fields_info->field_name; fields_info++){
        if( fields_info->field_name == tmp_colname ) {
            item_field = new (q.getThd()->mem_root) Item_field(&q.getLex()->select_lex->context, NullS, NullS, fields_info->field_name);
            Controller::s_printMessage(std::string("Field name: ") + fields_info->field_name);
            rewriteItemField(q, notLikeValue, fields_info->field_name);
            break;
        }
    }


}

void QueryRewriter::rewriteVariable(const DatabasePetriNetQuery& q, const std::string& notLikeValue){
    std::string tmp_colname = "Variable_name";

    rewriteItemField(q, notLikeValue, tmp_colname);
}

RewriteResult QueryRewriter::rewrite(MYSQL_THD thd, placeaction::RewriteQueryAction::QueryType& queryType, const DatabasePetriNetQuery& q)
{
    RewriteResult result;
    result.was_rewritten = false;

    if( q.getActionType() == SQLCOM_SHOW_TABLES || q.getActionType() == SQLCOM_SHOW_FIELDS)
        return result;

    std::string new_query = std::string(q.getThd()->query().str, q.getThd()->query().length);

    if( queryType == placeaction::RewriteQueryAction::Database){

        if( q.getActionType() == SQLCOM_SHOW_DATABASES){
            result.was_rewritten = true;

            // rewrite show databases queries
            {
                std::regex regx_where("^show\\sdatabases\\sWHERE.*", std::regex_constants::icase | std::regex_constants::ECMAScript);
                std::regex regx_like("^show\\sdatabases\\sLIKE.*", std::regex_constants::icase | std::regex_constants::ECMAScript);

                if (std::regex_match (new_query,regx_where)){
                    std::regex regx_find("\\b(show\\sdatabases\\sWHERE)([^ ]*)", std::regex_constants::icase | std::regex_constants::ECMAScript);
                    new_query = std::regex_replace (new_query,regx_find,"show databases WHERE `Database` not like 'dmaqs' AND ", std::regex_constants::format_first_only);
                } else if (std::regex_match (new_query,regx_like)){
                    std::regex regx_find("\\b(show\\sdatabases\\sLIKE)([^ ]*)", std::regex_constants::icase | std::regex_constants::ECMAScript);
                    new_query = std::regex_replace (new_query,regx_find,"show databases WHERE `Database` not like 'dmaqs' AND `Database` LIKE ", std::regex_constants::format_first_only);
                } else {
                    new_query = new_query + " WHERE `Database` not like 'dmaqs'";
                }
            }

            parse(thd, new_query, false);
        } else if(      q.getLex()->select_lex
                   &&   q.getLex()->select_lex->get_table_list() )
        {
            {
                std::regex regx_db("INFORMATION_SCHEMA", std::regex_constants::icase | std::regex_constants::ECMAScript);
                if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_db_name(),regx_db))
                {
                    // its a table of the INFORMATION_SCHEMA
                    {
                        std::regex regx_columns("(COLUMNS|KEY_COLUMN_USAGE|PARTITIONS|TABLES)", std::regex_constants::icase | std::regex_constants::ECMAScript);
                        if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_table_name(),regx_columns)){
                            std::string tmp_colname = "TABLE_SCHEMA";
                            rewriteSingleColumn(q, tmp_colname, "dmaqs");
                        }
                    }

                    {
                        std::regex regx_columns("(FILES)", std::regex_constants::icase | std::regex_constants::ECMAScript);
                        if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_table_name(),regx_columns)){
                            std::string tmp_colname = "FILE_NAME";
                            rewriteSingleColumn(q, tmp_colname, "./dmaqs/%");
                        }
                    }

                    {
                        std::regex regx_columns("(SCHEMATA)", std::regex_constants::icase | std::regex_constants::ECMAScript);
                        if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_table_name(),regx_columns)){
                            std::string tmp_colname = "SCHEMA_NAME";
                            rewriteSingleColumn(q, tmp_colname, "dmaqs");
                        }
                    }

                }
            }

            {
                std::regex regx_db("performance_schema", std::regex_constants::icase | std::regex_constants::ECMAScript);
                if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_db_name(),regx_db))
                {
                    // its a table of the performance_schema
                    {
                        std::regex regx_columns("file_instances|file_summary_by_instance", std::regex_constants::icase | std::regex_constants::ECMAScript);
                        if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_table_name(),regx_columns)){
                            std::string tmp_colname = "FILE_NAME";
                            rewriteSingleColumn(q, tmp_colname, "%/dmaqs/%");
                        }
                    }

                    {
                        std::regex regx_columns("table_handles|table_io_waits_summary_by_table|table_lock_waits_summary_by_table", std::regex_constants::icase | std::regex_constants::ECMAScript);
                        if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_table_name(),regx_columns)){
                            std::string tmp_colname = "OBJECT_SCHEMA";
                            rewriteSingleColumn(q, tmp_colname, "dmaqs[_]%");
                        }
                    }

                    {
                        std::regex regx_columns("objects_summary_global_by_type", std::regex_constants::icase | std::regex_constants::ECMAScript);
                        if (std::regex_match (q.getLex()->select_lex->get_table_list()->get_table_name(),regx_columns)){
                            std::string tmp_colname = "OBJECT_NAME";
                            rewriteSingleColumn(q, tmp_colname, "dmaqs[_]%");
                        }
                    }
                }
            }
        }
    } else if( queryType == placeaction::RewriteQueryAction::Variable){
        rewriteVariable(q, "dmaqs%");
    }

    return result;
}

RewriteResult QueryRewriter::backupTable(MYSQL_THD thd, const std::string& databaseName, const std::string& tableName)
{
    RewriteResult result;
    result.was_rewritten = true;

    HA_CREATE_INFO create_info;
    Alter_info alter_info;
    TABLE_LIST table_list;
    time_t ts;
    time(&ts);

    std::string newTableName = object_prefix_ + "_" + databaseName  + "_" + tableName + "_" + std::to_string(ts);

    table_list.init_one_table(databaseName.c_str(), databaseName.length(),
                                   tableName.c_str(), tableName.length(),
                                   "alias", TL_READ_NO_INSERT);


    table_list.lock_type= TL_READ_NO_INSERT;
    table_list.mdl_request.set_type(MDL_SHARED_NO_WRITE);
    memset(&create_info, 0, sizeof(create_info));
    create_info.row_type=ROW_TYPE_NOT_USED;
    create_info.default_table_charset=default_charset_info;

    alter_info.flags= Alter_info::ALTER_RENAME;

    // drop triggers
    drop_all_triggers(thd,databaseName.c_str(),tableName.c_str());

    // rename table
    if( !mysql_alter_table(thd, storagespace_.c_str(), newTableName.c_str(), &create_info,
                                        &table_list, &alter_info) )
    {
        std::string msg = "Table " + databaseName + "." + tableName + " renamed to " + storagespace_ + "." + newTableName;
        Controller::s_printMessageDirect(msg);
        // OK
    } else {
        std::string msg = "ERROR: Not possible to rename Table " + databaseName + "." + tableName + " to " + storagespace_ + "." + newTableName;
        Controller::s_printMessageDirect(msg);
        // NOK
    }

    return result;
}

RewriteResult QueryRewriter::backup(MYSQL_THD thd, const std::string& databaseName, const std::string& tableName)
{
    RewriteResult result;
    result.was_rewritten = false;


    if( !tableName.empty() )
    {
        backupTable(thd, databaseName, tableName);
    } else {
        LOOKUP_FIELD_VALUES lookup_field_vals;

        thd->security_context()->set_master_access(GLOBAL_ACLS);

        List<LEX_STRING> table_names ;
        TableFunctions::make_table_name_list(thd, &table_names,
                             &lookup_field_vals,
                             false, databaseName,
                             NULL);

        while( !table_names.is_empty() ){
            LEX_STRING* name = table_names.head();
            std::string tableName_;
            tableName_.assign(name->str, name->length);
            table_names.pop();

            backupTable(thd, databaseName, tableName_);
        }
    }

    return result;
}


