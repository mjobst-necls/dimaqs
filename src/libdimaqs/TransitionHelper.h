/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef TRANSITIONHELPER_H
#define TRANSITIONHELPER_H

#include "../../lib/petrinet/src/AbstractTransitionHelper.h"

class TransitionHelper : public petrinet::AbstractTransitionHelper
{
public:
    TransitionHelper();

    bool isMatching(const petrinet::Transition& transition, const petrinet::AbstractPetriNetQuery& query) const;
    std::list<petrinet::PlaceAction*> trigger(petrinet::Transition& transition, const petrinet::AbstractPetriNetQuery& query) const;
};

#endif // TRANSITIONHELPER_H
