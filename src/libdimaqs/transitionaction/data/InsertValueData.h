/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef INSERTVALUEACTION_H
#define INSERTVALUEACTION_H

#include "../../DatabaseTokenData.h"
#include "string"
#include "vector"

namespace transitionaction{
namespace data{

class InsertValueData : public DatabaseTokenData
{
public:
    InsertValueData();
    InsertValueData(const InsertValueData& other);

    virtual bool isMergeable(const TokenData& other) const;
    virtual InsertValueData* clone() const;

    std::string getTablename() const { return tablename_; }
    void setTablename(const std::string& tablename) { tablename_ = tablename; }

    const std::vector<std::string>& getValues() const { return values_; }
    void setValues(const std::vector<std::string>& values) { values_ = values; }

private:
    std::string tablename_;
    std::vector<std::string>    values_;
};

}}

#endif // INSERTVALUEACTION_H
