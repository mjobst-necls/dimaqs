/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "dmaqs_variable.h"
#include "Controller.h"

void update_excluded_dbs(MYSQL_THD thd  MY_ATTRIBUTE((unused)),
                              struct st_mysql_sys_var *var  MY_ATTRIBUTE((unused)),
                              void *var_ptr,
                              const void *save_ptr)
{
    const char* dbs = *static_cast<const char*const*>(save_ptr);

    *(const char**)var_ptr=
    *static_cast<const char**>(var_ptr)
            = *(const char**)save_ptr;

    Controller::s_instance->updateExcludedDbsList(dbs);
}

void update_object_prefix(MYSQL_THD thd  MY_ATTRIBUTE((unused)),
                              struct st_mysql_sys_var *var  MY_ATTRIBUTE((unused)),
                              void *var_ptr,
                              const void *save_ptr)
{
    const char* prefix = *static_cast<const char*const*>(save_ptr);

    *(const char**)var_ptr=
    *static_cast<const char**>(var_ptr)
            = *(const char**)save_ptr;

    Controller::s_instance->updateObjectPrefix(prefix);
}

void update_secret(MYSQL_THD thd  MY_ATTRIBUTE((unused)),
                              struct st_mysql_sys_var *var  MY_ATTRIBUTE((unused)),
                              void *var_ptr,
                              const void *save_ptr)
{
    // we do not allow to update the secret

    *(const char**)var_ptr= sysvar_secret;
}


