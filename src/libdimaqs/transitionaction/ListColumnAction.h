/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef LISTCOLUMNACTION_H
#define LISTCOLUMNACTION_H

#include "../../../lib/petrinet/src/TransitionAction.h"
#include "data/ListColumnData.h"

namespace petrinet{
class AbstractPetriNetQuery;
}

namespace transitionaction{

class ListColumnAction : public petrinet::TransitionAction
{
public:
    ListColumnAction();

    virtual bool isMatching(const petrinet::AbstractPetriNetQuery& query);
    data::ListColumnData* getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query);

};

}

#endif // LISTCOLUMNACTION_H
