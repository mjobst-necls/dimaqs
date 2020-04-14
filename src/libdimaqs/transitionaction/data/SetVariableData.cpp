/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "SetVariableData.h"

namespace transitionaction{
namespace data{

SetVariableData::SetVariableData()
{

}

SetVariableData::SetVariableData(const SetVariableData& other)
    : DatabaseTokenData(other)/*,
      table_(other.table_)*/
{
}

bool SetVariableData::isMergeable(const TokenData& other) const
{
    const SetVariableData* other_data = static_cast<const SetVariableData*>(&other);

    if( !DatabaseTokenData::isMergeable(other) )
        return false;

    return true;
}

SetVariableData* SetVariableData::clone() const
{
    return new SetVariableData(*this);
}

}}
