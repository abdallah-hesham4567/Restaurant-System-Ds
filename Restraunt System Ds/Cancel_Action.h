#pragma once
#include "Action.h"

class Restaurant;

// Cancels OVC orders only
class Cancel_Action : public Action
{
public:

    Cancel_Action(int time, int id);

    void Act(Restaurant* pRest) override;
};