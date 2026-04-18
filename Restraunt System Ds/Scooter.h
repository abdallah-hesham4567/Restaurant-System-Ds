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

    friend ostream& operator<<(ostream& out, Scooter* s);
};