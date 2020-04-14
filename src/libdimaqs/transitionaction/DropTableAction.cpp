/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "DropTableAction.h"
#include "../DatabasePetriNetQuery.h"
#include "sql_lex.h"
#include "Controller.h"

namespace transitionaction{

DropTableAction::DropTableAction()
{

}

bool DropTableAction::isMatching(const petrinet::AbstractPetriNetQuery& query)
{
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);
    return q.getActionType() == SQLCOM_DROP_TABLE && !q.getLex()->drop_temporary;
}

data::DropTableData* DropTableAction::getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query)
{
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);

    Controller::s_printMessageDirect("DB name:"  + std::string(q.getLex()->name.str, q.getLex()->name.length));

    data::DropTableData* data = new data::DropTableData();
    data->setTablename(q.getLex()->select_lex->get_table_list()->get_table_name());
    data->setDatabase(q.getLex()->select_lex->get_table_list()->get_db_name());
    return data;
}

}
