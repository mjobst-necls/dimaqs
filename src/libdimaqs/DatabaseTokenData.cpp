/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "DatabaseTokenData.h"

DatabaseTokenData::DatabaseTokenData()
{

}

DatabaseTokenData::DatabaseTokenData(const DatabaseTokenData& other)
    : petrinet::TokenData(other),
      database_(other.database_),
      querytype_(other.querytype_)
{

}

bool DatabaseTokenData::isMergeable(const TokenData& other) const
{
    const DatabaseTokenData* other_data = static_cast<const DatabaseTokenData*>(&other);

    if( database_ != other_data->database_)
        return false;

    if( querytype_ != other_data->querytype_)
        return false;

    return true;
}
