/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef MODIFYTABLEDATA_H
#define MODIFYTABLEDATA_H

#include "../../DatabaseTokenData.h"

namespace transitionaction{
namespace data{

class ModifyTableData : public DatabaseTokenData
{
public:
    ModifyTableData();
    ModifyTableData(const ModifyTableData& other);

    virtual bool isMergeable(const TokenData& other) const;
    virtual ModifyTableData* clone() const;
};

}}

#endif // MODIFYTABLEDATA_H
