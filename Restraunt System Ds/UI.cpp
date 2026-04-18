#include "UI.h"
using namespace std;

// ?? ACTIONS LIST ?????????????????????????????????????????????????????????????
// prints: [ODG, 100, 333] for request, (X, 105, 79) for cancel
// only first 10 actions printed
void UI::printActions(
    LinkedQueue<Request_Action*>& Ractions,
    LinkedQueue<Cancel_Action*>& Cactions)
{
    int total = Ractions.getCount() + Cactions.getCount();
    cout << "================ Actions List ================\n";
    cout << total << " actions remaining: ";

    // print first 10 actions total (mix of R and C)
    int printed = 0;

    // print from request actions first
    Node<Request_Action*>* rcur = Ractions.getFront();
    while (rcur && printed < 10) {
        Request_Action* a = rcur->item;
        // format: [OrderType, TQ, OrderID]
        cout << "[" << a->getOrderType()
            << ", " << a->get_TimeStep()
            << ", " << a->getOrderID() << "]";
        printed++;
        rcur = rcur->next;
        if (rcur || printed < 10) cout << ", ";
    }

    // print from cancel actions
    Node<Cancel_Action*>* ccur = Cactions.getFront();
    while (ccur && printed < 10) {
        Cancel_Action* a = ccur->item;
        // format: (X, Tcancel, orderID)
        cout << "(X, " << a->get_TimeStep()
            << ", " << a->getOrderID() << ")";
        printed++;
        ccur = ccur->next;
        if (ccur && printed < 10) cout << ", ";
    }

    if (total > 10) cout << " .....";
    cout << "\n\n";
}

// ?? PENDING ORDERS ????????????????????????????????????????????????????????????
// for each list: print count, type label, all IDs
void UI::printPendingOrders(
    LinkedQueue<Order*>& pendODG,
    LinkedQueue<Order*>& pendODN,
    LinkedQueue<Order*>& pendOT,
    LinkedQueue<Order*>& pendOVN,
    Pend_OVC& pendOVC,
    PriQueue<Order*>& pendOVG)
{
    cout << "--------------- Pending Orders IDs -----------------------\n";

    // helper lambda-style: print one pending list
    // ODG
    cout << pendODG.getCount() << " ODG: ";
    pendODG.printIDs();
    cout << "\n";

    // ODN
    cout << pendODN.getCount() << " ODN: ";
    pendODN.printIDs();
    cout << "\n";

    // OT
    cout << pendOT.getCount() << " OT: ";
    pendOT.printIDs();
    cout << "\n";

    // OVN
    cout << pendOVN.getCount() << " OVN: ";
    pendOVN.printIDs();
    cout << "\n";

    // OVC
    cout << pendOVC.getCount() << " OVC: ";
    pendOVC.printIDs();
    cout << "\n";

    // OVG
    cout << pendOVG.getCount() << " OVG: ";
    pendOVG.printIDs();
    cout << "\n\n";
}

// ?? AVAILABLE CHEFS ???????????????????????????????????????????????????????????
// format:  2 CS : 10, 22
//         10 CN : 2, 7, ..... (print all)
void UI::printAvailableChefs(
    LinkedQueue<Chef*>& freeCS,
    LinkedQueue<Chef*>& freeCN)
{
    cout << "--------------- Available Chefs IDs ----------------------\n";

    cout << freeCS.getCount() << " CS : ";
    freeCS.printIDs();
    cout << "\n";

    cout << freeCN.getCount() << " CN : ";
    freeCN.printIDs();
    cout << "\n\n";
}

// ?? COOKING ORDERS ????????????????????????????????????????????????????????????
// format:  23 cooking orders: [55, 3], [12, 2], ..... (print all)
// [OrderID, ChefID]
void UI::printCookingOrders(Cook_Ords& cooking)
{
    cout << "--------------- Cooking Orders [Orders ID, chef ID] ------\n";
    cout << cooking.getCount() << " cooking orders: ";
    cooking.printIDs();   // Cook_Ords::printIDs() prints [orderID, chefID]
    cout << "\n\n";
}

// ?? READY ORDERS ??????????????????????????????????????????????????????????????
// for each ready list: count, type, all IDs
void UI::printReadyOrders(
    LinkedQueue<Order*>& rdyOT,
    RDY_OV& rdyOV,
    LinkedQueue<Order*>& rdyOD)
{
    cout << "--------------- Ready Orders IDs -------------------------\n";

    cout << rdyOT.getCount() << " OT: ";
    rdyOT.printIDs();
    cout << "\n";

    cout << rdyOV.getCount() << " OV: ";
    rdyOV.printIDs();
    cout << "\n";

    cout << rdyOD.getCount() << " OD: ";
    rdyOD.printIDs();
    cout << "\n\n";
}

// ?? AVAILABLE SCOOTERS ????????????????????????????????????????????????????????
// format:  8 Scooters : 17, 12, 33, ..... (print all)
void UI::printAvailableScooters(PriQueue<Scooter*>& freeScooters)
{
    cout << "--------------- Available Scooters IDs -------------------\n";
    cout << freeScooters.getCount() << " Scooters : ";
    freeScooters.printIDs();
    cout << "\n\n";
}

// ?? AVAILABLE TABLES ??????????????????????????????????????????????????????????
// format:  30 tables : [T7, 8, 3], ..... (print all)
// [TableID, capacity, free seats]
void UI::printAvailableTables(Fit_Tables& freeTables)
{
    cout << "--------------- Available Tables [ID, capacity, free seats]\n";
    cout << freeTables.getCount() << " tables : ";
    freeTables.printIDs();   // Fit_Tables::printIDs() prints [T7, 8, 3]
    cout << "\n\n";
}

// ?? IN-SERVICE ORDERS ?????????????????????????????????????????????????????????
// format:  15 Orders: [75, S14], [6, T3] ..... (print all)
// [OrderID, ScooterID or TableID]
void UI::printInService(PriQueue<Order*>& inService)
{
    cout << "--------------- In-Service orders [order ID, scooter/Table ID]\n";
    cout << inService.getCount() << " Orders: ";
    //just  printing In-Service orders [order ID, scooter/Table ID] if ov or oc and handling it
	Node<Order*>* cur = inService.getFront();
	while (cur) {
		Order* o = cur->item;
		cout << "[" << o->getID() << ", ";
		if (o->isDelivery()) cout << "S" << o->getScooterID();
		else cout << "T" << o->getTableID();
		cout << "] ";
		cur = cur->next;
	}
	
	
       // PriQueue<Order*>::printIDs() prints [orderID, resourceID]
    cout << "\n\n";
}

// ?? MAINTENANCE SCOOTERS ??????????????????????????????????????????????????????
// format:  3 scooters: 40, 13, 9
void UI::printMaintenanceScooters(LinkedQueue<Scooter*>& maintScooters)
{
    cout << "--------------- In-maintenance Scooters IDs --------------\n";
    cout << maintScooters.getCount() << " scooters: ";
    maintScooters.printIDs();
    cout << "\n\n";
}

// ?? BACK SCOOTERS ?????????????????????????????????????????????????????????????
// format:  2 scooters: 74, 93
void UI::printBackScooters(PriQueue<Scooter*>& backScooters)
{
    cout << "--------------- Scooters Back to Restaurant IDs ----------\n";
    cout << backScooters.getCount() << " scooters: ";
    backScooters.printIDs();
    cout << "\n\n";
}

// ?? CANCELLED ORDERS ?????????????????????????????????????????????????????????
// format:  1 cancelled: 68
void UI::printCancelledOrders(LinkedQueue<Order*>& cancelled)
{
    cout << "--------------- Cancelled Orders IDs ---------------------\n";
    cout << cancelled.getCount() << " cancelled: ";
    cancelled.printIDs();
    cout << "\n\n";
}

// ?? FINISHED ORDERS ??????????????????????????????????????????????????????????
// format:  54 Orders: 39, 2, 19, 1 ..... (descending finish time — stack gives this)
void UI::printFinishedOrders(Stack<Order*>& finished)
{
    cout << "--------------- Finished Orders IDs ----------------------\n";
    cout << finished.getCount() << " Orders: ";
    finished.printIDs();   // Stack prints from top (latest finished first)
    cout << "\n\n";
}

// ?? MASTER PRINT ?????????????????????????????????????????????????????????????
void UI::printTimestep(
    int timestep,
    LinkedQueue<Request_Action*>& Ractions,
    LinkedQueue<Cancel_Action*>& Cactions,
    LinkedQueue<Order*>& pendODG,
    LinkedQueue<Order*>& pendODN,
    LinkedQueue<Order*>& pendOT,
    LinkedQueue<Order*>& pendOVN,
    Pend_OVC& pendOVC,
    PriQueue<Order*>& pendOVG,
    LinkedQueue<Chef*>& freeCS,
    LinkedQueue<Chef*>& freeCN,
    Cook_Ords& cooking,
    LinkedQueue<Order*>& rdyOT,
    RDY_OV& rdyOV,
    LinkedQueue<Order*>& rdyOD,
    PriQueue<Scooter*>& freeScooters,
    Fit_Tables& freeTables,
    PriQueue<Order*>& inService,
    LinkedQueue<Scooter*>& maintScooters,
    PriQueue<Scooter*>& backScooters,
    Stack<Order*>& finished,
    LinkedQueue<Order*>& cancelled)
{
    cout << "\nCurrent Timestep: " << timestep << "\n";

    printActions(Ractions, Cactions);
    printPendingOrders(pendODG, pendODN, pendOT, pendOVN, pendOVC, pendOVG);
    printAvailableChefs(freeCS, freeCN);
    printCookingOrders(cooking);
    printReadyOrders(rdyOT, rdyOV, rdyOD);
    printAvailableScooters(freeScooters);
    printAvailableTables(freeTables);
    printInService(inService);
    printMaintenanceScooters(maintScooters);
    printBackScooters(backScooters);
    printCancelledOrders(cancelled);
    printFinishedOrders(finished);

    cout << "PRESS ANY KEY TO MOVE TO NEXT STEP!\n";
}