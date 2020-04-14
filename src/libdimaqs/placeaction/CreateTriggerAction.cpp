/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "CreateTriggerAction.h"
#include "../../lib/petrinet/src/TokenData.h"
#include "transitionaction/data/CreateTableData.h"
#include "../TriggerCreator.h"

namespace placeaction{

CreateTriggerAction::CreateTriggerAction(TriggerCreator* triggerCreator)
    : triggerCreator_(triggerCreator),
      thd_(0)
{
}

CreateTriggerAction::CreateTriggerAction(const CreateTriggerAction& other)
    : petrinet::PlaceAction(other),
      triggerCreator_(other.triggerCreator_),
      thd_(other.thd_),
      tableName_(other.tableName_),
      databaseName_(other.databaseName_)
{
}

void CreateTriggerAction::execute()
{
    triggerCreator_->createTriggersForTable(thd_, databaseName_, tableName_);
}

bool CreateTriggerAction::equals(const PlaceAction& other) const
{
    const CreateTriggerAction* other_ = dynamic_cast<const CreateTriggerAction*>(&other);

    if( !other_ )
        return false;

    if( tableName_ != other_->tableName_ )
        return false;

    if( databaseName_ != other_->databaseName_ )
        return false;

    return true;
}

CreateTriggerAction* CreateTriggerAction::create(const petrinet::TokenData& tokenData)
{
    CreateTriggerAction* action = NULL;

    const transitionaction::data::CreateTableData* data_ = dynamic_cast<const transitionaction::data::CreateTableData*>(&tokenData);
    if( data_ ){
        action = new CreateTriggerAction(triggerCreator_);
        action->setDatabaseName(data_->getDatabase());
        action->setTableName(data_->getTablename());
        action->setExecuteLater(true);
        action->setTHD(thd_);
    }

    return action;
}

}
