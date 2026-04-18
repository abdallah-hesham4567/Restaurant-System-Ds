#pragma once
#include "Action.h"
#include "Order.h"

class Restaurant;  // forward declaration

class Request_Action : public Action {
private:
    ORD_TYPE Order_Type;
    int      ID;
    int      Size;
    double   Price;

    // dine-in
    int    Num_Seats;
    double Duration;
    bool   CanShare;

    // delivery
    int    Distance;

    // ── NEW: stored after Act() creates it ───────
    Order* pOrder;

public:
    Request_Action(ORD_TYPE ot, int t, int i, int s, double p);
    Request_Action(int t, Order* o);
    bool SetOD(int num, double dur, bool CS);
    bool SetOV(int d);

    // new
    int    getOrderID();
    Order* getOrder();

    string getOrderType()
    {
        switch (Order_Type) {
        case ODN: return "ODN";
        case ODG: return "ODG";
        case OT:  return "OT";
        case OVN: return "OVN";
        case OVC: return "OVC";
        case OVG: return "OVG";
        default:  return "Unknown";
        }
    }
    void Act(Restaurant* pRest) override;
};