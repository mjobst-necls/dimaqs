/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "TransitionHelper.h"
#include "../../lib/petrinet/src/Arc.h"
#include "../../lib/petrinet/src/Place.h"
#include "../../lib/petrinet/src/Transition.h"
#include "../../lib/petrinet/src/TransitionAction.h"
#include "../../lib/petrinet/src/Token.h"
#include "Controller.h"

TransitionHelper::TransitionHelper()
{

}

bool TransitionHelper::isMatching(const petrinet::Transition& transition, const petrinet::AbstractPetriNetQuery& query) const
{
    petrinet::TransitionAction* action = transition.getAction();
    return action->isMatching(query);
}

std::list<petrinet::PlaceAction*> TransitionHelper::trigger(petrinet::Transition& transition, const petrinet::AbstractPetriNetQuery& query) const
{
    std::list<petrinet::PlaceAction*> actions;
    unsigned int nrOfTokensTransferred = 0;

    if( transition.getOutgoing().size() == 0)
        return actions;

    if( Controller::getVerbosity() >= 4 ){
        std::string msg ="Triggered Transition (for matching conditions): " + transition.getName();
        Controller::s_printMessage(msg);
    }

    std::vector<petrinet::Place*> targetPlaces;
    for( unsigned int i = 0; i < transition.getOutgoing().size(); i++){
        petrinet::Place* target = static_cast<petrinet::Place*>(transition.getOutgoing().at(i)->getTarget());
        if( !transition.hasTowardsArc(*target) )
        {
            targetPlaces.push_back(target);
        }
    }

    std::vector<petrinet::Place*> sourcePlaces;
    for( unsigned int i = 0; i < transition.getIncomming().size(); i++){
        sourcePlaces.push_back(static_cast<petrinet::Place*>(transition.getIncomming().at(i)->getSource()));
    }

    for( unsigned int k = 0; k < targetPlaces.size(); k++){
        for( unsigned int i = 0; i < sourcePlaces.size(); i++){

            petrinet::TokenData* tokenData_ = transition.getTokenData(query);


            if( transition.hasBackwardsArc(*sourcePlaces.at(i))){
                // make a copy of the token and merge into target
                for (auto it = std::begin(sourcePlaces.at(i)->getTokens()); it!=std::end(sourcePlaces.at(i)->getTokens()); ++it)
                {
                    // check if conditions are fullfilled
                    if( !transition.checkConditions(*(*it) /*token*/, *tokenData_))
                        continue;

                    petrinet::Token* tokencopy = new petrinet::Token(*(*it));
                    actions.merge(targetPlaces.at(k)->addToken(tokencopy, tokenData_));

                    nrOfTokensTransferred++;
                }

            } else {
                // move the token and merge into target
                while( sourcePlaces.at(i)->getTokens().size() > 0){

                    petrinet::Token* tokenmove = sourcePlaces.at(i)->takeLastToken();
                    actions.merge(targetPlaces.at(k)->addToken(tokenmove, tokenData_));

                    nrOfTokensTransferred++;
                }
            }
        }
    }

    if( Controller::getVerbosity() >= 4 ){
        std::string msg ="Tokens transferred: " + std::to_string(nrOfTokensTransferred);
        Controller::s_printMessage(msg);

        static unsigned long counter = 0;

        if(     transition.getName() == "DT"
             || transition.getName() == "DD"){
            counter += nrOfTokensTransferred;
            Controller::s_printMessage("Notifications created: " + std::to_string(counter));
        }
    }

    return actions;
}
