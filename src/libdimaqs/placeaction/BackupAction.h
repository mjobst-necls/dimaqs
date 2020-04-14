/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef BACKUPACTION_H
#define BACKUPACTION_H

#include "../../../lib/petrinet/src/PlaceAction.h"
#include "plugin.h"

class QueryRewriter;

namespace petrinet{
class TokenData;
}

namespace placeaction{

class BackupAction : public petrinet::PlaceAction
{
public:
    enum ObjectType{
        Database,
        Table
    };

    explicit BackupAction(QueryRewriter& rewriter);
    BackupAction(const BackupAction& other);

    void execute();
    BackupAction* create(const petrinet::TokenData& tokenData);

    bool equals(const PlaceAction& other) const;

    void setDatabaseName(const std::string& databaseName) { databaseName_ = databaseName; }
    void setTableName(const std::string& tableName) { tableName_ = tableName; }

    std::string getDatabaseName() const { return databaseName_; }
    std::string getTableName() const { return tableName_; }

    ObjectType getObjectType() const;
    void setTHD(MYSQL_THD thd) { thd_ = thd; }

private:
    std::string databaseName_;
    std::string tableName_;

    QueryRewriter& rewriter_;
    MYSQL_THD       thd_;

};

}

#endif // BACKUPACTION_H
