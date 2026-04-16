#include "Table.h"
using namespace std;



Table::Table(int id, int cap) : ID(id), capacity(cap), freeSeats(cap),
sharable(false), freeTime(0) {
} 


int Table::getCapacity()
{
    return capacity;
    
}

int Table::getFreeSeats()
{
    return freeSeats;
}

bool Table::isSharable()
{
    return sharable;
}

int Table::getFreeTime()
{
    return freeTime;
}

void Table::reserveSeats(int s, int duration, int currentTime, bool share)
{
    freeSeats -= s;
    sharable = share;
    freeTime = currentTime + duration;
}

void Table::releaseSeats(int s)
{
    freeSeats += s;
    if (freeSeats == capacity) sharable = false;
}

ostream& operator<<(ostream& out, Table* t)
{
    out << "[T" << t->ID << " cap:" << t->capacity
        << " free:" << t->freeSeats << "]";
    return out;
}
