/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "ListDatabaseData.h"
#include "regex"

namespace transitionaction{
namespace data{

ListDatabaseData::ListDatabaseData()
{

}

ListDatabaseData::ListDatabaseData(const ListDatabaseData& other)
    : DatabaseTokenData(other),
      table_(other.table_)
{
}

bool ListDatabaseData::isMergeable(const TokenData& other) const
{
    const ListDatabaseData* other_data = static_cast<const ListDatabaseData*>(&other);

    if( !DatabaseTokenData::isMergeable(other) )
        return false;

    if( table_ != other_data->table_ )
        return false;

    return true;
}

ListDatabaseData* ListDatabaseData::clone() const
{
    return new ListDatabaseData(*this);
}

}}
