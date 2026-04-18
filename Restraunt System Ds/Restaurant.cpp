#include "Restaurant.h"

void Restaurant::AddOrder(Order* ord)
{
    switch (ord->getType()) {
    case ORD_TYPE::ODG:
        PEND_ODG.enqueue(ord);
        break;
    case ORD_TYPE::ODN:
        PEND_ODN.enqueue(ord);
        break;
    case ORD_TYPE::OT:
        PEND_OT.enqueue(ord);
        break;
    case ORD_TYPE::OVN:
        PEND_OVN.enqueue(ord);
        break;
    case ORD_TYPE::OVG:
        PEND_OVG.enqueue(ord, ord->getPriority());
        break;
	case ORD_TYPE::OVC:
        PEND_OVC.enqueue(ord);
		break;


    };
}

void Restaurant::Cancel_Order(int id)
{
    // search in PEND_OVC and move to CANCELLED if found
    if (PEND_OVC.CancelOrder(id)) {
        // find the order and add to CANCELLED
        
    }
}

