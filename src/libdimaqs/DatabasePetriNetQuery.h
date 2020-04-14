/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef DATABASEPETRINETQUERY_H
#define DATABASEPETRINETQUERY_H

#include "../../lib/petrinet/src/AbstractPetriNetQuery.h"
#include "my_sqlcommand.h"
#include "protocol.h"
#include "vector"
struct LEX;

class DatabasePetriNetQuery : public petrinet::AbstractPetriNetQuery
{

public:
    DatabasePetriNetQuery();


    void setLex(LEX* lex) { lex_ = lex; }
    void setQuery(std::string& query) { query_ = query; }

    virtual enum_sql_command getActionType() const;
    virtual std::string getQuery() const;
    virtual std::string getQueryType() const;

    void setInsertValues(const std::string& dbname, const std::string& tablename, const std::vector<std::string>& values);
    const std::vector<std::string>& getInsertedValues() const { return inserted_values_; }

    const std::string& getDatabaseName() const { return dbname_; }
    const std::string& getTableName() const { return tablename_; }

    LEX* getLex() const { return lex_; }



    MYSQL_THD   getThd() const { return thd_; }
    void setThd(MYSQL_THD thd) { thd_ = thd; }

private:
    LEX*        lex_;
    std::string query_;
    MYSQL_THD   thd_;

    std::string dbname_;
    std::string tablename_;
    std::vector<std::string> inserted_values_;
};

#endif // DATABASEPETRINETQUERY_H
