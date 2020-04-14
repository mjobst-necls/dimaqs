/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "ListColumnData.h"

namespace transitionaction{
namespace data{

ListColumnData::ListColumnData()
{

}

ListColumnData::ListColumnData(const ListColumnData& other)
    : DatabaseTokenData(other),
      table_(other.table_)
{
}

bool ListColumnData::isMergeable(const TokenData& other) const
{
    const ListColumnData* other_data = static_cast<const ListColumnData*>(&other);

    if( !DatabaseTokenData::isMergeable(other) )
        return false;

    if( table_ != other_data->table_ )
        return false;

    return true;
}

ListColumnData* ListColumnData::clone() const
{
    return new ListColumnData(*this);
}

}}
