/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef ADMINHANDLER_H
#define ADMINHANDLER_H

#include "sql_base.h"
#include "mutex"

class AdminHandler
{
public:
    struct ConnectionInformation{
        MYSQL_THD   thd;
        bool        isAdmin;
    };

public:
    AdminHandler(const std::string& secret);
    ~AdminHandler();

    void setIsAdmin(MYSQL_THD thd, bool isAdmin);
    bool getIsAdmin(MYSQL_THD thd) const;

    void addConnection(MYSQL_THD thd);
    void removeConnection(MYSQL_THD thd);

    const std::string& getSecret() const { return secret_; }

    bool checkSecret(const std::string& secret) const;

private:

    std::map<MYSQL_THD, ConnectionInformation>* list_;
    std::mutex  mutex_;

    const std::string& secret_;

};

#endif // ADMINHANDLER_H
