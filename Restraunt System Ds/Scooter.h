#pragma once
#include <ostream>
using namespace std;
class Scooter {
private:
    int    ID;
    double speed;        // meters/timestep
    int    mainOrd;      // go to maintenance after this many orders
    int    mainDur;      // maintenance duration
    int    ordCount;     // orders delivered so far
    double distanceCut;  // total distance covered (for FREE_SCOOTERS priority)

public:
    Scooter(int id, double sp, int mOrd, int mDur);
       
        int    getID();
        double getSpeed();
        int    getMainOrd();
        int    getMainDur();
        int    getOrdCount();
        double getDistanceCut(); 

        void   addDistance(double d); 
        void   incrementOrders(); 
        bool   needsMaintenance(); 
        void   resetOrdCount();

        friend ostream& operator<<(ostream& out, Scooter* s); 
};

