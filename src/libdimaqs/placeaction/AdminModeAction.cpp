/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "AdminModeAction.h"
#include "transitionaction/DropDatabaseAction.h"
#include "transitionaction/DropTableAction.h"
#include "../../lib/petrinet/src/Transition.h"
#include "QueryRewriter.h"
#include "Controller.h"
#include "transitionaction/data/SetVariableData.h"

namespace placeaction{

AdminModeAction::AdminModeAction(AdminHandler& adminHandler)
    : petrinet::PlaceAction(),
      adminHandler_(adminHandler)
{
    setName("AdminModeAction");
}

AdminModeAction::AdminModeAction(const AdminModeAction& other)
    : petrinet::PlaceAction(other),
      adminHandler_(other.adminHandler_),
      varname_(other.varname_),
      varvalue_(other.varvalue_)
{
}

void AdminModeAction::execute()
{
    Controller::s_printMessageDirect(varname_);
    Controller::s_printMessageDirect(varvalue_);

    if( adminHandler_.checkSecret(varvalue_)){
        adminHandler_.setIsAdmin(thd_, true);
        Controller::s_printMessageDirect("set admin mode on");
    } else {
        adminHandler_.setIsAdmin(thd_, false);
        Controller::s_printMessageDirect("set admin mode off");
    }
}

bool AdminModeAction::equals(const PlaceAction& other) const
{
    const AdminModeAction* other_ = dynamic_cast<const AdminModeAction*>(&other);

    if( !other_ )
        return false;

    return true;
}

AdminModeAction* AdminModeAction::create(const petrinet::TokenData& tokenData)
{
    AdminModeAction* action = new AdminModeAction(adminHandler_);

    {
        const transitionaction::data::SetVariableData* a = dynamic_cast<const transitionaction::data::SetVariableData*>(&tokenData);
        if( a != 0 ){
            action->setVarname(a->getVarname());
            action->setVarvalue(a->getVarvalue());
        }
    }

    return action;
}

}
