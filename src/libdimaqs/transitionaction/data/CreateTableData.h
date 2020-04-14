/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef CREATETABLEDATA_H
#define CREATETABLEDATA_H

#include "../../DatabaseTokenData.h"
#include "string"

namespace transitionaction{
namespace data{

class CreateTableData : public DatabaseTokenData
{
public:
    CreateTableData();
    CreateTableData(const CreateTableData& other);

    virtual bool isMergeable(const TokenData& other) const;
    virtual CreateTableData* clone() const;

    std::string getTablename() const { return tablename_; }
    void setTablename(const std::string& tablename) { tablename_ = tablename; }

private:
    std::string tablename_;
};

}}
#endif // CREATETABLEDATA_H
