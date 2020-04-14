/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef LISTTABLEDATA_H
#define LISTTABLEDATA_H

#include "../../DatabaseTokenData.h"

namespace transitionaction{
namespace data{

class ListTableData : public DatabaseTokenData
{
public:
    ListTableData();
    ListTableData(const ListTableData& other);

    virtual bool isMergeable(const TokenData& other) const;
    virtual ListTableData* clone() const;

    const std::string& getTable() const { return table_; }
    void setTable(const std::string& table) { table_ = table; }

private:
    std::string table_;
};

}}

#endif // LISTTABLEDATA_H
