/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef LISTTABLEACTION_H
#define LISTTABLEACTION_H

#include "../../../lib/petrinet/src/TransitionAction.h"
#include "data/ListTableData.h"

namespace petrinet{
class AbstractPetriNetQuery;
}

namespace transitionaction{

class ListTableAction : public petrinet::TransitionAction
{
public:
    ListTableAction();

    virtual bool isMatching(const petrinet::AbstractPetriNetQuery& query);
    data::ListTableData* getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query);

};

}

#endif // LISTTABLEACTION_H
