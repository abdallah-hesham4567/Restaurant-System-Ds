#pragma once
#include "Order.h"
class Restaurant;

class Action
{
    protected:
    int TimeStep;
    ORD_TYPE Order_Type;
    int ID;
    public:

    Action(int t): TimeStep(t){};

    int get_TimeStep(){return TimeStep;};
    int getOrderID() { return ID; };
    ORD_TYPE getOrderType() { return Order_Type; };
    virtual void Act(Restaurant* pRes) = 0;

};

