/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "ListTableData.h"

namespace transitionaction{
namespace data{

ListTableData::ListTableData()
{

}

ListTableData::ListTableData(const ListTableData& other)
    : DatabaseTokenData(other),
    table_(other.table_)
{
}

bool ListTableData::isMergeable(const TokenData& other) const
{
    const ListTableData* other_data = static_cast<const ListTableData*>(&other);

    if( !DatabaseTokenData::isMergeable(other) )
        return false;

    if( table_ != other_data->table_ )
        return false;

    return true;
}

ListTableData* ListTableData::clone() const
{
    return new ListTableData(*this);
}

}}
