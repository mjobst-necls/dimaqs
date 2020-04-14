/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef QUERYREWRITER_H
#define QUERYREWRITER_H

#include "RewriteResult.h"
#include "plugin.h"
#include "placeaction/RewriteQueryAction.h"

namespace petrinet{
class AbstractPetriNetQuery;
}

namespace placeaction{
class BackupAction;
}

class DatabaseTokenData;

class QueryRewriter
{
public:
    explicit QueryRewriter(const std::string& object_prefix, const std::string& storagespace);

    void rewriteVariable(const DatabasePetriNetQuery& q, const std::string& notLikeValue);
    void rewriteSingleColumn(const DatabasePetriNetQuery& q, const std::string& tmp_colname, const std::string& notLikeValue);
    RewriteResult rewrite(MYSQL_THD thd, placeaction::RewriteQueryAction::QueryType& queryType, const DatabasePetriNetQuery& query);
    void rewriteItemField(const DatabasePetriNetQuery& q, const std::string& notLikeValue, const std::string& fieldName);

    RewriteResult backup(MYSQL_THD thd, const std::string& databaseName, const std::string& tableName);
    RewriteResult backupTable(MYSQL_THD thd, const std::string& databaseName, const std::string& tableName);

private:
    const std::string&        object_prefix_;
    const std::string&        storagespace_;
};

#endif // QUERYREWRITER_H
