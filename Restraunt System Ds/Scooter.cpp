#include "Scooter.h"

Scooter::Scooter(int ID, double sp, int mOrders, int mDuration)
{
    id = ID;
    speed = sp;

    maintenanceOrders = mOrders;
    maintenanceDuration = mDuration;

    deliveredOrders = 0;
    totalDistance = 0;

    returnTime = 0;
    maintenanceEnd = 0;
    busyTime = 0;
    // for bonus part
    failed = false;
    failTime = 0;
    distanceCovered = 0;
}

int Scooter::getID() const { return id; }

double Scooter::getSpeed() const { return speed; }

int Scooter::getReturnTime() const { return returnTime; }

int Scooter::getMaintenanceEnd() const { return maintenanceEnd; }

double Scooter::getDistanceCut() const { return totalDistance; }

void Scooter::setReturnTime(int time)
{
    returnTime = time;
}

void Scooter::setMaintenanceEnd(int time)
{
    maintenanceEnd = time;
    resetDistance();
}

void Scooter::addDistance(double d)
{
    totalDistance += d;
}

void Scooter::incrementOrders()
{
    deliveredOrders++;
}

void Scooter::resetOrders()
{
    deliveredOrders = 0;
}

bool Scooter::needsMaintenance() const
{
    return deliveredOrders >= maintenanceOrders;
}

void Scooter::resetDistance()
{
    totalDistance = 0.0;
}

void Scooter::addBusyTime(int t)
{
    busyTime += t;
}

int Scooter::getBusyTime() const
{
    return busyTime;
}

ostream& operator<<(ostream& out, Scooter* s)
{
    out << "[S" << s->id << "]";
    return out;
}



// bonus

void Scooter::setFailed(int time, double covered)
{
    failed = true; failTime = time; distanceCovered = covered;
}

bool Scooter::hasFailed() const { return failed; }

double Scooter::getDistanceCovered() const { return distanceCovered; }

void Scooter::resetFailed()
{
    failed = false; failTime = 0; distanceCovered = 0;
}