#include "Request_Action.h"
#include "Restaurant.h"

Request_Action::Request_Action(ORD_TYPE ot, int t, int i, int s, double p)
    : Action(t)
{
    Order_Type = ot;
    ID = i;
    Size = s;
    Price = p;
    Num_Seats = 0;
    Duration = 0;
    CanShare = false;
    Distance = 0;
    pOrder = nullptr;  // created lazily in Act()
}

Request_Action::Request_Action(int t, Order* o) : Action(t)
{
    TimeStep = t;
    Order_Type = o->getType();
    ID = o->getID();
    Size = o->getSize();
    Price = o->getPrice();
	pOrder = o;  // already created, just store pointer
}

bool Request_Action::SetOD(int num, double dur, bool CS)
{
    if (Order_Type == ODG || Order_Type == ODN) {
        Num_Seats = num;
        Duration = dur;
        CanShare = CS;
        return true;
    }
    return false;
}

bool Request_Action::SetOV(int d)
{
    if (Order_Type == OVC || Order_Type == OVG || Order_Type == OVN) {
        Distance = d;
        return true;
    }
    return false;
}

// ── NEW: returns order ID for cancel logic ────────────────────────────────────
int Request_Action::getOrderID() { return ID; }

// ── NEW: returns the order pointer (created in Act, stored after) ─────────────
Order* Request_Action::getOrder() { return pOrder; }

// ── ACT: creates order and adds to restaurant ─────────────────────────────────
void Request_Action::Act(Restaurant* pRest)
{
    if (Order_Type == ODG || Order_Type == ODN)
        pOrder = new Order(ID, Order_Type, get_TimeStep(), Size, Price,
            Num_Seats, Duration, CanShare);
    else if (Order_Type == OVC || Order_Type == OVG || Order_Type == OVN)
        pOrder = new Order(ID, Order_Type, get_TimeStep(), Size, Price, Distance);
    else
        pOrder = new Order(ID, Order_Type, get_TimeStep(), Size, Price);

    pRest->AddOrder(pOrder);
}