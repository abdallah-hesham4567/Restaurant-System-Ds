#include "Request_Action.h"

Request_Action::Request_Action(ORD_TYPE ot, int t, int i, int s, double p): Action(t)
{
    Order_Type = ot;
    ID = i;
    Size = s;
    Price = p;

    Num_Seats = 0;
    Duration = 0;
    CanShare = false;

    Distance = 0;
}

bool Request_Action::SetOD(int num, double dur, bool CS)
{
    if (Order_Type == ORD_TYPE::OD)
        {
            Num_Seats = num;
            Duration = dur;
            CanShare = CS;
        }
        else
        {
            return false;
        }
}
bool Request_Action::SetOV(int d)
{
    if(Order_Type == ORD_TYPE::OV)
    {
        Distance = d;
    }
    else
    {
        return false;
    }
}

void Request_Action::Act(Restaurant* pRest)
{
    Order* pOrder = nullptr;
    if(Order_Type == ORD_TYPE::OD)
    {
        pOrder = new Order(ID, Order_Type, get_TimeStep(), Size, Price,Num_Seats,Duration,CanShare);
    }
    else if(Order_Type == ORD_TYPE::OV)
    {
        pOrder = new Order(ID, Order_Type, get_TimeStep(),Size,Price,Distance);
    }
    else
    {
        pOrder = new Order(ID, Order_Type, get_TimeStep(),Size,Price);
    }
    pRest->AddOrder(pOrder); //    <<--- AddOrder should be made in the restaurant class
}
