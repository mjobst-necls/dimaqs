/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef DATABASETOKENDATA_H
#define DATABASETOKENDATA_H


#include "../../lib/petrinet/src/TokenData.h"

#include "string"

class DatabaseTokenData : public petrinet::TokenData
{
public:
    DatabaseTokenData();
    DatabaseTokenData(const DatabaseTokenData& other);

    virtual bool isMergeable(const TokenData& other) const;

    const std::string& getDatabase() const { return database_; }
    void setDatabase(const std::string& database) { database_ = database; }

    const std::string& getQueryType() const { return querytype_; }
    void setQueryType(const std::string& querytype) { querytype_ = querytype; }

private:
    std::string database_;
    std::string querytype_;

};

#endif // DATABASETOKENDATA_H
