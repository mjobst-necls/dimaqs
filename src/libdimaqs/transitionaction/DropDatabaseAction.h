/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef DROPDATABASEACTION_H
#define DROPDATABASEACTION_H

#include "../../../lib/petrinet/src/TransitionAction.h"
#include "data/DropDatabaseData.h"

namespace petrinet{
class AbstractPetriNetQuery;
}

namespace transitionaction{

class DropDatabaseAction : public petrinet::TransitionAction
{
public:
    DropDatabaseAction();

    virtual bool isMatching(const petrinet::AbstractPetriNetQuery& query);
    data::DropDatabaseData* getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query);

};

}

#endif // DROPDATABASEACTION_H
