#pragma once
// Classes/Table.h
#include <iostream>
#include <string>
using namespace std;
class Table {
private:
    int  ID;
    int  capacity;    // total seats
    int  freeSeats;   // currently available seats
    bool sharable;    // is anyone sharing currently?
    int  freeTime;    // timestep when table becomes fully free

public:
    Table(int id, int cap);
       

    int getID() { return ID; }
    int  getCapacity();
    int  getFreeSeats();
    bool isSharable();
    int  getFreeTime();

    void reserveSeats(int s, int duration, int currentTime, bool share);

    void releaseSeats(int s);
    friend ostream& operator<<(ostream& out, Table* t);

};
