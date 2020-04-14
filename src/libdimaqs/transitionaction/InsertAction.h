/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef INSERTACTION_H
#define INSERTACTION_H

#include "../../../lib/petrinet/src/TransitionAction.h"
#include "data/InsertValueData.h"

#include "list"

namespace petrinet{
class AbstractPetriNetQuery;
}

namespace transitionaction{

class InsertAction : public petrinet::TransitionAction
{
public:
    InsertAction(std::list<std::string>& malicious_values);

    virtual bool isMatching(const petrinet::AbstractPetriNetQuery& query);
    data::InsertValueData* getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query);

private:
    std::list<std::string> malicious_values_;

};

}

#endif // INSERTACTION_H
