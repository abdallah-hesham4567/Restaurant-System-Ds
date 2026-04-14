#pragma once
// Classes/Scooter.h
class Scooter {
private:
    int    ID;
    double speed;        // meters/timestep
    int    mainOrd;      // go to maintenance after this many orders
    int    mainDur;      // maintenance duration
    int    ordCount;     // orders delivered so far
    double distanceCut;  // total distance covered (for FREE_SCOOTERS priority)

public:
    Scooter(int id, double sp, int mOrd, int mDur)
        : ID(id), speed(sp), mainOrd(mOrd), mainDur(mDur),
        ordCount(0), distanceCut(0) {
    }

    int    getID() { return ID; }
    double getSpeed() { return speed; }
    int    getMainOrd() { return mainOrd; }
    int    getMainDur() { return mainDur; }
    int    getOrdCount() { return ordCount; }
    double getDistanceCut() { return distanceCut; }

    void   addDistance(double d) { distanceCut += d; }
    void   incrementOrders() { ordCount++; }
    bool   needsMaintenance() { return ordCount >= mainOrd; }
    void   resetOrdCount() { ordCount = 0; }

    friend ostream& operator<<(ostream& out, Scooter* s) {
        out << "[S" << s->ID << "]";
        return out;
    }
};

