/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef CREATETABLEACTION_H
#define CREATETABLEACTION_H

#include "../../../lib/petrinet/src/TransitionAction.h"
#include "data/CreateTableData.h"

#include "list"

namespace petrinet{
class AbstractPetriNetQuery;
}

namespace transitionaction{

class CreateTableAction : public petrinet::TransitionAction
{
public:
    CreateTableAction(std::list<std::string>& malicious_table_names);

    virtual bool isMatching(const petrinet::AbstractPetriNetQuery& query);
    data::CreateTableData* getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query);

private:
    std::list<std::string> malicious_table_names_;
};

}


#endif // CREATETABLEACTION_H
