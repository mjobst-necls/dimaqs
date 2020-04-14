/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "DropDatabaseData.h"

namespace transitionaction{
namespace data{

DropDatabaseData::DropDatabaseData()
{

}

DropDatabaseData::DropDatabaseData(const DropDatabaseData& other)
    : DatabaseTokenData(other)
{

}

bool DropDatabaseData::isMergeable(const TokenData& other) const
{
    return static_cast<const DropDatabaseData*>(&other) != NULL;
}

DropDatabaseData* DropDatabaseData::clone() const
{
    return new DropDatabaseData(*this);
}

}}
