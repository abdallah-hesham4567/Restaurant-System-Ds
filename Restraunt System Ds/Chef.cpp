#include "Chef.h"
#include "Scooter.h"




Chef::Chef(int id, string t)
{
    ID = id;
    type = t;
    busy = false;
    busyTime = 0;
}

int Chef::getID() { return ID; }
string Chef::getType() { return type; }
bool Chef::getBusy() { return busy; }
int Chef::getBusyTime() { return busyTime; }

int Chef::getSpeed()
{
    if (type == "CS")
        return CS_speed;
    else
        return CN_speed;
}

void Chef::setBusy(bool b) { busy = b; }
void Chef::setCSSpeed(int s) {
	if (s > 0) CS_speed = s;
	else CS_speed = 1; 
}
void Chef::setCNSpeed(int s) {
    if (s > 0) CN_speed = s;
    else CN_speed = 1; 
}
void Chef::addBusyTime(int t) { busyTime += t; }

ostream& operator<<(ostream& out, Chef* c)
{
    out << "[Chef" << c->ID
        << " Type:" << c->type
        << " Busy:" << (c->busy ? "Yes" : "No") << "]";
    return out;
}