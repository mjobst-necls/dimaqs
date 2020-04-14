/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef ALWAYSDATA_H
#define ALWAYSDATA_H

#include "../../DatabaseTokenData.h"

namespace transitionaction{
namespace data{

class AlwaysData : public DatabaseTokenData
{
public:
    AlwaysData();
    AlwaysData(const AlwaysData& other);

    virtual bool isMergeable(const TokenData& other) const;
    virtual AlwaysData* clone() const;
};

}}

#endif // ALWAYSDATA_H
