#include "Table.h"

Table::Table(int tableID, int cap)
{
    id = tableID;
    capacity = cap;
    freeSeats = cap;

    sharable = false;
    freeTime = 0;
}

int Table::getID() const { return id; }

int Table::getCapacity() const { return capacity; }

int Table::getFreeSeats() const { return freeSeats; }

bool Table::isSharable() const { return sharable; }

int Table::getFreeTime() const { return freeTime; }

void Table::reserveSeats(int seats, int duration, int currentTime, bool share)
{
    freeSeats -= seats;
    sharable = share;
    freeTime = currentTime + duration;
}

void Table::releaseSeats(int seats)
{
    freeSeats += seats;

    if (freeSeats == capacity)
        sharable = false;
}

ostream& operator<<(ostream& out, Table* t)
{
    out << "[T" << t->id << "]";
    return out;
}