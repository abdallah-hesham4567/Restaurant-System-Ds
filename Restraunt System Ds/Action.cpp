#include "Action.h"

Action::Action(int time, int id)
{
    timeStep = time;
    orderID = id;
}

int Action::getTimeStep() const
{
    return timeStep;
}

int Action::getOrderID() const
{
    return orderID;
}