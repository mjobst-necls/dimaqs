/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef REWRITEQUERYACTION_H
#define REWRITEQUERYACTION_H

#include "../../../lib/petrinet/src/PlaceAction.h"
#include "sql_class.h"

class QueryRewriter;
class DatabasePetriNetQuery;
class DatabaseTokenData;

namespace petrinet{
class TokenData;
}

namespace placeaction{

class RewriteQueryAction : public petrinet::PlaceAction
{
public:
    enum QueryType{
        Database,
        Table,
        Column,
        Variable
    };

public:
    explicit RewriteQueryAction(QueryRewriter& rewriter, QueryType queryType);
    RewriteQueryAction(const RewriteQueryAction& other);

    void execute();

    RewriteQueryAction* create(const petrinet::TokenData& tokenData);

    bool equals(const PlaceAction& other) const;

    void setQueryType(QueryType queryType) { queryType_ = queryType; }
    void setTokenData(const DatabaseTokenData* tokenData);
    void setTHD(MYSQL_THD thd) { thd_ = thd; }
    void setQuery(const DatabasePetriNetQuery* query) { query_ = query; }

private:
    QueryRewriter&              rewriter_;
    QueryType                   queryType_;
    const DatabasePetriNetQuery*      query_;
    MYSQL_THD                   thd_;
};

}

#endif // REWRITEQUERYACTION_H
