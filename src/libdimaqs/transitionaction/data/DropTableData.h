/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef DROPTABLEDATA_H
#define DROPTABLEDATA_H

#include "../../DatabaseTokenData.h"

#include "string"

namespace transitionaction{
namespace data{

class DropTableData : public DatabaseTokenData
{
public:
    DropTableData();
    DropTableData(const DropTableData& other);

    virtual bool isMergeable(const TokenData& other) const;
    virtual DropTableData* clone() const;

    std::string getTablename() const { return tablename_; }
    void setTablename(const std::string& tablename) { tablename_ = tablename; }

    std::string getDatabase() const { return database_; }
    void setDatabase(const std::string& database) { database_ = database; }


private:
    std::string tablename_;
    std::string database_;
};

}}

#endif // DROPTABLEDATA_H
