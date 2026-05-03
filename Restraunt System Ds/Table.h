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

  
   

public:

    Table(int tableID, int cap);

    int getID() const;
    int getCapacity() const;
    int getFreeSeats() const;

    void reserveSeats(int seats, int duration, int currentTime);
    void releaseSeats(int seats);

    friend ostream& operator<<(ostream& out, Table* t);
};