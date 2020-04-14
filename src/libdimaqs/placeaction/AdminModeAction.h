/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef ADMINMODEACTION_H
#define ADMINMODEACTION_H

#include "../../../lib/petrinet/src/PlaceAction.h"
#include "plugin.h"

namespace petrinet{
class TokenData;
}

class AdminHandler;

namespace placeaction{

class AdminModeAction : public petrinet::PlaceAction
{
public:
    enum ObjectType{
        Database,
        Table
    };

    explicit AdminModeAction(AdminHandler& adminHandler);
    AdminModeAction(const AdminModeAction& other);

    void execute();
    AdminModeAction* create(const petrinet::TokenData& tokenData);

    bool equals(const PlaceAction& other) const;

    const std::string& getVarname() const { return varname_; }
    void setVarname(const std::string& varname) { varname_ = varname; }

    const std::string& getVarvalue() const { return varvalue_; }
    void setVarvalue(const std::string& varvalue) { varvalue_ = varvalue; }

    void setTHD(MYSQL_THD thd) { thd_ = thd; }

private:
    AdminHandler& adminHandler_;
    MYSQL_THD       thd_;
    std::string     varname_;
    std::string     varvalue_;
};

}

#endif // ADMINMODEACTION_H
