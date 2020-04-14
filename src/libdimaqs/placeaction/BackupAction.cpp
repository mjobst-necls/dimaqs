/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "BackupAction.h"
#include "transitionaction/DropDatabaseAction.h"
#include "transitionaction/DropTableAction.h"
#include "../../lib/petrinet/src/Transition.h"
#include "QueryRewriter.h"

#include "Controller.h"

namespace placeaction{

BackupAction::BackupAction(QueryRewriter& rewriter)
    : petrinet::PlaceAction(),
      rewriter_(rewriter)
{
    setName("BackupAction");
}

BackupAction::BackupAction(const BackupAction& other)
    : petrinet::PlaceAction(other),
      databaseName_(other.databaseName_),
      tableName_(other.tableName_),
      rewriter_(other.rewriter_),
      thd_(other.thd_)
{
}

void BackupAction::execute()
{
    std::string msg = "Backup Action: " + databaseName_ + "." + tableName_;
    Controller::s_printMessageDirect(msg);
    rewriter_.backup(thd_, databaseName_, tableName_);
}

BackupAction::ObjectType BackupAction::getObjectType() const
{
     if( tableName_.empty() )
         return Database;

     return Table;
}

bool BackupAction::equals(const PlaceAction& other) const
{
    const BackupAction* other_ = dynamic_cast<const BackupAction*>(&other);

    if( !other_ )
        return false;

    if( tableName_ != other_->tableName_ )
        return false;

    if( databaseName_ != other_->databaseName_ )
        return false;

    return true;
}

BackupAction* BackupAction::create(const petrinet::TokenData& tokenData)
{
    BackupAction* action = new BackupAction(rewriter_);

    {
        const transitionaction::data::DropTableData* a = dynamic_cast<const transitionaction::data::DropTableData*>(&tokenData);
        if( a != 0 ){
            action->setDatabaseName( a->getDatabase());
            action->setTableName(a->getTablename());
            return action;
        }
    }

    {
        const transitionaction::data::DropDatabaseData* a = dynamic_cast<const transitionaction::data::DropDatabaseData*>(&tokenData);
        if( a != 0 ){
            action->setDatabaseName( a->getDatabase());
            return action;
        }
    }

    return NULL;
}

}
