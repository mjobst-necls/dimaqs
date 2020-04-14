/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "AlwaysData.h"

namespace transitionaction{
namespace data{

AlwaysData::AlwaysData()
{

}

AlwaysData::AlwaysData(const AlwaysData& other)
    : DatabaseTokenData(other)
{

}

bool AlwaysData::isMergeable(const TokenData& other) const
{
    return static_cast<const AlwaysData*>(&other) != NULL;
}

AlwaysData* AlwaysData::clone() const
{
    return new AlwaysData(*this);
}

}}
