#pragma once
#include <iostream>
#include <string>
using namespace std;

class Chef
{
private:
    int ID;
    string type;
    bool busy;
    int busyTime;
    static int CS_speed;
    static int CN_speed;

public:
    

    Chef(int id, string t);

    int getID();
    string getType();
    bool getBusy();
    int getBusyTime();
    int getSpeed();

    void setBusy(bool b);
    void setCSSpeed(int s);
	void setCNSpeed(int s);
    void addBusyTime(int t);

    friend ostream& operator<<(ostream& out, Chef* c);
};