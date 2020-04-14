/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef LITERALCOLLECTOR_H
#define LITERALCOLLECTOR_H

#include "services.h"
#include "item.h"
#include "list"

struct CondValue{
    std::string conditionName;
    std::vector<std::string> values;
};

class LiteralCollector : public services::Literal_visitor
{
public:

  bool visit(MYSQL_ITEM item)
  {
      switch(item->type())
      {
      case Item::FIELD_ITEM:
      {
          CondValue val;
          val.conditionName = item->full_name();
          values_.push_back(val);
          break;
      }
      case Item::PARAM_ITEM:
      case Item::STRING_ITEM:
      case Item::INT_ITEM:
      case Item::DECIMAL_ITEM:
      case Item::REAL_ITEM:
      case Item::NULL_ITEM:
      case Item::VARBIN_ITEM:
      {
          values_.back().values.push_back(item->full_name());
          break;
      }
      default:
        break;
      }
      return false;
    }

  std::vector<CondValue> get_literals();


  static std::vector<CondValue> getLiterals(MYSQL_THD thd);

private:
  std::vector<CondValue> values_;
};

#endif // LITERALCOLLECTOR_H
