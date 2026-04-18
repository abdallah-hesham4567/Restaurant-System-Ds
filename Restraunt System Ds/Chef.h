#pragma once
#include <iostream>
#include <string>

using namespace std;

// Represents one chef in restaurant
class Chef
{
private:
    int id;
    string type;      // CS or CN
    bool busy;
    int busyTime;
    int speed;

public:

    Chef(int chefID, string chefType, int chefSpeed);

    // Getters
    int getID() const;
    string getType() const;
    bool isBusy() const;
    int getBusyTime() const;
    int getSpeed() const;

    // Setters
    void setBusy(bool state);
    void addBusyTime(int time);

    friend ostream& operator<<(ostream& out, Chef* chef);
};