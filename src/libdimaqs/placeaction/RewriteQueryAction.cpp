/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "RewriteQueryAction.h"
#include "transitionaction/data/ListDatabaseData.h"
#include "transitionaction/data/ListTableData.h"
#include "transitionaction/data/ListColumnData.h"
#include "QueryRewriter.h"
#include "Controller.h"

namespace placeaction{

RewriteQueryAction::RewriteQueryAction(QueryRewriter& rewriter, QueryType queryType)
    : petrinet::PlaceAction(),
      rewriter_(rewriter),
      queryType_(queryType),
      query_(NULL),
      thd_(NULL)
{

}

RewriteQueryAction::RewriteQueryAction(const RewriteQueryAction& other)
    : petrinet::PlaceAction(other),
      rewriter_(other.rewriter_),
      queryType_(other.queryType_),
      query_(other.query_),
      thd_(other.thd_)
{

}

void RewriteQueryAction::execute()
{
    if( Controller::getVerbosity() >= 5 ){
        Controller::s_printMessage("RewriteQueryAction::execute");
    }
    rewriter_.rewrite(thd_, queryType_, *query_);
}

RewriteQueryAction* RewriteQueryAction::create(const petrinet::TokenData& tokenData)
{
    return new RewriteQueryAction(rewriter_, queryType_);
}

bool RewriteQueryAction::equals(const PlaceAction& other) const
{
    return true;
}

}
