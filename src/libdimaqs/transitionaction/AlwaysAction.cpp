/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "AlwaysAction.h"
#include "../DatabasePetriNetQuery.h"

namespace transitionaction{

AlwaysAction::AlwaysAction()
{

}

bool AlwaysAction::isMatching(const petrinet::AbstractPetriNetQuery& query)
{
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);
    return q.getActionType() == SQLCOM_END; // always
}

data::AlwaysData* AlwaysAction::getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query)
{
    return new data::AlwaysData();
}

}
