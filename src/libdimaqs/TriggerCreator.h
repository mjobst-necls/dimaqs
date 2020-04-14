/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef TRIGGERCREATOR_H
#define TRIGGERCREATOR_H

#include "plugin.h"
#include "vector"

struct TABLE;

class TriggerCreator
{
public:
    explicit TriggerCreator(const std::vector<std::string>& excluded_dbs, const std::string& object_prefix);
    ~TriggerCreator();

    void createTriggers(MYSQL_THD thd, bool& triggersCreated);
    void createTriggersInThread(MYSQL_THD thd);

    void createTriggersForTable(MYSQL_THD thd, const std::string& dbName, const std::string& tableName);

    void do_createTriggers(MYSQL_THD thd, const std::string& dbName, const std::string& tableName);
    void do_createSingleTrigger(MYSQL_THD thd, const std::string& dbName, const std::string& tableName);

    static int field_index(TABLE *table, const char *field_name);

private:
    const std::vector<std::string>&          excluded_dbs_;
    const std::string&                       object_prefix_;
};

#endif // TRIGGERCREATOR_H
