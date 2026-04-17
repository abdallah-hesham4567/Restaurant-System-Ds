#include "Cancel_Action.h"

Cancel_Action::Cancel_Action(ORD_TYPE ot, int t, int id) : Action(t)
{
    order_type = ot;
    ID = id;
}
void Cancel_Action::Act(Restaurant* pRest)
{
    if (order_type == ORD_TYPE::OV)
    {
        pRest->CancelOrder(ID);
    }
    else
    {
        //unable to delete order
    }
}