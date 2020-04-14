/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef DROPTABLEACTION_H
#define DROPTABLEACTION_H

#include "../../../lib/petrinet/src/TransitionAction.h"
#include "data/DropTableData.h"

namespace petrinet{
class AbstractPetriNetQuery;
}

namespace transitionaction{

class DropTableAction : public petrinet::TransitionAction
{
public:
    DropTableAction();

    virtual bool isMatching(const petrinet::AbstractPetriNetQuery& query);
    data::DropTableData* getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query);

};

}

#endif // DROPTABLEACTION_H
