#include "Order.h"

// ================= Constructors =================

Order::Order(int id, ORD_TYPE t, int tq, int s, double p)
{
    ID = id; type = t; TQ = tq; size = s; price = p;

    TA = TR = TS = TF = 0;
    chefID = tableID = scooterID = -1;

    seats = duration = distance = 0;
    canShare = false;

    status = PENDING;

    // new
    chef = nullptr;
    scooter = nullptr;
    table = nullptr;
    finishCookTime = 0;
    finishServiceTime = 0;
}

Order::Order(int id, ORD_TYPE t, int tq, int s, double p,
    int st, double dur, bool share)
{
    ID = id; type = t; TQ = tq; size = s; price = p;

    seats = st;
    duration = dur;
    canShare = share;

    TA = TR = TS = TF = 0;
    chefID = tableID = scooterID = -1;

    distance = 0;

    status = PENDING;

    // new
    chef = nullptr;
    scooter = nullptr;
    table = nullptr;
    finishCookTime = 0;
    finishServiceTime = 0;
}

Order::Order(int id, ORD_TYPE t, int tq, int s, double p, int dist)
{
    ID = id; type = t; TQ = tq; size = s; price = p;

    distance = dist;

    TA = TR = TS = TF = 0;
    chefID = tableID = scooterID = -1;

    seats = duration = 0;
    canShare = false;

    status = PENDING;

    // new
    chef = nullptr;
    scooter = nullptr;
    table = nullptr;
    finishCookTime = 0;
    finishServiceTime = 0;
}

// ================= Getters =================

int      Order::getID() { return ID; }
ORD_TYPE Order::getType() { return type; }
ORD_STATUS Order::getStatus() { return status; }

int   Order::getTQ() { return TQ; }
int   Order::getTA() { return TA; }
int   Order::getTR() { return TR; }
int   Order::getTS() { return TS; }
int   Order::getTF() { return TF; }

int   Order::getSize() { return size; }
float Order::getPrice() { return price; }
int   Order::getChefID() { return chefID; }

int   Order::getSeats() { return seats; }
int   Order::getDuration() { return duration; }
bool  Order::getCanShare() { return canShare; }
int   Order::getTableID() { return tableID; }

int   Order::getDistance() { return distance; }
int   Order::getScooterID() { return scooterID; }

// new getters
Chef* Order::getChef() { return chef; }
Scooter* Order::getScooter() { return scooter; }
Table* Order::getTable() { return table; }
int      Order::getFinishCookTime() { return finishCookTime; }
int      Order::getFinishServiceTime() { return finishServiceTime; }

// ================= Setters =================

void Order::setStatus(ORD_STATUS s) { status = s; }

void Order::setTA(int ta) { TA = ta; }
void Order::setTR(int tr) { TR = tr; }
void Order::setTS(int ts) { TS = ts; }
void Order::setTF(int tf) { TF = tf; }

void Order::setChefID(int id) { chefID = id; }
void Order::setTableID(int id) { tableID = id; }
void Order::setScooterID(int id) { scooterID = id; }

// new setters
void Order::setChef(Chef* c) { chef = c; }
void Order::setScooter(Scooter* s) { scooter = s; }
void Order::setTable(Table* t) { table = t; }
void Order::setFinishCookTime(int t) { finishCookTime = t; }
void Order::setFinishServiceTime(int t) { finishServiceTime = t; }

// ================= Calculations =================

int Order::getTi() { return (TA - TQ) + (TS - TR); }
int Order::getTC() { return TR - TA; }
int Order::getTW() { return getTi() + getTC(); }
int Order::getTserv() { return TF - TS; }

// ================= Helper Functions =================

bool Order::isDineIn() { return type == ODG || type == ODN; }
bool Order::isDelivery() { return type == OVC || type == OVG || type == OVN; }
bool Order::isTakeaway() { return type == OT; }
bool Order::isGrilled() { return type == ODG || type == OVG; }
bool Order::isVIP() { return type == OVG; }

// ================= Priority =================

double Order::getPriority() {
    if (type != OVG) return 0;
    return price * 0.5 + size * 0.3 + distance * 0.2;
}

// ================= Output =================

ostream& operator<<(ostream& out, Order* o)
{
    out << o->TF << " "
        << o->ID << " "
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