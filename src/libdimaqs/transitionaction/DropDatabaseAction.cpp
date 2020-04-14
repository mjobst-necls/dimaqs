/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "DropDatabaseAction.h"
#include "../DatabasePetriNetQuery.h"
#include "sql_lex.h"
#include "Controller.h"

namespace transitionaction{

DropDatabaseAction::DropDatabaseAction()
{

}

bool DropDatabaseAction::isMatching(const petrinet::AbstractPetriNetQuery& query)
{
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);
    return q.getActionType() == SQLCOM_DROP_DB;
}

data::DropDatabaseData* DropDatabaseAction::getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query)
{
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);

    data::DropDatabaseData* data = new data::DropDatabaseData();
    data->setDatabase(std::string(q.getLex()->name.str, q.getLex()->name.length));
    return data;
}

}
