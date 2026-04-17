#pragma once
#include<Restaurant.h>
enum ORDER_TYPE { OD, ODG, ODN, OT, OV, OVC, OVG, OVN };

class Action
{
    private:
    int TimeStep;

    public:

    Action(int t): TimeStep(t){};

    int get_TimeStep(){return TimeStep;};

    virtual void Act(Restaurant* pRes) = 0;

};

