#include "Request_Action.h"
#include "Restaurant.h"

Request_Action::Request_Action(
    ORD_TYPE type,
    int time,
    int id,
    int sz,
    double pr)
    : Action(time, id)
{
    orderType = type;
    size = sz;
    price = pr;

    seats = 0;
    duration = 0;
    canShare = false;

    distance = 0;
}

bool Request_Action::setDineIn(
    int requiredSeats,
    int dur,
    bool share)
{
    if (orderType == ODG || orderType == ODN)
    {
        seats = requiredSeats;
        duration = dur;
        canShare = share;
        return true;
    }

    return false;
}

bool Request_Action::setDelivery(int dist)
{
    if (orderType == OVC ||
        orderType == OVG ||
        orderType == OVN)
    {
        distance = dist;
        return true;
    }

    return false;
}

string Request_Action::getOrderType() const
{
    switch (orderType)
    {
    case ODG: return "ODG";
    case ODN: return "ODN";
    case OT:  return "OT";
    case OVC: return "OVC";
    case OVG: return "OVG";
    case OVN: return "OVN";
    default:  return "Unknown";
    }
}

void Request_Action::Act(Restaurant* pRest)
{
    Order* newOrder = nullptr;

    if (orderType == ODG || orderType == ODN)
    {
        newOrder = new Order(
            orderID,
            orderType,
            timeStep,
            size,
            price,
            seats,
            duration,
            canShare);
    }
    else if (orderType == OVC ||
        orderType == OVG ||
        orderType == OVN)
    {
        newOrder = new Order(
            orderID,
            orderType,
            timeStep,
            size,
            price,
            distance);
    }
    else
    {
        newOrder = new Order(
            orderID,
            orderType,
            timeStep,
            size,
            price);
    }

    pRest->AddOrder(newOrder);
}