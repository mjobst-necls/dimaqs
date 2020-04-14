/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "RelatedObjectTransitionCondition.h"
#include "transitionaction/data/CreateTableData.h"
#include "transitionaction/data/InsertValueData.h"

#include "Controller.h"

RelatedObjectTransitionCondition::RelatedObjectTransitionCondition(ValueComparision valueComparision)
    : valueComparision_(valueComparision)
{
    relatedTransition_ = 0;
}

bool RelatedObjectTransitionCondition::isFullfilled(const petrinet::TokenData& tokenDataExisting, const petrinet::TokenData& tokenDataNew) const
{
    bool retVal = false;

    const transitionaction::data::CreateTableData& createTableData = dynamic_cast<const transitionaction::data::CreateTableData&>(tokenDataExisting);
    const transitionaction::data::InsertValueData& insertValueData = dynamic_cast<const transitionaction::data::InsertValueData&>(tokenDataNew);

    switch( valueComparision_ ){
    case DbName:
    {
        break;
    }
    case TableName:
        if( createTableData.getTablename() == insertValueData.getTablename() )
            retVal = true;
        break;
    }

    return retVal;
}
