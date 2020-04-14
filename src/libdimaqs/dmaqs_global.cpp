/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "dmaqs_global.h"

bool vectorContains(const std::vector<std::string>& l, const std::string& e)
{
    for (auto it = std::begin(l); it!=std::end(l); ++it) {
        if( strcmp((*it).c_str(),e.c_str()) == 0)
            return true;
    }

    return false;
}

MYSQL_LEX_STRING make_lex_string(const std::string &str)
{
  MYSQL_LEX_STRING lex_str= { const_cast<char*>(str.c_str()), str.length() };
  return lex_str;
}
