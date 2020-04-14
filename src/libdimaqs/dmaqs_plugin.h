/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef ANUBIS_PLUGIN_H
#define ANUBIS_PLUGIN_H

#include "my_config.h"
#include <my_global.h>
#include <mysql/plugin_audit.h>


#define PLUGIN_NAME "dmaqs"
#define PLUGIN_AUTHOR "Michael Jobst"

MYSQL_PLUGIN* get_dmaqs_plugin_info();

#endif // ANUBIS_PLUGIN_H
