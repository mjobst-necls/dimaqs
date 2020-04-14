/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef DMAQS_VARIABLE_H
#define DMAQS_VARIABLE_H

#include "dmaqs_plugin.h"
#include "sql_const.h"
#include "template_utils.h"

static char*	sysvar_secret = NULL;
static char*    sysvar_object_prefix = NULL;
static char*    sysvar_email = NULL;
static char*    sysvar_excluded_dbs = NULL;
static char*    sysvar_storagespace = NULL;
static my_bool  sysvar_mode = false;
static uint     sysvar_verbosity;
static uint     sysvar_maxcapacity;

static st_mysql_show_var dmaqs_plugin_status_vars[]=
{
  {PLUGIN_NAME "_secret", pointer_cast<char*>(&sysvar_secret), SHOW_CHAR_PTR, SHOW_SCOPE_GLOBAL},
  {PLUGIN_NAME "_object_prefix", pointer_cast<char*>(&sysvar_object_prefix), SHOW_CHAR_PTR, SHOW_SCOPE_GLOBAL},
  {PLUGIN_NAME "_email", pointer_cast<char*>(&sysvar_email), SHOW_CHAR_PTR, SHOW_SCOPE_GLOBAL},
  {PLUGIN_NAME "_excluded_dbs", pointer_cast<char*>(&sysvar_excluded_dbs), SHOW_CHAR_PTR, SHOW_SCOPE_GLOBAL},
  {PLUGIN_NAME "_storagespace", pointer_cast<char*>(&sysvar_storagespace), SHOW_CHAR_PTR, SHOW_SCOPE_GLOBAL},
  {PLUGIN_NAME "_mode", pointer_cast<char*>(&sysvar_mode), SHOW_BOOL, SHOW_SCOPE_GLOBAL},
  {PLUGIN_NAME "_verbosity", pointer_cast<char*>(&sysvar_verbosity), SHOW_INT, SHOW_SCOPE_GLOBAL},
  {PLUGIN_NAME "_max_capacity", pointer_cast<char*>(&sysvar_maxcapacity), SHOW_INT, SHOW_SCOPE_GLOBAL},
  {0, 0, SHOW_BOOL, SHOW_SCOPE_GLOBAL}
};


void update_excluded_dbs(MYSQL_THD thd  MY_ATTRIBUTE((unused)),
                              struct st_mysql_sys_var *var  MY_ATTRIBUTE((unused)),
                              void *var_ptr,
                              const void *save_ptr);

void update_object_prefix(MYSQL_THD thd  MY_ATTRIBUTE((unused)),
                              struct st_mysql_sys_var *var  MY_ATTRIBUTE((unused)),
                              void *var_ptr,
                              const void *save_ptr);

void update_secret(MYSQL_THD thd  MY_ATTRIBUTE((unused)),
                              struct st_mysql_sys_var *var  MY_ATTRIBUTE((unused)),
                              void *var_ptr,
                              const void *save_ptr);

static MYSQL_SYSVAR_STR(secret, sysvar_secret, PLUGIN_VAR_RQCMDARG | PLUGIN_VAR_MEMALLOC, "The secret key for administrator mode.", NULL, update_secret, NULL);
static MYSQL_SYSVAR_STR(object_prefix, sysvar_object_prefix, PLUGIN_VAR_MEMALLOC, "The object prefix for dmaqs db objects.", NULL, update_object_prefix, NULL);
static MYSQL_SYSVAR_STR(email, sysvar_email, PLUGIN_VAR_MEMALLOC, "The email address of the administrator to be notified.", NULL, NULL, NULL);
static MYSQL_SYSVAR_STR(excluded_dbs, sysvar_excluded_dbs, PLUGIN_VAR_OPCMDARG | PLUGIN_VAR_MEMALLOC, "Comma separated values of database names which are excluded from dmaqs functionality.", NULL, update_excluded_dbs, NULL);
static MYSQL_SYSVAR_STR(storagespace, sysvar_storagespace, PLUGIN_VAR_RQCMDARG | PLUGIN_VAR_MEMALLOC, "The database name in which backed up tables are stored.", NULL, NULL, NULL);
static MYSQL_SYSVAR_BOOL(mode, sysvar_mode, PLUGIN_VAR_OPCMDARG, "Whether admin mode is on or off", NULL, NULL, 0);
static MYSQL_SYSVAR_UINT(verbosity, sysvar_verbosity, PLUGIN_VAR_OPCMDARG, "Verbosity level; Min: 0, Max: 3", NULL, NULL, 1, 0, 4, 0);
static MYSQL_SYSVAR_UINT(max_capacity, sysvar_maxcapacity, PLUGIN_VAR_OPCMDARG, "Max backup capacity (MB); Min: 0, Max: not specified", NULL, NULL, 1, 0, UINT_MAX32, 0);



#endif // DMAQS_VARIABLE_H
