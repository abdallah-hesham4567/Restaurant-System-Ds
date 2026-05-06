#pragma once
#include <iostream>

using namespace std;

// Delivery scooter
class Scooter
{
private:
    int id;
    double speed;

    int maintenanceOrders;
    int maintenanceDuration;

    int deliveredOrders;
    double totalDistance;

    int returnTime;
    int maintenanceEnd;
    int busyTime;

/// /////// bonus part
    bool failed;
    int failTime;
    double distanceCovered;  // how far before failure

public:

    Scooter(int ID, double sp, int mOrders, int mDuration);

    // Getters
    int getID() const;
    double getSpeed() const;
    int getReturnTime() const;
    int getMaintenanceEnd() const;
    double getDistanceCut() const;

    // Setters
    void setReturnTime(int time);
    void setMaintenanceEnd(int time);

    void addDistance(double d);
    void incrementOrders();
    void resetOrders();

    bool needsMaintenance() const;
    void resetDistance();
    friend ostream& operator<<(ostream& out, Scooter* s);

    void addBusyTime(int t);
    int getBusyTime() const;



	/////bonus part
    void setFailed(int time, double covered);
    bool hasFailed() const;
    double getDistanceCovered() const;
    void resetFailed();
};