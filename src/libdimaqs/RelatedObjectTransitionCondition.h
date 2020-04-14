/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef RELATEDOBJECTTRANSITIONCONDITION_H
#define RELATEDOBJECTTRANSITIONCONDITION_H

#include "../../lib/petrinet/src/AbstractTransitionCondition.h"
#include "ValueComparision.h"

namespace petrinet{
class TokenData;
}

class RelatedObjectTransitionCondition : public petrinet::AbstractTransitionCondition
{
public:
    explicit RelatedObjectTransitionCondition(ValueComparision valueComparision);

    bool isFullfilled(const petrinet::TokenData& tokenDataExisting, const petrinet::TokenData& tokenDataNew) const;

private:
    ValueComparision valueComparision_;
};

#endif // RELATEDOBJECTTRANSITIONCONDITION_H
