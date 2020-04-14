/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "TriggerCreator.h"
#include "template_utils.h"

#include "sstream"
#include "../regex/my_regex.h"
#include <mysql/service_rules_table.h>
#include "thr_lock.h"
#include "table.h"
#include "handler.h"
#include "my_base.h"
#include "my_bitmap.h"
#include "field.h"
#include "sql_string.h"
#include "transaction.h"
#include "sql_lex.h"
#include "sql_base.h"
#include "sql_show.h"
#include "item_cmpfunc.h"                   // Item_cond       // check_grant_db
#include "sql_table.h"                      // filename_to_tablename
#include "tztime.h"                         // Time_zone
#include "table_trigger_dispatcher.h"
#include "sql_trigger.h"
#include "trigger.h"
#include "sp_head.h"
#include "sql_parse.h"

#include "sql_handler.h"

#include "services.h"

#include "TableFunctions.h"

#include "dmaqs_plugin.h"
#include "dmaqs_global.h"

#include "Controller.h"


/*#include "QtSql/qsqldatabase.h"
#include "QtSql/qsqlquery.h"
#include "QtSql/qsqlrecord.h"
#include "qvariant.h"

void CreatorWorker::createTriggers()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL", "default");
        db.setHostName("localhost");
        db.setDatabaseName("information_schema");
        db.setUserName("root");
        db.setPassword("toor");
    if( !db.open() )
    {
        // write error message
    }

    QSqlQuery query("SELECT * FROM information_schema.tables WHERE TABLE_TYPE like 'BASE TABLE' AND TABLE_SCHEMA not like 'mysql' and TABLE_SCHEMA not like 'information_schema' and TABLE_SCHEMA not like 'performance_schema'");
    int tableSchemaColNr = query.record().indexOf("TABLE_SCHEMA");
    int tableNameColNr = query.record().indexOf("TABLE_NAME");
    while (query.next()) {
        QString tableSchema = query.value(tableSchemaColNr).toString();
        QString tableName = query.value(tableNameColNr).toString();
        createTrigger(tableSchema, tableName);
    }

}

void CreatorWorker::createTrigger(const QString& tableSchema, const QString& tableName)
{
    QSqlDatabase db_orig = QSqlDatabase::database("default");
    QSqlDatabase db = QSqlDatabase::cloneDatabase(db_orig, "new");
    db.setDatabaseName(tableSchema);

    if( !db.open() )
    {
        // write error message
    }

    QString q_drop = "DROP TRIGGER IF EXISTS anubis_" + tableName + "_i;";
    QString q_create =  "Delimiter $$\n"
                    +   QString("CREATE DEFINER=`root`@`localhost` TRIGGER anubis_") + tableName + "_i\n"
                    +   "BEFORE INSERT ON " + tableName + "\n"
                    +   "FOR EACH ROW\n"
                    +   "begin\n"
                    +   "\tdeclare stat varchar(20);\n"
                    +   "\t\n"
                    +   "\tset stat = eval_value_insert('test', 'test', 'id', 1);\n"
                    +   "\tset stat = eval_value_insert('test', 'test', 'value', 2);\n"
                    +   "end $$";


    QSqlQuery query_drop(q_drop, db);
    query_drop.exec();

    QSqlQuery query_create(q_create, db);
    query_create.exec();

    db.close();
}
*/

static mysql_rwlock_t LOCK_table;

namespace {
struct CreateTriggers_callback_args
{
  TriggerCreator *me;
  MYSQL_THD session_thd;
  std::string dbName;
  std::string tableName;
};

extern "C"
void *createTriggers_callback(void *p_args)
{
  CreateTriggers_callback_args *args= pointer_cast<CreateTriggers_callback_args*>(p_args);
  (args->me->do_createTriggers)(args->session_thd, args->dbName, args->tableName);
  return NULL;
}

} // namespace

namespace triggercreatorservice {

}

TriggerCreator::TriggerCreator(const std::vector<std::string>& excluded_dbs, const std::string& object_prefix)
    : excluded_dbs_(excluded_dbs),
      object_prefix_(object_prefix)
{
}

TriggerCreator::~TriggerCreator()
{
}

void TriggerCreator::createTriggers(MYSQL_THD thd, bool& triggersCreated)
{
    mysql_rwlock_wrlock(&LOCK_table);

    if( !triggersCreated ) {
        if( !object_prefix_.empty() ){
            createTriggersInThread(thd);
        }
    }

    triggersCreated = true;
    mysql_rwlock_unlock(&LOCK_table);
}

void TriggerCreator::createTriggersForTable(MYSQL_THD thd, const std::string& dbName, const std::string& tableName)
{
    mysql_rwlock_wrlock(&LOCK_table);

    std::string msg;
    msg += "Create Trigger for: ";
    msg += dbName + " " + tableName;
    Controller::s_printMessageDirect(msg);

    services::Session session(thd);

    CreateTriggers_callback_args args= { this, session.thd(), dbName, tableName};
    my_thread_handle handle;
    mysql_parser_start_thread(session.thd(), createTriggers_callback, &args, &handle);
    mysql_parser_join_thread(&handle);

    mysql_rwlock_unlock(&LOCK_table);
}

void TriggerCreator::createTriggersInThread(MYSQL_THD thd)
{
    services::Session session(thd);

    CreateTriggers_callback_args args= { this, session.thd(), "", ""};
    my_thread_handle handle;
    mysql_parser_start_thread(session.thd(), createTriggers_callback, &args, &handle);
    mysql_parser_join_thread(&handle);
}

class Silence_deprecation_warnings : public Internal_error_handler
{
public:
  virtual bool handle_condition(THD *thd,
                                uint sql_errno,
                                const char* sqlstate,
                                Sql_condition::enum_severity_level *level,
                                const char* msg)
  {
    if (sql_errno == ER_WARN_DEPRECATED_SYNTAX)
      return true;

    return false;
  }
};

void TriggerCreator::do_createTriggers(MYSQL_THD thd, const std::string& dbName, const std::string& tableName)
{
    thd->security_context()->set_master_access(GLOBAL_ACLS);

    List<LEX_STRING> table_names;
    List<LEX_STRING> db_names;
    LOOKUP_FIELD_VALUES lookup_field_vals;
    LEX_STRING *table_name, *database_name;

    bool with_i_schema = false;

    if( dbName.empty() && tableName.empty() ){

        // create trigger for all non excluded tables
        make_db_list(thd, &db_names,
                     &lookup_field_vals,
                     &with_i_schema, NULL);

        List_iterator_fast<LEX_STRING> it_databases(db_names);
        while ((database_name= it_databases++))
        {
            if( vectorContains(excluded_dbs_, database_name->str) )
            {
                continue;
            }

            std::stringstream scols;
            scols << "do_createTriggers,dbname:" << std::string(database_name->str, database_name->length);

            TableFunctions::make_table_name_list(thd, &table_names,
                                 &lookup_field_vals,
                                 false, database_name,
                                 NULL);

            List_iterator_fast<LEX_STRING> it_files(table_names);
            while ((table_name= it_files++))
            {
                std::stringstream scols;
                scols << "do_createTriggers,tablename:" << std::string(table_name->str, table_name->length);
                std::string dbName_temp(database_name->str, database_name->length);
                std::string tableName_temp(table_name->str, table_name->length);

                do_createSingleTrigger(thd, dbName_temp, tableName_temp);
            }
        }
    } else {
        // create trigger for single table
        do_createSingleTrigger(thd, dbName, tableName);
    }

}

void TriggerCreator::do_createSingleTrigger(MYSQL_THD thd, const std::string& dbName, const std::string& tableName)
{
    TABLE_LIST table_list;

    table_list.init_one_table(dbName.c_str(), dbName.length(),
                                   tableName.c_str(), tableName.length(),
                                   "alias", TL_READ_NO_INSERT);

    TABLE *table= open_n_lock_single_table(thd, &table_list,
                                    TL_READ_NO_INSERT, 0);

      if (table == NULL)
        return; // Error

      std::string trigger_name = object_prefix_ + "_" + tableName + "_iu";
      std::string q_create;// =  "Delimiter $$\n";
      q_create += "CREATE DEFINER=`root`@`localhost` TRIGGER " + trigger_name + "\n";
      q_create +=   std::string("BEFORE INSERT ON `") + tableName + "`\n";
      q_create += "FOR EACH ROW\n";
      q_create += "begin\n";
      q_create += "\tdeclare stat varchar(20);\n";
      q_create += "\t\n";

      q_create += "\tset stat = eval_value_iu(";
      q_create += "'" + dbName + "',";
      q_create += "'" + tableName + "',";

     for (uint i= 0; i < table->s->fields; ++i)
     {
         q_create += "New." + std::string(table->field[i]->field_name);

         if( i < table->s->fields -1 ){
             q_create += ",";
         } else {
             q_create += ");\n";
         }
     }

     close_all_tables_for_name(thd, table->s, false, NULL);

     q_create += "end";// $$";


     services::set_current_database(thd, dbName);
     services::parse(thd, q_create, false);
     std::string q_drop;
     q_drop = "DROP TRIGGER IF EXISTS " + dbName + "." + trigger_name;
     services::parse(thd, q_drop, false);
     mysql_create_or_drop_trigger(thd, thd->lex->query_tables, false);

     services::parse(thd, q_create, false);
     if( !mysql_create_or_drop_trigger(thd, thd->lex->query_tables, true) )
     {
         // OK
         trans_commit_stmt(thd);

     } else {
         // error
         trans_rollback_stmt(thd);
     }

     close_thread_tables(thd);
}

