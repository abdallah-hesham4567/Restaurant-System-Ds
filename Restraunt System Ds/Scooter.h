#pragma once
#include <iostream>
using namespace std;

class Scooter {
private:
    int    ID;
    double speed;
    int    mainOrd;      // go to maintenance after this many orders
    int    mainDur;      // maintenance duration in timesteps
    int    ordCount;     // orders delivered so far
    double distanceCut;  // total distance covered
    int    mainEndTime;  // ← NEW: timestep when maintenance ends
    int backTime;
public:
    Scooter(int id, double sp, int mOrd, int mDur);

    int    getID();
    double getSpeed();
    int    getMainOrd();
    int    getMainDur();
    int    getOrdCount();
    double getDistanceCut();

    // new
    int    getMainEndTime();
    void   setMainEndTime(int t);

    int getBackTime();
    void setBackTime(int t);

    void   addDistance(double d);
    void   incrementOrders();
    bool   needsMaintenance();
    void   resetOrdCount();

    friend ostream& operator<<(ostream& out, Scooter* s);
};