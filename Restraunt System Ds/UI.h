#pragma once
#include <iostream>

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

// Responsible for printing simulation status
class UI
{
public:

    void printActions(
        LinkedQueue<Request_Action*>& requests,
        LinkedQueue<Cancel_Action*>& cancels);

    void printPendingOrders(
        LinkedQueue<Order*>& pendODG,
        LinkedQueue<Order*>& pendODN,
        LinkedQueue<Order*>& pendOT,
        LinkedQueue<Order*>& pendOVN,
        Pend_OVC& pendOVC,
        PriQueue<Order*>& pendOVG);

    void printAvailableChefs(
        LinkedQueue<Chef*>& freeCS,
        LinkedQueue<Chef*>& freeCN);

    void printCookingOrders(
        Cook_Ords& cooking);

    void printReadyOrders(
        LinkedQueue<Order*>& readyOT,
        RDY_OV& readyOV,
        LinkedQueue<Order*>& readyOD);

    void printAvailableScooters(
        PriQueue<Scooter*>& freeScooters);

    void printAvailableTables(
        Fit_Tables& freeTables);

    void printInService(
        PriQueue<Order*>& inService);

    void printMaintenance(
        LinkedQueue<Scooter*>& maintenance);

    void printBackScooters(
        PriQueue<Scooter*>& backScooters);

    void printCancelled(
        LinkedQueue<Order*>& cancelled);

    void printFinished(
        Stack<Order*>& finished);

    void printTimestep(
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
        LinkedQueue<Order*>& cancelled);
};