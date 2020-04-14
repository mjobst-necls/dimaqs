/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef LISTCOLUMNDATA_H
#define LISTCOLUMNDATA_H

#include "../../DatabaseTokenData.h"

namespace transitionaction{
namespace data{

class ListColumnData : public DatabaseTokenData
{
public:
    ListColumnData();
    ListColumnData(const ListColumnData& other);

    virtual bool isMergeable(const TokenData& other) const;
    virtual ListColumnData* clone() const;

    const std::string& getTable() const { return table_; }
    void setTable(const std::string& table) { table_ = table; }

private:
    std::string table_;
};

}}

#endif // LISTCOLUMNDATA_H
