#include "Cancel_Action.h"

Cancel_Action(ORD_TYPE ot, int t, int id): Action(t)
{
    Order_Type = ot;
    ID = id;
}
void Cancel_Action::Act(restaurant* pRest)
{
    if(Order_Type == ORD_TYPE::OV)
    {
        pRest->CancelOrder(ID);
    }
    else
    {
        //unable to delete order
    }
}