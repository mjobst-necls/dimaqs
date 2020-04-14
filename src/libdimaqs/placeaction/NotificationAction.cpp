/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "NotificationAction.h"
#include "Controller.h"

namespace placeaction{

NotificationAction::NotificationAction(std::string& recipient)
    : recipient_(recipient)
{
    setName("NotificationAction");
}

NotificationAction::NotificationAction(const NotificationAction& other)
    : petrinet::PlaceAction(other)
{
}

void NotificationAction::execute()
{
    Controller::s_printMessageDirect("NotificationAction::execute");
    std::string command = "sendemail -f dmaqs@dmaqs -t ";
    command += recipient_;
    command += " -u \"DIMAQS: Incident appeared\" -m \"Please check your MySQL server\"";
    system(command.c_str());
}

NotificationAction* NotificationAction::create(const petrinet::TokenData& tokenData)
{
    NotificationAction* action = new NotificationAction(recipient_);
    return action;
}

bool NotificationAction::equals(const PlaceAction& other) const
{
    const NotificationAction* other_ = dynamic_cast<const NotificationAction*>(&other);

    if( !other_ )
        return false;

    return true;
}

}
