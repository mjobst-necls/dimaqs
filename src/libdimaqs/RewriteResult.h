/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef REWRITERESULT_H
#define REWRITERESULT_H

#include "string"

struct RewriteResult
{
    RewriteResult() : was_rewritten(false) {}
    RewriteResult(const RewriteResult& other)
        : was_rewritten(other.was_rewritten),
          new_query(other.new_query)
    {}

    bool        was_rewritten;
    std::string new_query;


};

#endif // REWRITERESULT_H
