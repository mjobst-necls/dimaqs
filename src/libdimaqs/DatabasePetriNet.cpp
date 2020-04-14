/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "DatabasePetriNet.h"
#include "../../lib/petrinet/src/Arc.h"
#include "DatabasePetriNetData.h"
#include "../../lib/petrinet/src/Place.h"
#include "../../lib/petrinet/src/Transition.h"
#include "../../lib/petrinet/src/Token.h"
#include "../../lib/petrinet/src/AbstractPetriNetQuery.h"
#include "../../lib/petrinet/src/PlaceAction.h"

#include "transitionaction/AlwaysAction.h"
#include "Controller.h"
#include "PlaceTimeoutThread.h"
#include "mutex"

template <typename T>
bool listContains(const std::list<T>& l, T e)
{
    for (auto it = std::begin(l); it!=std::end(l); ++it) {
        if( (*it) == e)
            return true;
    }

    return false;
}

template <typename T>
void listMoveFromTo(std::list<T>& from, std::list<T>& to)
{
    while(from.size() > 0){
        to.push_back(from.front());
        from.pop_front();
    }

}

DatabasePetriNet::DatabasePetriNet()
    : data_(NULL)
{
    mutex_ = new std::mutex();
    timeoutThread_ = new PlaceTimeoutThread(*this);
}

DatabasePetriNet::~DatabasePetriNet()
{
    delete timeoutThread_;
    delete data_;
    delete mutex_;
}

std::list<petrinet::PlaceAction*> DatabasePetriNet::handle(const petrinet::AbstractPetriNetQuery& query)
{
    std::list<petrinet::PlaceAction*> executedAction;

    if( query.getQueryType().empty() ) return executedAction;

    mutex_->lock();

    if( Controller::getVerbosity() >= 4 ){
        Controller::s_printMessage("----------------------------------------");
        Controller::s_printMessage("Query: " + query.getQuery());
        Controller::s_printMessage("Query Type: " + query.getQueryType());
    }

    std::list<petrinet::Transition*> activeTransitions = getActiveTransitions();
    std::list<petrinet::Transition*> transitionsToTrigger;

    for (auto it = std::begin(activeTransitions); it!=std::end(activeTransitions); ++it)
    {
        if( (*it)->isMatching(query) ){
            transitionsToTrigger.push_back(*it);
        }
    }

    if( Controller::getVerbosity() >= 4 ){
        std::string msg ="Active Transitions: ";
        for (auto it = std::begin(activeTransitions); it!=std::end(activeTransitions); ++it)
        {
            if( it != std::begin(activeTransitions) ) msg += ',';
            msg += (*it)->getName();
        }

        Controller::s_printMessage(msg);
    }

    for (auto it = std::begin(transitionsToTrigger); it!=std::end(transitionsToTrigger); ++it)
    {
        std::list<petrinet::PlaceAction*> tmpActions = (*it)->trigger(query);
        listMoveFromTo(tmpActions, executedAction);
    }

    if( Controller::getVerbosity() >= 4 ){

        std::list<petrinet::Place*> activePlaces = getActivePlaces();
        std::string msg;

        msg = "ActivePlaces(Tokens): ";

        for (auto it = std::begin(activePlaces); it!=std::end(activePlaces); ++it)
        {
            if( it != std::begin(activePlaces) ) msg += ',';
            msg += (*it)->getName() + "(" + std::to_string((*it)->getTokens().size()) + ")";
        }

        Controller::s_printMessage(msg);
    }

    // execute Always-Transtions
    activeTransitions = getActiveTransitions();
    transitionsToTrigger.clear();
    for (auto it = std::begin(activeTransitions); it!=std::end(activeTransitions); ++it)
    {
        if( dynamic_cast<transitionaction::AlwaysAction*>((*it)->getAction()) != NULL){
            std::list<petrinet::PlaceAction*> tmpActions = (*it)->trigger(query);
            listMoveFromTo(tmpActions, executedAction);
        }
    }

    mutex_->unlock();

    return executedAction;
}

std::list<petrinet::Place*> DatabasePetriNet::getActivePlaces() const
{
    std::list<petrinet::Place*> activePlaces;

    for (auto it = std::begin(places_); it!=std::end(places_); ++it)
    {
        if( (*it)->getTokens().size() > 0 ){
            activePlaces.push_back(*it);
        }
    }

    return activePlaces;
}

std::list<petrinet::Transition*> DatabasePetriNet::getActiveTransitions() const
{
    std::list<petrinet::Transition*>  activeTransitions;
    std::list<petrinet::Place*> activePlaces = getActivePlaces();

    for (auto it_ap = std::begin(activePlaces); it_ap!=std::end(activePlaces); ++it_ap)
    {
        for (auto it_out = std::begin((*it_ap)->getOutgoing()); it_out!=std::end((*it_ap)->getOutgoing()); ++it_out)
        {
            petrinet::Transition* trans = static_cast<petrinet::Transition*>((*it_out)->getTarget());
            if( !trans->isActive() )
                continue;

            // append only once
            if( !listContains(activeTransitions, trans))
                activeTransitions.push_back(trans);
        }
    }

    return activeTransitions;
}

void DatabasePetriNet::tokenTimeout()
{
    mutex_->lock();

    std::list<petrinet::Place*> places = getActivePlaces();

    for (auto it = std::begin(places); it!=std::end(places); ++it)
    {
        (*it)->tokenTimeout();
    }

    mutex_->unlock();
}
