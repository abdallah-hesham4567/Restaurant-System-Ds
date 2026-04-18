#pragma once
#include "Action.h"
#include "Order.h"
class Restaurant;

class Cancel_Action : public Action
{
   

    public:

    Cancel_Action(ORD_TYPE ot, int t, int id);
    
    virtual void Act(Restaurant* pRest);
};

