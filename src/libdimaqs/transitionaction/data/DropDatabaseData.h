/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef DROPDATABASEDATA_H
#define DROPDATABASEDATA_H

#include "../../DatabaseTokenData.h"
#include "string"

namespace transitionaction{
namespace data{

class DropDatabaseData : public DatabaseTokenData
{
public:
    DropDatabaseData();
    DropDatabaseData(const DropDatabaseData& other);

    virtual bool isMergeable(const TokenData& other) const;
    virtual DropDatabaseData* clone() const;

    std::string getDatabase() const { return database_; }
    void setDatabase(const std::string& database) { database_ = database; }

private:
    std::string database_;
};

}}

#endif // DROPDATABASEDATA_H
