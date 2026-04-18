#include "Chef.h"

Chef::Chef(int chefID, string chefType, int chefSpeed)
{
    id = chefID;
    type = chefType;
    speed = chefSpeed;
    busy = false;
    busyTime = 0;
}

int Chef::getID() const { return id; }

string Chef::getType() const { return type; }

bool Chef::isBusy() const { return busy; }

int Chef::getBusyTime() const { return busyTime; }

int Chef::getSpeed() const { return speed; }

void Chef::setBusy(bool state)
{
    busy = state;
}

void Chef::addBusyTime(int time)
{
    busyTime += time;
}

ostream& operator<<(ostream& out, Chef* chef)
{
    out << "[Chef " << chef->id << "]";
    return out;
}