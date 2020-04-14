/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "AdminHandler.h"

AdminHandler::AdminHandler(const std::string& secret)
    : secret_(secret)
{
    list_ = new std::map<MYSQL_THD, ConnectionInformation>();
}

AdminHandler::~AdminHandler()
{
    list_->clear();
    delete list_;
}

void AdminHandler::setIsAdmin(MYSQL_THD thd, bool isAdmin)
{
    mutex_.lock();
    list_->at(thd).isAdmin = isAdmin;
    mutex_.unlock();
}

bool AdminHandler::getIsAdmin(MYSQL_THD thd) const{
    bool isAdmin;
    const_cast<std::mutex&>(mutex_).lock();
    isAdmin = list_->at(thd).isAdmin;
    const_cast<std::mutex&>(mutex_).unlock();

    return isAdmin;
}

void AdminHandler::addConnection(MYSQL_THD thd)
{
    mutex_.lock();
    ConnectionInformation info;
    info.isAdmin = false;
    info.thd = thd;
    list_->insert(std::pair<MYSQL_THD, ConnectionInformation>(thd, info));
    mutex_.unlock();
}

void AdminHandler::removeConnection(MYSQL_THD thd){
    mutex_.lock();
    list_->erase(thd);
    mutex_.unlock();
}

bool AdminHandler::checkSecret(const std::string& secret) const
{
    return secret_ == secret;
}
