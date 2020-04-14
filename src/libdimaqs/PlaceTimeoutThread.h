/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef PLACETIMEOUTTHREAD_H
#define PLACETIMEOUTTHREAD_H

class DatabasePetriNet;

namespace std{
class thread;
class mutex;
}

class PlaceTimeoutThread
{
public:
    explicit PlaceTimeoutThread(DatabasePetriNet& net);
    virtual ~PlaceTimeoutThread();

private:
    void execute(PlaceTimeoutThread* ptr);

    void tokenTimeout(DatabasePetriNet& net);
private:
    DatabasePetriNet&   net_;
    std::thread*        thread_;

    bool                stop_;
    std::mutex*         stopmutex_;
};

#endif // PLACETIMEOUTTHREAD_H
