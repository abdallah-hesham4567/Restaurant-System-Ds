#pragma once
#include <iostream>

using namespace std;

// Restaurant table
class Table
{
private:
    int id;
    int capacity;
    int freeSeats;

    bool sharable;
    int freeTime;

public:

    Table(int tableID, int cap);

    int getID() const;
    int getCapacity() const;
    int getFreeSeats() const;
    bool isSharable() const;
    int getFreeTime() const;

    void reserveSeats(int seats, int duration, int currentTime, bool share);
    void releaseSeats(int seats);

    friend ostream& operator<<(ostream& out, Table* t);
};