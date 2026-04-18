#include "Cancel_Action.h"
#include "Restaurant.h"

Cancel_Action::Cancel_Action(int time, int id)
    : Action(time, id)
{}

void Cancel_Action::Act(Restaurant* pRest)
{
    pRest->Cancel_Order(orderID);
}