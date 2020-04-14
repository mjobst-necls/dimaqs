/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef SERVICES_INCLUDED
#define SERVICES_INCLUDED

#include "my_global.h"
#include <mysql/service_parser.h>
#include <string>
#include <vector>

/**
  @file services.h

  Conversion layer between the parser service and this plugin. This plugin is
  written in C++, while the parser service is written in C.

  The layer handles:

  - Copying between server and plugin memory. This is necessary on some
    platforms (e.g. Windows) where dynamicly linked libraries have their own
    heap.

  - Wrapping raw const char * in std::string classes.
*/

namespace services
{

class Session
{
public:
  Session(MYSQL_THD current_session);

  MYSQL_THD thd() { return m_current_session; }

private:
  MYSQL_THD m_previous_session;
  MYSQL_THD m_current_session;
};

class Digest
{
  uchar m_buf[PARSER_SERVICE_DIGEST_LENGTH];

public:
  bool load(MYSQL_THD thd);

  const uchar *c_ptr() const { return m_buf; }
};

class Literal_visitor
{
public:
  virtual bool visit(MYSQL_ITEM item) = 0;
};

class Condition_handler
{
public:

  virtual bool handle(int sql_errno, const char* sqlstate, const char* message)
    = 0;

  virtual ~Condition_handler() = 0;
};

std::string print_digest(const uchar *digest);

void set_current_database(MYSQL_THD thd, const std::string &db);

bool parse(MYSQL_THD thd, const std::string &query, bool is_prepared,
           Condition_handler *handler);

bool parse(MYSQL_THD thd, const std::string &query, bool is_prepared);

bool is_select_statement(MYSQL_THD thd);

int get_number_params(MYSQL_THD thd);

bool visit_parse_tree(MYSQL_THD thd, Literal_visitor *visitor);

/// Prints an Item as an std::string.
std::string print_item(MYSQL_ITEM item);

std::string get_current_query_normalized(MYSQL_THD thd);

std::vector<int> get_parameter_positions(MYSQL_THD thd);
}

#endif // SERVICES_INCLUDED
