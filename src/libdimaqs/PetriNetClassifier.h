/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef PETRINETCLASSIFIER_H
#define PETRINETCLASSIFIER_H

#include "AbstractClassifier.h"
#include "list"

class DatabasePetriNet;

class PetriNetClassifier : public AbstractClassifier
{
public:
    PetriNetClassifier();
    virtual ~PetriNetClassifier();

    void addPetriNet(DatabasePetriNet* net);

    virtual std::list<petrinet::PlaceAction*> handle(const petrinet::AbstractPetriNetQuery& query);

private:
    std::list<DatabasePetriNet*>    nets_;
};

#endif // PETRINETCLASSIFIER_H
