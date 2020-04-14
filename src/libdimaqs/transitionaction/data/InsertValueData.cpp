/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "InsertValueData.h"

namespace transitionaction{
namespace data{

InsertValueData::InsertValueData()
{

}

InsertValueData::InsertValueData(const InsertValueData& other)
    : DatabaseTokenData(other)
{
}

bool InsertValueData::isMergeable(const TokenData& other) const
{
    return static_cast<const InsertValueData*>(&other) != NULL;
}

InsertValueData* InsertValueData::clone() const
{
    return new InsertValueData(*this);
}

}}
