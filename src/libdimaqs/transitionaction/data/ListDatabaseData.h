/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef LISTDATABASEDATA_H
#define LISTDATABASEDATA_H

#include "../../DatabaseTokenData.h"
#include "../../LiteralCollector.h"

namespace transitionaction{
namespace data{

class ListDatabaseData : public DatabaseTokenData
{
public:
    ListDatabaseData();
    ListDatabaseData(const ListDatabaseData& other);

    virtual bool isMergeable(const TokenData& other) const;
    virtual ListDatabaseData* clone() const;

    const std::string& getTable() const { return table_; }
    void setTable(const std::string& table) { table_ = table; }

private:
    std::string table_;
};

}}

#endif // LISTDATABASEDATA_H
