/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "LiteralCollector.h"
#include "select_lex_visitor.h"
#include "services.h"
#include "sql_class.h"

class WhereCond_visitor: public Select_lex_visitor
{
  parse_node_visit_function m_processor;
  uchar *m_arg;

public:
  WhereCond_visitor(parse_node_visit_function processor, uchar *arg) :
    m_processor(processor),
    m_arg(arg)
  {}

protected:
  bool visit_item(Item *item)
  {
    switch(item->type())
    {
      // These are all the literals.
    case Item::PARAM_ITEM:
    case Item::STRING_ITEM:
    case Item::INT_ITEM:
    case Item::DECIMAL_ITEM:
    case Item::REAL_ITEM:
    case Item::NULL_ITEM:
    case Item::VARBIN_ITEM:
    case Item::FIELD_ITEM:

      return m_processor(item, m_arg);
    default:
      break;
    }
    return false;
  }
};

int process_item(MYSQL_ITEM item, uchar *arg)
{
  services::Literal_visitor *visitor= pointer_cast<services::Literal_visitor*>(arg);
  if (visitor->visit(item))
    return 1;
  return 0;
}

std::vector<CondValue> LiteralCollector::get_literals() {

    std::vector<CondValue> tmp_;
    for (auto it = std::begin(values_); it!=std::end(values_); ++it)
    {
        if( (*it).values.size() != 0)
            tmp_.push_back(*it);
    }

    return tmp_;
}

std::vector<CondValue> LiteralCollector::getLiterals(MYSQL_THD thd)
{
    LiteralCollector collector;

    uchar *arg= pointer_cast<uchar*>(&collector);

    WhereCond_visitor visitor(process_item, arg);
    thd->lex->accept(&visitor);

    return collector.get_literals();
}


