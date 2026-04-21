#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <cstdlib>
#include <ctime>

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

#include "UI.h"

class Restaurant
{
private:

    // Actions
    LinkedQueue<Request_Action*> requestList;
    LinkedQueue<Cancel_Action*>  cancelList;

    // Pending Orders
    LinkedQueue<Order*> pendODG;
    LinkedQueue<Order*> pendODN;
    LinkedQueue<Order*> pendOT;
    LinkedQueue<Order*> pendOVN;
    Pend_OVC            pendOVC;
    PriQueue<Order*>    pendOVG;

    // Free Chefs
    LinkedQueue<Chef*> freeCS;
    LinkedQueue<Chef*> freeCN;

    // Cooking
    Cook_Ords cooking;

    // Ready
    LinkedQueue<Order*> readyOT;
    RDY_OV              readyOV;
    LinkedQueue<Order*> readyOD;

    // In Service
    PriQueue<Order*> inService;

    // Scooters
    PriQueue<Scooter*>    freeScooters;
    PriQueue<Scooter*>    backScooters;
    LinkedQueue<Scooter*> maintenanceScooters;

    // Tables
    Fit_Tables freeTables;
    Fit_Tables busy_sharable;
    Fit_Tables busy_noshare;

    // Results
    Stack<Order*>        finishedOrders;
    LinkedQueue<Order*>  cancelledOrders;

    // Config
    int csCount, cnCount;
    int csSpeed, cnSpeed;
    int scooterCount, scooterSpeed;
    int maintOrders, maintDuration;
    int tableCount;
    int TH;                 // overwait threshold
    bool interactiveMode;

    // Statistics helpers
    int totalSimTime;

    UI ui;

    // --- helpers ---
    bool pendingEmpty();
    bool readyEmpty();
    bool allDone();

    void moveToReady(Order* order);

    // --- Phase 2 internal steps ---
    void processActions(int timestep);   // Feature 3 & 4
    void assignToChefs(int timestep);    // Feature 8
    void updateCooking(int timestep);    // Feature 9
    void moveReadyToService(int timestep);       // Features 10,11,12
    void moveInServiceToFinished(int timestep);  // Features 6 & 7
    void updateScooters(int timestep);   // Feature 5
    void updateTables(int timestep);

    void generateOutputFile();           // Feature 13

public:

    Restaurant();

    void AddOrder(Order* order);
    void Cancel_Order(int id);

    void loadChefs();
    void loadScooters();
    void loadTables();

    // Feature 2
    bool loadFromFile(const string& filename);

    // Feature 1 - main simulation loop
    void simulate();
};