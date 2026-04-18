#pragma once
#include "Action.h"
#include "Order.h"

class Restaurant;

class Request_Action  :  public Action
{   
    private:
    
    int Size;
    double Price;

    //optional for OD
    int Num_Seats;
    double Duration;
    bool CanShare;

    //optional for OV
    int Distance;

    public:
    Request_Action(ORD_TYPE ot, int t, int i, int s, double p);
    

    bool SetOD(int num, double dur, bool CS);
    bool SetOV(int d);
    virtual void Act(Restaurant* pRest);

};

