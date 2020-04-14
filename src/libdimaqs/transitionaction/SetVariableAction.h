/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef SETVARIABLEACTION_H
#define SETVARIABLEACTION_H

#include "../../../lib/petrinet/src/TransitionAction.h"
#include "data/SetVariableData.h"

namespace petrinet{
class AbstractPetriNetQuery;
}

namespace transitionaction{

class SetVariableAction : public petrinet::TransitionAction
{
public:
    SetVariableAction();

    virtual bool isMatching(const petrinet::AbstractPetriNetQuery& query);
    data::SetVariableData* getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query);

};
}


#endif // SETVARIABLEACTION_H
