#pragma once
#include "Action.h"

class Restaurant;

// Creates a new order request
class Request_Action : public Action
{
private:
    ORD_TYPE orderType;

    int size;
    double price;

    // Dine-in only
    int seats;
    int duration;
    bool canShare;

    // Delivery only
    int distance;

    Order* order;

public:

    Request_Action(ORD_TYPE type, int time,
        int id, int sz, double pr);

    bool setDineIn(int requiredSeats,
        int dur,
        bool share);

    bool setDelivery(int dist);

    string getOrderType() const;

    void Act(Restaurant* pRest) override;


    void setOrder(Order* o) { order = o; }
    Order* getOrder() const { return order; }
};