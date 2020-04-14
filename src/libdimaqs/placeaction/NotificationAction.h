/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef NOTIFICATIONACTION_H
#define NOTIFICATIONACTION_H

#include "../../../lib/petrinet/src/PlaceAction.h"
#include "string"

namespace petrinet{
class TokenData;
}

namespace placeaction{

class NotificationAction : public petrinet::PlaceAction
{
public:

    explicit NotificationAction(std::string& recipient);
    NotificationAction(const NotificationAction& other);

    bool equals(const PlaceAction& other) const;

    void execute();
    NotificationAction* create(const petrinet::TokenData& tokenData);

private:
    std::string recipient_;
};

}


#endif // NOTIFICATIONACTION_H
