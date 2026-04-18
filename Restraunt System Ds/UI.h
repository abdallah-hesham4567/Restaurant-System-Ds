#pragma once
#include <iostream>
#include <string>
using namespace std;

#include "LinkedQueue.h"
#include "Pri_Queue.h"
#include "Stack.h"
#include "Pend_OVC.h"
#include "Cook_Ords.h"
#include "RDY_OV.h"
#include "Fit_Tables.h"
#include "Order.h"
#include "Chef.h"
#include "Scooter.h"
#include "Table.h"
#include "Request_Action.h"
#include "Cancel_Action.h"

class UI {
public:

    // ── each section has its own print function ──

    void printActions(
        LinkedQueue<Request_Action*>& Ractions,
        LinkedQueue<Cancel_Action*>& Cactions
    );

    void printPendingOrders(
        LinkedQueue<Order*>& pendODG,
        LinkedQueue<Order*>& pendODN,
        LinkedQueue<Order*>& pendOT,
        LinkedQueue<Order*>& pendOVN,
        Pend_OVC& pendOVC,
        PriQueue<Order*>& pendOVG
    );

    void printAvailableChefs(
        LinkedQueue<Chef*>& freeCS,
        LinkedQueue<Chef*>& freeCN
    );

    void printCookingOrders(Cook_Ords& cooking);

    void printReadyOrders(
        LinkedQueue<Order*>& rdyOT,
        RDY_OV& rdyOV,
        LinkedQueue<Order*>& rdyOD
    );

    void printAvailableScooters(PriQueue<Scooter*>& freeScooters);

    void printAvailableTables(Fit_Tables& freeTables);

    void printInService(PriQueue<Order*>& inService);

    void printMaintenanceScooters(LinkedQueue<Scooter*>& maintScooters);

    void printBackScooters(PriQueue<Scooter*>& backScooters);

    void printCancelledOrders(LinkedQueue<Order*>& cancelled);

    void printFinishedOrders(Stack<Order*>& finished);

    // ── master print: calls all above in order ──
    void printTimestep(
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
        LinkedQueue<Order*>& cancelled
    );
};