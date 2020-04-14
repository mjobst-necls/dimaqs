/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "DropTableData.h"

namespace transitionaction{
namespace data{

DropTableData::DropTableData()
{
    tablename_ = "";
    database_ = "";
}

DropTableData::DropTableData(const DropTableData& other)
    : DatabaseTokenData(other),
      tablename_(other.tablename_),
      database_(other.database_)
{
}

bool DropTableData::isMergeable(const TokenData& other) const
{
    return static_cast<const DropTableData*>(&other) != NULL;
}

DropTableData* DropTableData::clone() const
{
    return new DropTableData(*this);
}

}}
