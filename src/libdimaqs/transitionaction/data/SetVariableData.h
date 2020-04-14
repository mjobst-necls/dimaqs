/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef SETVARIABLEDATA_H
#define SETVARIABLEDATA_H

#include "../../DatabaseTokenData.h"

namespace transitionaction{
namespace data{

class SetVariableData : public DatabaseTokenData
{
public:
    SetVariableData();
    SetVariableData(const SetVariableData& other);

    virtual bool isMergeable(const TokenData& other) const;
    virtual SetVariableData* clone() const;

    const std::string& getVarname() const { return varname_; }
    void setVarname(const std::string& varname) { varname_ = varname; }

    const std::string& getVarvalue() const { return varvalue_; }
    void setVarvalue(const std::string& varvalue) { varvalue_ = varvalue; }

private:

    std::string     varname_;
    std::string     varvalue_;
};

}}

#endif // SETVARIABLEDATA_H
