/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "SetVariableAction.h"
#include "../DatabasePetriNetQuery.h"
#include "sql_lex.h"
#include "Controller.h"

namespace transitionaction{

SetVariableAction::SetVariableAction()
{

}

bool SetVariableAction::isMatching(const petrinet::AbstractPetriNetQuery& query)
{
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);

    if( q.getActionType() != SQLCOM_SET_OPTION)
        return false;

    List<set_var_base>*  var_list = &q.getLex()->var_list;
    List_iterator_fast<set_var_base> it(*var_list);
    set_var_base *var;
    set_var*   setvar;
    while ((var=it++))
    {
        if( (setvar = dynamic_cast<set_var*>(var)) != NULL){
            std::string varname = std::string(setvar->var->name.str, setvar->var->name.length);

            if( varname == "dmaqs_secret")
                return true;
        }

    }

    return false;
}

data::SetVariableData* SetVariableAction::getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query)
{
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);

    data::SetVariableData* data = new data::SetVariableData();

    List<set_var_base>*  var_list = &q.getLex()->var_list;
    List_iterator_fast<set_var_base> it(*var_list);
    set_var_base *var;
    set_var*   setvar;
    while ((var=it++))
    {
        if( (setvar = dynamic_cast<set_var*>(var)) != NULL){
            std::string varname = std::string(setvar->var->name.str, setvar->var->name.length);
            std::string varvalue = std::string(setvar->value->str_value.c_ptr(), setvar->value->str_value.length());

            data->setVarname(varname);
            data->setVarvalue(varvalue);
            break;
        }

    }

    return data;
}

}
