#include "Table.h"

Table::Table(int tableID, int cap)
{
    id = tableID;
    capacity = cap;
    freeSeats = cap;
   
}

int Table::getID() const { return id; }

int Table::getCapacity() const { return capacity; }

int Table::getFreeSeats() const { return freeSeats; }

void Table::reserveSeats(int seats, int duration, int currentTime)
{
    freeSeats -= seats;                 
}

void Table::releaseSeats(int seats)
{
    freeSeats += seats;    
}

ostream& operator<<(ostream& out, Table* t)
{
    out << "[T" << t->id << "]";
    return out;
}