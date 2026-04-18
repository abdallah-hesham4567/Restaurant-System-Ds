#pragma once
#include "Order.h"

class Restaurant;

// Base class for all actions
class Action
{
protected:
    int timeStep;
    int orderID;

public:

    Action(int time, int id = 0);

    virtual ~Action() {}

    int getTimeStep() const;
    int getOrderID() const;

    // Execute action
    virtual void Act(Restaurant* pRest) = 0;
};