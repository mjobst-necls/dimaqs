/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef LIBANUBIS_GLOBAL_H
#define LIBANUBIS_GLOBAL_H

#if defined(LIBANUBIS_LIBRARY)
#  define LIBANUBISSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBANUBISSHARED_EXPORT Q_DECL_IMPORT
#endif

#include "string.h"
#include "string"
#include "vector"
#include <stdio.h>
#include "mysql/mysql_lex_string.h"


bool vectorContains(const std::vector<std::string>& l, const std::string& e);

MYSQL_LEX_STRING make_lex_string(const std::string &str);

#endif // LIBANUBIS_GLOBAL_H
