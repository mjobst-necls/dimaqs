/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "InsertAction.h"
#include "../DatabasePetriNetQuery.h"

#include "regex"

#include "Controller.h"

namespace transitionaction{

InsertAction::InsertAction(std::list<std::string>& malicious_values)
    : malicious_values_(malicious_values)
{
}

bool InsertAction::isMatching(const petrinet::AbstractPetriNetQuery& query)
{
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);

    if( q.getActionType() == SQLCOM_INSERT || q.getActionType() == SQLCOM_INSERT_SELECT ){

        if( malicious_values_.size() == 0)
            return true;

        for (auto it = std::begin(malicious_values_); it!=std::end(malicious_values_); ++it)
        {
            std::regex regx((*it).c_str(), std::regex_constants::icase | std::regex_constants::ECMAScript);

            const std::vector<std::string>& insertedValues = q.getInsertedValues();

            for (auto itt = std::begin(insertedValues); itt!=std::end(insertedValues); ++itt)
            {
                if (std::regex_search ((*itt).c_str(),regx)){
                    return true;
                }
            }
        }
    }

    return false;
}

data::InsertValueData* InsertAction::getTransitionTokenData(const petrinet::AbstractPetriNetQuery& query)
{
    const DatabasePetriNetQuery& q = static_cast<const DatabasePetriNetQuery&>(query);

    data::InsertValueData* data = new data::InsertValueData();

    data->setTablename(q.getTableName());
    data->setDatabase(q.getDatabaseName());
    data->setValues(q.getInsertedValues());

    return data;
}

}
