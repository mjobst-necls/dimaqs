/*******************************************************
 * Copyright (C) 2018 Michael Jobst <mjobst@tecratech.de>
 *
 * This file is part of Dimaqs.
 *
 * Dimaqs can not be copied and/or distributed without the express
 * permission of Michael Jobst
 *******************************************************/

#ifndef ABSTRACTCLASSIFIER_H
#define ABSTRACTCLASSIFIER_H

#include "list"

namespace petrinet{
class AbstractPetriNetQuery;
class PlaceAction;
}

class AbstractClassifier
{
public:
    AbstractClassifier();
    virtual ~AbstractClassifier() {;}

    virtual std::list<petrinet::PlaceAction*> handle(const petrinet::AbstractPetriNetQuery& query) = 0;
};

#endif // ABSTRACTCLASSIFIER_H
