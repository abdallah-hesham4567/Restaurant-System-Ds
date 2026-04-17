#pragma once
#include "Action.h"
class Cancel_Action : public Action
{
    private:

    ORD_TYPE Order_Type;
    int ID;

    public:

    Cancel_Action(ORD_TYPE ot, int t, int id);

    virtual void Act(Restaurant* pRest);
};

