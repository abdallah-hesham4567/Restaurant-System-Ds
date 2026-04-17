#pragma once
#include "Action.h"

class Cancel_Action : public Action
{
    private:

    int ID;
    ORD_TYPE order_type;

    public:

    Cancel_Action(ORD_TYPE ot, int t, int id);

    virtual void Act(Restaurant* pRest);
};

