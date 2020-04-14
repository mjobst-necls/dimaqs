/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef SHOWVARIABLEACTION_H
#define SHOWVARIABLEACTION_H

#include "../../../lib/petrinet/src/TransitionAction.h"
#include "data/ShowVariableData.h"

namespace petrinet{
class AbstractPetriNetQuery;
}

namespace transitionaction{

class ShowVariableAction : public petrinet::TransitionAction
{
public:
    ShowVariableAction();

    virtual bool isMatching(const petrinet::AbstractPetriNetQuery& query);
    data::ShowVariableData* getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query);
};

}

#endif // SHOWVARIABLEACTION_H
