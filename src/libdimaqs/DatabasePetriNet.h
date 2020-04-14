/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef DATABASEPETRINET_H
#define DATABASEPETRINET_H

#include "../../lib/petrinet/src/PetriNet.h"

namespace std{
class mutex;
}

namespace petrinet{
class AbstractPetriNetQuery;
class PlaceAction;
}

class DatabasePetriNetData;
class TransitionHelper;
class PlaceTimeoutThread;

class DatabasePetriNet : public petrinet::PetriNet
{
public:
    DatabasePetriNet();
    ~DatabasePetriNet();

    std::list<petrinet::PlaceAction*> handle(const petrinet::AbstractPetriNetQuery& query);

    std::list<petrinet::Place*> getActivePlaces() const;
    std::list<petrinet::Transition*> getActiveTransitions() const;

    void tokenTimeout();

private:
    DatabasePetriNetData*               data_;

    std::mutex*                         mutex_;
    PlaceTimeoutThread*                 timeoutThread_;

};

#endif // DATABASEPETRINET_H
