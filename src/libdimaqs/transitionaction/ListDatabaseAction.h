/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef LISTDATABASEACTION_H
#define LISTDATABASEACTION_H

#include "../../../lib/petrinet/src/TransitionAction.h"
#include "data/ListDatabaseData.h"

namespace petrinet{
class AbstractPetriNetQuery;
}

namespace transitionaction{

class ListDatabaseAction : public petrinet::TransitionAction
{
public:
    ListDatabaseAction();

    virtual bool isMatching(const petrinet::AbstractPetriNetQuery& query);
    data::ListDatabaseData* getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query);

};

}

#endif // LISTDATABASEACTION_H
