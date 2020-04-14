/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef MODIFYTABLEACTION_H
#define MODIFYTABLEACTION_H

#include "../../../lib/petrinet/src/TransitionAction.h"
#include "data/ModifyTableData.h"

namespace petrinet{
class AbstractPetriNetQuery;
}

namespace transitionaction{

class ModifyTableAction : public petrinet::TransitionAction
{
public:
    ModifyTableAction();

    virtual bool isMatching(const petrinet::AbstractPetriNetQuery& query);
    data::ModifyTableData* getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query);

};

}

#endif // MODIFYTABLEACTION_H
