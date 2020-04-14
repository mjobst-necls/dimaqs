/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef CREATETRIGGERACTION_H
#define CREATETRIGGERACTION_H

#include "../../../lib/petrinet/src/PlaceAction.h"
#include "plugin.h"

class TriggerCreator;

namespace placeaction{

class CreateTriggerAction : public petrinet::PlaceAction
{
public:
    CreateTriggerAction(TriggerCreator* triggerCreator);
    CreateTriggerAction(const CreateTriggerAction& other);

    void execute();
    CreateTriggerAction* create(const petrinet::TokenData& tokenData);

    bool equals(const PlaceAction& other) const;

    void setDatabaseName(const std::string& databaseName) { databaseName_ = databaseName; }
    void setTableName(const std::string& tableName) { tableName_ = tableName; }
    void setTHD(MYSQL_THD thd) { thd_ = thd; }

    std::string getDatabaseName() const { return databaseName_; }
    std::string getTableName() const { return tableName_; }

private:
    std::string databaseName_;
    std::string tableName_;

    TriggerCreator* triggerCreator_;
    MYSQL_THD       thd_;
};

}

#endif // CREATETRIGGERACTION_H
