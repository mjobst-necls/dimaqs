/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "dmaqs_plugin.h"
#include "template_utils.h"
#include "Controller.h"
#include "mysql/service_my_plugin_log.h"
#include "sstream"

#include "dmaqs_variable.h"

static MYSQL_PLUGIN plugin_info;

MYSQL_PLUGIN* get_dmaqs_plugin_info() { return &plugin_info; }


static void release_thd(MYSQL_THD thd);

static int audit_notify(MYSQL_THD thd, mysql_event_class_t event_class,
                                 const void *event);
static int dmaqs_plugin_init(MYSQL_PLUGIN plugin_ref);
static int dmaqs_plugin_deinit(void*);

/* Audit plugin descriptor */
static struct st_mysql_audit dmaqs_query_descriptor=
{
  MYSQL_AUDIT_INTERFACE_VERSION,                    /* interface version */
  release_thd,                                      /* release_thd()     */
  audit_notify,                             /* event_notify()    */
  { (unsigned long) MYSQL_AUDIT_GENERAL_RESULT,
    (unsigned long) MYSQL_AUDIT_CONNECTION_ALL,
    (unsigned long) MYSQL_AUDIT_PARSE_POSTPARSE,
    0,/*(unsigned long) MYSQL_AUDIT_AUTHORIZATION_DB
              | MYSQL_AUDIT_AUTHORIZATION_TABLE
              | MYSQL_AUDIT_AUTHORIZATION_COLUMN,*/
    0, //(unsigned long) MYSQL_AUDIT_TABLE_ACCESS_ALL,
    0,
    0,
    0,
    0,
    0,//(unsigned long) MYSQL_AUDIT_QUERY_ALL,
    0,//(unsigned long) MYSQL_AUDIT_STORED_PROGRAM_EXECUTE,
    }        /* class mask        */
};

struct st_mysql_sys_var* dmaqs_plugin_sys_vars[]=
{
  MYSQL_SYSVAR(secret),
  MYSQL_SYSVAR(object_prefix),
  MYSQL_SYSVAR(email),
  MYSQL_SYSVAR(excluded_dbs),
  MYSQL_SYSVAR(storagespace),
  MYSQL_SYSVAR(mode),
  MYSQL_SYSVAR(verbosity),
  MYSQL_SYSVAR(max_capacity),
  NULL
};

/* Plugin descriptor */
mysql_declare_plugin(audit_log)
{
    MYSQL_AUDIT_PLUGIN,             /* plugin type                   */
    &dmaqs_query_descriptor,        /* type specific descriptor      */
    PLUGIN_NAME,                    /* plugin name                   */
    "Michael Jobst",                /* author                        */
    "A security plugin to identify"
    "malicious query sequences.",   /* description                   */
    PLUGIN_LICENSE_GPL,             /* license                       */
    dmaqs_plugin_init,              /* plugin initializer            */
    dmaqs_plugin_deinit,            /* plugin deinitializer          */
    0x0001,                         /* version                       */
    dmaqs_plugin_status_vars,       /* status variables              */
    dmaqs_plugin_sys_vars,          /* system variables              */
    NULL,                           /* reserverd                     */
    0                               /* flags                         */
}
mysql_declare_plugin_end;

/**
  Entry point to the plugin. The server calls this function after each parsed
  query when the plugin is active. The function extracts the digest of the
  query. If the digest matches an existing rewrite rule, it is executed.
*/

static int audit_notify(MYSQL_THD thd, mysql_event_class_t event_class,
                                 const void *event)
{
    return Controller::s_instance->audit_notify(thd, event_class, event);
}

  static void release_thd(MYSQL_THD thd)
  {
      std::stringstream ss;
      ss << "Release_thd" << ";";
      std::string msg(ss.str());
      my_plugin_log_message(&plugin_info, MY_ERROR_LEVEL, "%s", msg.c_str());
  }

static int dmaqs_plugin_init(MYSQL_PLUGIN plugin_ref)
{
    plugin_info= plugin_ref;

    Controller::s_instance = new Controller();
    Controller::s_instance->init();

    if( sysvar_excluded_dbs )
        Controller::s_instance->updateExcludedDbsList(sysvar_excluded_dbs);

    if( sysvar_object_prefix )
        Controller::s_instance->updateObjectPrefix(sysvar_object_prefix);

    if( sysvar_storagespace )
        Controller::s_instance->updateStorageSpace(sysvar_storagespace);

    if( sysvar_secret )
        Controller::s_instance->updateSecret(sysvar_secret);

    if( sysvar_storagespace )
        Controller::s_instance->updateVerbosity(sysvar_verbosity);

    if( sysvar_email )
        Controller::s_instance->updateRecipient(sysvar_email);

    std::string msg = "dmaqs_plugin_init";
    my_plugin_log_message(&plugin_info, MY_ERROR_LEVEL, "%s", msg.c_str());

    return 0;
}

static int dmaqs_plugin_deinit(void*)
{
    std::string msg = "dmaqs_plugin_deinit";
    my_plugin_log_message(&plugin_info, MY_ERROR_LEVEL, "%s", msg.c_str());

    plugin_info= NULL;
    delete Controller::s_instance;
    Controller::s_instance = 0;
    return 0;
}
