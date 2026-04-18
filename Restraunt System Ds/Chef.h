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
     int CS_speed;
    int CN_speed;

public:
    

    Chef(int id, string t,int speed);

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