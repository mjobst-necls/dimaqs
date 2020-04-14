/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "CreateTableData.h"

namespace transitionaction{
namespace data{

CreateTableData::CreateTableData()
{

}

CreateTableData::CreateTableData(const CreateTableData& other)
    : DatabaseTokenData(other),
      tablename_(other.tablename_)
{

}

bool CreateTableData::isMergeable(const TokenData& other) const
{

    const CreateTableData* other_data = static_cast<const CreateTableData*>(&other);

    if(     other_data!= NULL
        &&  other_data->getTablename().compare(getTablename()) == 0){
        return true;
    }

    return false;
}

CreateTableData* CreateTableData::clone() const
{
    return new CreateTableData(*this);
}

}}
