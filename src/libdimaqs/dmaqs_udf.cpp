/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include <my_global.h>
#include <mysql.h>
#include "dmaqs_plugin.h"
#include "sstream"
#include "mysql/service_my_plugin_log.h"

#include "Controller.h"

using namespace std;

extern "C" {

my_bool eval_value_iu_init(UDF_INIT *initid, UDF_ARGS *args, char *message)
{
  if (get_dmaqs_plugin_info() != NULL)
    return 0;
  strncpy(message, "DIMAQS plugin needs to be installed.", MYSQL_ERRMSG_SIZE);
  return 1;
}

char *eval_value_iu(UDF_INIT *initid, UDF_ARGS *args,
          char *result, unsigned long *length,
          char *is_null, char *error)
{
    Controller::s_instance->handleInsertUpdate(args);

    const char *message= "OK";
    *length= static_cast<unsigned long>(strlen(message));
    return const_cast<char*>(message);
}

void eval_value_iu_deinit(UDF_INIT *initid) {}

}
