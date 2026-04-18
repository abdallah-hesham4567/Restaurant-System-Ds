#include "Order.h"
#include "Chef.h"
#include "Scooter.h"
#include "Table.h"

/*
==================================================
 Constructors
==================================================
*/

// Takeaway
Order::Order(int ID, ORD_TYPE t, int tq, int sz, double pr)
{
    id = ID;
    type = t;
    TQ = tq;
    size = sz;
    price = pr;

    TA = TR = TS = TF = 0;

    seats = 0;
    duration = 0;
    canShare = false;

    distance = 0;

    chefID = scooterID = tableID = -1;

    chef = nullptr;
    scooter = nullptr;
    table = nullptr;

    finishCookTime = 0;
    finishServiceTime = 0;

    status = PENDING;
}

// Dine-in
Order::Order(int ID, ORD_TYPE t, int tq, int sz, double pr,
    int requiredSeats, int dur, bool share)
{
    id = ID;
    type = t;
    TQ = tq;
    size = sz;
    price = pr;

    TA = TR = TS = TF = 0;

    seats = requiredSeats;
    duration = dur;
    canShare = share;

    distance = 0;

    chefID = scooterID = tableID = -1;

    chef = nullptr;
    scooter = nullptr;
    table = nullptr;

    finishCookTime = 0;
    finishServiceTime = 0;

    status = PENDING;
}

// Delivery
Order::Order(int ID, ORD_TYPE t, int tq, int sz, double pr,
    int dist)
{
    id = ID;
    type = t;
    TQ = tq;
    size = sz;
    price = pr;

    TA = TR = TS = TF = 0;

    seats = 0;
    duration = 0;
    canShare = false;

    distance = dist;

    chefID = scooterID = tableID = -1;

    chef = nullptr;
    scooter = nullptr;
    table = nullptr;

    finishCookTime = 0;
    finishServiceTime = 0;

    status = PENDING;
}

/*
==================================================
 Getters
==================================================
*/

int Order::getID() const { return id; }
ORD_TYPE Order::getType() const { return type; }
ORD_STATUS Order::getStatus() const { return status; }

int Order::getTQ() const { return TQ; }
int Order::getTA() const { return TA; }
int Order::getTR() const { return TR; }
int Order::getTS() const { return TS; }
int Order::getTF() const { return TF; }

int Order::getSize() const { return size; }
double Order::getPrice() const { return price; }

int Order::getSeats() const { return seats; }
int Order::getDuration() const { return duration; }
bool Order::getCanShare() const { return canShare; }

int Order::getDistance() const { return distance; }

int Order::getChefID() const { return chefID; }
int Order::getScooterID() const { return scooterID; }
int Order::getTableID() const { return tableID; }

int Order::getFinishCookTime() const { return finishCookTime; }
int Order::getFinishServiceTime() const { return finishServiceTime; }

Chef* Order::getChef() const { return chef; }
Scooter* Order::getScooter() const { return scooter; }
Table* Order::getTable() const { return table; }

/*
==================================================
 Setters
==================================================
*/

void Order::setStatus(ORD_STATUS s) { status = s; }

void Order::setTA(int time) { TA = time; }
void Order::setTR(int time) { TR = time; }
void Order::setTS(int time) { TS = time; }
void Order::setTF(int time) { TF = time; }

void Order::setChefID(int ID) { chefID = ID; }
void Order::setScooterID(int ID) { scooterID = ID; }
void Order::setTableID(int ID) { tableID = ID; }

void Order::setFinishCookTime(int time)
{
    finishCookTime = time;
}

void Order::setFinishServiceTime(int time)
{
    finishServiceTime = time;
}

void Order::setChef(Chef* c) { chef = c; }
void Order::setScooter(Scooter* s) { scooter = s; }
void Order::setTable(Table* t) { table = t; }

/*
==================================================
 Helper Functions
==================================================
*/

bool Order::isDineIn() const
{
    return type == ODG || type == ODN;
}

bool Order::isDelivery() const
{
    return type == OVC || type == OVG || type == OVN;
}

bool Order::isTakeaway() const
{
    return type == OT;
}

bool Order::isGrilled() const
{
    return type == ODG || type == OVG;
}

bool Order::isVIP() const
{
    return type == OVG;
}

/*
==================================================
 Time Calculations
==================================================
*/

int Order::getTi() const
{
    return (TA - TQ) + (TS - TR);
}

int Order::getTC() const
{
    return TR - TA;
}

int Order::getTW() const
{
    return getTi() + getTC();
}

int Order::getTserv() const
{
    return TF - TS;
}

/*
==================================================
 Priority for OVG Orders
==================================================
*/

double Order::getPriority() const
{
    if (type != OVG)
        return 0;

    return price * 0.5 +
        size * 0.3 +
        distance * 0.2;
}

/*
==================================================
 Output
==================================================
*/

ostream& operator<<(ostream& out, Order* o)
{
    out << o->TF << " "
        << o->id << " "
        << o->TQ << " "
        << o->TA << " "
        << o->TR << " "
        << o->TS << " "
        << o->getTi() << " "
        << o->getTC() << " "
        << o->getTW() << " "
        << o->getTserv();

    return out;
}