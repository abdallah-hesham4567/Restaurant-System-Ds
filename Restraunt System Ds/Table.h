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
    Table(int id, int cap)
        : ID(id), capacity(cap), freeSeats(cap),
        sharable(false), freeTime(0) {
    }

    int  getID() { return ID; }
    int  getCapacity() { return capacity; }
    int  getFreeSeats() { return freeSeats; }
    bool isSharable() { return sharable; }
    int  getFreeTime() { return freeTime; }

    void reserveSeats(int s, int duration, int currentTime, bool share) {
        freeSeats -= s;
        sharable = share;
        freeTime = currentTime + duration;
    }

    void releaseSeats(int s) {
        freeSeats += s;
        if (freeSeats == capacity) sharable = false;
    }

    friend ostream& operator<<(ostream& out, Table* t) {
        out << "[T" << t->ID << " cap:" << t->capacity
            << " free:" << t->freeSeats << "]";
        return out;
    }
};
