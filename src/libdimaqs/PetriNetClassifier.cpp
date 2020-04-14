/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#include "PetriNetClassifier.h"
#include "DatabasePetriNet.h"

PetriNetClassifier::PetriNetClassifier()
{
}

PetriNetClassifier::~PetriNetClassifier()
{
    while( nets_.size() > 0){
        DatabasePetriNet* obj = nets_.back();
        nets_.pop_back();
        delete obj;
    }
}

void PetriNetClassifier::addPetriNet(DatabasePetriNet* net)
{
    nets_.push_back(net);
}

std::list<petrinet::PlaceAction*> PetriNetClassifier::handle(const petrinet::AbstractPetriNetQuery& query)
{
    std::list<petrinet::PlaceAction*> actions;

    for (auto it = std::begin(nets_); it!=std::end(nets_); ++it)
        actions.merge((*it)->handle(query));

    return actions;
}
