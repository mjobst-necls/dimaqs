/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "PlaceTimeoutThread.h"
#include "DatabasePetriNet.h"
#include <thread>
#include <mutex>
#include <unistd.h>

PlaceTimeoutThread::PlaceTimeoutThread(DatabasePetriNet& net)
    : net_(net),
      stop_(false)
{
    stopmutex_ = new std::mutex();
    thread_ = new std::thread(&PlaceTimeoutThread::execute, this, this);
}

PlaceTimeoutThread::~PlaceTimeoutThread()
{
    stopmutex_->lock();
    stop_ = true;
    stopmutex_->unlock();

    thread_->join();

    delete thread_;
    delete stopmutex_;
}


void PlaceTimeoutThread::execute(PlaceTimeoutThread* ptr)
{
    bool stop = false;

    do{
        time_t nextrun;
        time(&nextrun);
        nextrun += 60; // run after 60 seconds

        while(true){
            stopmutex_->lock();
            stop = ptr->stop_;
            stopmutex_->unlock();

            if(stop)
                return;

            time_t current_time;
            time(&current_time);


            if( current_time >= nextrun )
                break;

            sleep(1);
        }

        ptr->tokenTimeout(ptr->net_);

    }
    while(true);
}

void PlaceTimeoutThread::tokenTimeout(DatabasePetriNet& net)
{
    net.tokenTimeout();
}
