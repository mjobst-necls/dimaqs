/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef ALWAYSACTION_H
#define ALWAYSACTION_H

#include "../../../lib/petrinet/src/TransitionAction.h"
#include "data/AlwaysData.h"

namespace petrinet{
class AbstractPetriNetQuery;
}

namespace transitionaction{

class AlwaysAction : public petrinet::TransitionAction
{
public:
    AlwaysAction();

    virtual bool isMatching(const petrinet::AbstractPetriNetQuery& query);
    virtual data::AlwaysData* getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query);

};

}

#endif // ALWAYSACTION_H
