#include "UI.h"

/*
==================================================
 Actions
==================================================
*/

void UI::printActions(
    LinkedQueue<Request_Action*>& requests,
    LinkedQueue<Cancel_Action*>& cancels)
{
    cout << "=============== Actions List ===============\n";

    int total =
        requests.getCount() +
        cancels.getCount();

    cout << total << " actions remaining: ";

    int printed = 0;

    // print first request actions
    Node<Request_Action*>* r =
        requests.getFront();

    while (r && printed < 10)
    {
        Request_Action* a = r->item;

        cout << "("
            << a->getOrderType()
            << ", "
            << a->getTimeStep()
            << ", "
            << a->getOrderID()
            << ")";

        printed++;

        if (printed < 10 &&
            (r->next || !cancels.isEmpty()))
            cout << ", ";

        r = r->next;
    }

    // print cancel actions if still space
    Node<Cancel_Action*>* c =
        cancels.getFront();

    while (c && printed < 10)
    {
        Cancel_Action* a = c->item;

        cout << "(X, "
            << a->getTimeStep()
            << ", "
            << a->getOrderID()
            << ")";

        printed++;

        if (printed < 10 && c->next)
            cout << ", ";

        c = c->next;
    }

    cout << endl;
}

/*
==================================================
 Pending Orders
==================================================
*/

void UI::printPendingOrders(
    LinkedQueue<Order*>& pendODG,
    LinkedQueue<Order*>& pendODN,
    LinkedQueue<Order*>& pendOT,
    LinkedQueue<Order*>& pendOVN,
    Pend_OVC& pendOVC,
    PriQueue<Order*>& pendOVG)
{
    cout << "----------- Pending Orders -----------\n";

    cout << pendODG.getCount()
        << " ODG : ";
    pendODG.printIDs();
    cout << endl;

    cout << pendODN.getCount()
        << " ODN : ";
    pendODN.printIDs();
    cout << endl;

    cout << pendOT.getCount()
        << " OT  : ";
    pendOT.printIDs();
    cout << endl;

    cout << pendOVN.getCount()
        << " OVN : ";
    pendOVN.printIDs();
    cout << endl;

    cout << pendOVC.getCount()
        << " OVC : ";
    pendOVC.printIDs();
    cout << endl;

    cout << pendOVG.getCount()
        << " OVG : ";
    pendOVG.printIDs();
    cout << "\n\n";
}

/*
==================================================
 Free Chefs
==================================================
*/

void UI::printAvailableChefs(
    LinkedQueue<Chef*>& freeCS,
    LinkedQueue<Chef*>& freeCN)
{
    cout << "----------- Available Chefs -----------\n";

    cout << freeCS.getCount()
        << " CS : ";
    freeCS.printIDs();
    cout << endl;

    cout << freeCN.getCount()
        << " CN : ";
    freeCN.printIDs();
    cout << "\n\n";
}

/*
==================================================
 Cooking
==================================================
*/

void UI::printCookingOrders(
    Cook_Ords& cooking)
{
    cout << "----------- Cooking Orders -----------\n";

    cout << cooking.getCount()
        << " Orders : ";

    cooking.printIDs();

    cout << "\n\n";
}

/*
==================================================
 Ready Orders
==================================================
*/

void UI::printReadyOrders(
    LinkedQueue<Order*>& readyOT,
    RDY_OV& readyOV,
    LinkedQueue<Order*>& readyOD)
{
    cout << "----------- Ready Orders -----------\n";

    cout << readyOT.getCount()
        << " OT : ";
    readyOT.printIDs();
    cout << endl;

    cout << readyOV.getCount()
        << " OV : ";
    readyOV.printIDs();
    cout << endl;

    cout << readyOD.getCount()
        << " OD : ";
    readyOD.printIDs();
    cout << "\n\n";
}

/*
==================================================
 Scooters
==================================================
*/

void UI::printAvailableScooters(
    PriQueue<Scooter*>& freeScooters)
{
    cout << "----------- Available Scooters -----------\n";

    cout << freeScooters.getCount()
        << " Scooters : ";

    freeScooters.printIDs();

    cout << "\n\n";
}

/*
==================================================
 Tables
==================================================
*/

void UI::printAvailableTables(
    Fit_Tables& freeTables)
{
    cout << "----------- Available Tables -----------\n";

    cout << freeTables.getCount()
        << " Tables : ";

    freeTables.printIDs();

    cout << "\n\n";
}

/*
==================================================
 In Service
==================================================
*/

void UI::printInService(
    PriQueue<Order*>& inService)
{
    cout << "----------- In Service -----------\n";

    Node<Order*>* current =
        inService.getFront();

    cout << inService.getCount()
        << " Orders : ";

    while (current)
    {
        Order* order = current->item;

        cout << "["
            << order->getID()
            << ", ";

        if (order->isDelivery())
            cout << "S"
            << order->getScooterID();
        else
            cout << "T"
            << order->getTableID();

        cout << "] ";

        current = current->next;
    }

    cout << "\n\n";
}

/*
==================================================
 Maintenance
==================================================
*/

void UI::printMaintenance(
    LinkedQueue<Scooter*>& maintenance)
{
    cout << "----------- Maintenance Scooters -----------\n";

    cout << maintenance.getCount()
        << " Scooters : ";

    maintenance.printIDs();

    cout << "\n\n";
}

/*
==================================================
 Returning Scooters
==================================================
*/

void UI::printBackScooters(
    PriQueue<Scooter*>& backScooters)
{
    cout << "----------- Back Scooters -----------\n";

    cout << backScooters.getCount()
        << " Scooters : ";

    backScooters.printIDs();

    cout << "\n\n";
}

/*
==================================================
 Cancelled
==================================================
*/

void UI::printCancelled(
    LinkedQueue<Order*>& cancelled)
{
    cout << "----------- Cancelled Orders -----------\n";

    cout << cancelled.getCount()
        << " Orders : ";

    cancelled.printIDs();

    cout << "\n\n";
}

/*
==================================================
 Finished
==================================================
*/

void UI::printFinished(
    Stack<Order*>& finished)
{
    cout << "----------- Finished Orders -----------\n";

    cout << finished.getCount()
        << " Orders : ";

    finished.printIDs();

    cout << "\n\n";
}

/*
==================================================
 Full Screen
==================================================
*/

void UI::printTimestep(
    int timestep,

    LinkedQueue<Request_Action*>& requests,
    LinkedQueue<Cancel_Action*>& cancels,

    LinkedQueue<Order*>& pendODG,
    LinkedQueue<Order*>& pendODN,
    LinkedQueue<Order*>& pendOT,
    LinkedQueue<Order*>& pendOVN,
    Pend_OVC& pendOVC,
    PriQueue<Order*>& pendOVG,

    LinkedQueue<Chef*>& freeCS,
    LinkedQueue<Chef*>& freeCN,

    Cook_Ords& cooking,

    LinkedQueue<Order*>& readyOT,
    RDY_OV& readyOV,
    LinkedQueue<Order*>& readyOD,

    PriQueue<Scooter*>& freeScooters,
    Fit_Tables& freeTables,

    PriQueue<Order*>& inService,

    LinkedQueue<Scooter*>& maintenance,
    PriQueue<Scooter*>& backScooters,

    Stack<Order*>& finished,
    LinkedQueue<Order*>& cancelled)
{
    cout << "\n========================================\n";
    cout << "Current Timestep : "
        << timestep << endl;
    cout << "========================================\n\n";

   /* printActions(requests, cancels);*/

    printPendingOrders(
        pendODG, pendODN,
        pendOT, pendOVN,
        pendOVC, pendOVG);

    printAvailableChefs(
        freeCS, freeCN);

    printCookingOrders(cooking);

    printReadyOrders(
        readyOT, readyOV, readyOD);

    printAvailableScooters(
        freeScooters);

    printAvailableTables(
        freeTables);

    printInService(inService);

    printMaintenance(maintenance);

    printBackScooters(backScooters);

    printCancelled(cancelled);

    printFinished(finished);
}