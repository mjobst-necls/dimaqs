/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "TableFunctions.h"
#include "sql_table.h"
#include "sql_class.h"
#include "Controller.h"
#include "sstream"
#include "dmaqs_global.h"

int TableFunctions::make_table_name_list(THD *thd, List<LEX_STRING> *table_names,
                     LOOKUP_FIELD_VALUES *lookup_field_vals,
                     bool with_i_schema, const std::string& db_name,
                     MEM_ROOT *tmp_mem_root)
{
    LEX_STRING db_name_;
    db_name_ = make_lex_string(db_name);

    Controller::s_printMessageDirect("Database Name: " + std::string(db_name_.str, db_name_.length));

    return make_table_name_list(thd, table_names, lookup_field_vals, with_i_schema, &db_name_, tmp_mem_root);
}

int TableFunctions::make_table_name_list(THD *thd, List<LEX_STRING> *table_names,
                     LOOKUP_FIELD_VALUES *lookup_field_vals,
                     bool with_i_schema, LEX_STRING *db_name,
                     MEM_ROOT *tmp_mem_root)
{
  char path[FN_REFLEN + 1];
  build_table_filename(path, sizeof(path) - 1, db_name->str, "", "", 0);

  /*
    This call will add all matching the wildcards (if specified) IS tables
    to the list
  */
  if (with_i_schema)
    return (schema_tables_add(thd, table_names,
                              lookup_field_vals->table_value.str));

  std::stringstream scols;
  scols << "do_createTriggers" << std::string(db_name->str, db_name->length) << "," << path;
  std::string msgcols(scols.str());
  Controller::s_instance->s_printMessageDirect(msgcols.c_str());

  find_files_result res= find_files(thd, table_names, db_name->str, path,
                                    0, 0,
                                    tmp_mem_root);
  if (res != FIND_FILES_OK)
  {
    return 1;
  }
  return 0;
}


bool TableFunctions::tableExists(THD *thd, const std::string& dbName, const std::string& tableName, MEM_ROOT *tmp_mem_root)
{
    //make_table_name_list
    bool exist = false;

    List<LEX_STRING> table_names;
    List<LEX_STRING> db_names;
    LOOKUP_FIELD_VALUES lookup_field_vals;
    bool with_i_schema = false;

    LEX_STRING db_name;
    db_name.str = const_cast<char*>(dbName.c_str());
    db_name.length = dbName.length();

    db_names.push_back(&db_name);

    MEM_ROOT *mem_root = NULL;

    if( tmp_mem_root ){
        mem_root = tmp_mem_root;
    } else {
        mem_root = new MEM_ROOT();
    }

    make_table_name_list(thd, &table_names, &lookup_field_vals, with_i_schema, &db_name, mem_root);


    List_iterator_fast<LEX_STRING> it_tables(table_names);
    LEX_STRING *table_name;
    while ((table_name= it_tables++))
    {
        if( strncmp(table_name->str, tableName.c_str(), (table_name->length < tableName.length()) ? table_name->length : tableName.length()) == 0){
            exist = true;
        }
    }


    if( !tmp_mem_root ){
        delete mem_root;
    }

    return exist;


}
