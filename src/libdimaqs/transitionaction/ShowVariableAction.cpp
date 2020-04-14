/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "ShowVariableAction.h"
#include "../DatabasePetriNetQuery.h"
#include "sql_lex.h"
#include "Controller.h"

namespace transitionaction{

ShowVariableAction::ShowVariableAction()
{

}

bool ShowVariableAction::isMatching(const petrinet::AbstractPetriNetQuery& query)
{
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);

    return q.getActionType() == SQLCOM_SHOW_VARIABLES;
}

data::ShowVariableData* ShowVariableAction::getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query)
{
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);

    data::ShowVariableData* data = new data::ShowVariableData();
    return data;
}

}
