/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "ShowVariableData.h"

namespace transitionaction{
namespace data{

ShowVariableData::ShowVariableData()
{

}

ShowVariableData::ShowVariableData(const ShowVariableData& other)
    : DatabaseTokenData(other)/*,
      table_(other.table_)*/
{
}

bool ShowVariableData::isMergeable(const TokenData& other) const
{
    const ShowVariableData* other_data = static_cast<const ShowVariableData*>(&other);

    if( !DatabaseTokenData::isMergeable(other) )
        return false;

    return true;
}

ShowVariableData* ShowVariableData::clone() const
{
    return new ShowVariableData(*this);
}

}}
