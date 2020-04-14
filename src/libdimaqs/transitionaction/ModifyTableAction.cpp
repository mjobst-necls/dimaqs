/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "ModifyTableAction.h"
#include "../DatabasePetriNetQuery.h"

namespace transitionaction{

ModifyTableAction::ModifyTableAction()
{

}

bool ModifyTableAction::isMatching(const petrinet::AbstractPetriNetQuery& query)
{
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);
    return q.getActionType() == SQLCOM_ALTER_TABLE;
}

data::ModifyTableData* ModifyTableAction::getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query)
{
    return new data::ModifyTableData();
}

}
