/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef SHOWVARIABLEDATA_H
#define SHOWVARIABLEDATA_H

#include "../../DatabaseTokenData.h"

namespace transitionaction{
namespace data{

class ShowVariableData : public DatabaseTokenData
{
public:
    ShowVariableData();
    ShowVariableData(const ShowVariableData& other);

    virtual bool isMergeable(const TokenData& other) const;
    virtual ShowVariableData* clone() const;
};

}}

#endif // SHOWVARIABLEDATA_H
