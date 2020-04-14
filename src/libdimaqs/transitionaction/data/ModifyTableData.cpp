/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "ModifyTableData.h"

namespace transitionaction{
namespace data{

ModifyTableData::ModifyTableData()
{

}

ModifyTableData::ModifyTableData(const ModifyTableData& other)
    : DatabaseTokenData(other)
{
}

bool ModifyTableData::isMergeable(const TokenData& other) const
{
    return static_cast<const ModifyTableData*>(&other) != NULL;
}

ModifyTableData* ModifyTableData::clone() const
{
    return new ModifyTableData(*this);
}

}}
