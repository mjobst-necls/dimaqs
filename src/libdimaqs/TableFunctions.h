/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef TABLEFUNCTIONS_H
#define TABLEFUNCTIONS_H

#include "plugin.h"
#include "sql_list.h"
#include "sql_lex.h"

int get_all_tables(THD *thd, TABLE_LIST *tables, Item *cond);
TABLE *create_schema_table(THD *thd, TABLE_LIST *table_list);

int schema_tables_add(THD *thd, List<LEX_STRING> *files, const char *wild);

typedef struct st_lookup_field_values
{
  LEX_STRING db_value;
  LEX_STRING table_value;
  bool wild_db_value;
  bool wild_table_value;
} LOOKUP_FIELD_VALUES;

class TableFunctions
{
public:
    static int make_table_name_list(THD *thd, List<LEX_STRING> *table_names,
                         LOOKUP_FIELD_VALUES *lookup_field_vals,
                         bool with_i_schema, LEX_STRING *db_name,
                         MEM_ROOT *tmp_mem_root);
    static int make_table_name_list(THD *thd, List<LEX_STRING> *table_names,
                         LOOKUP_FIELD_VALUES *lookup_field_vals,
                         bool with_i_schema, const std::string& db_name,
                         MEM_ROOT *tmp_mem_root);
    static bool tableExists(THD *thd, const std::string& dbName, const std::string& tableName, MEM_ROOT *tmp_mem_root = NULL);
};

int make_db_list(THD *thd, List<LEX_STRING> *files,
                 LOOKUP_FIELD_VALUES *lookup_field_vals,
                 bool *with_i_schema, MEM_ROOT *tmp_mem_root);

#endif // TABLEFUNCTIONS_H
