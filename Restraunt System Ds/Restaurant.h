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
    LinkedQueue<Cancel_Action*> cancelList;

    // Pending Orders
    LinkedQueue<Order*> pendODG;
    LinkedQueue<Order*> pendODN;
    LinkedQueue<Order*> pendOT;
    LinkedQueue<Order*> pendOVN;
    Pend_OVC pendOVC;
    PriQueue<Order*> pendOVG;

    // Free Chefs
    LinkedQueue<Chef*> freeCS;
    LinkedQueue<Chef*> freeCN;

    // Cooking
    Cook_Ords cooking;

    // Ready
    LinkedQueue<Order*> readyOT;
    RDY_OV readyOV;
    LinkedQueue<Order*> readyOD;

    // In Service
    PriQueue<Order*> inService;

    // Scooters
    PriQueue<Scooter*> freeScooters;
    PriQueue<Scooter*> backScooters;
    LinkedQueue<Scooter*> maintenanceScooters;

    // Tables
    Fit_Tables freeTables;
    Fit_Tables busy;
    Fit_Tables busy_sharable;
    Fit_Tables busy_noshare;


    // Results
    Stack<Order*> finishedOrders;
    LinkedQueue<Order*> cancelledOrders;

    // Config
    int csCount, cnCount;
    int csSpeed, cnSpeed;
    int scooterCount, tableCount;

    bool interactiveMode;

    UI ui;

    bool pendingEmpty();
    bool readyEmpty();
    bool allDone();

    void moveToReady(Order* order);

public:

    Restaurant();

    void AddOrder(Order* order);
    void Cancel_Order(int id);

    void loadChefs();
    void loadScooters();
    void loadTables();

    void generateRandomOrders();

    void executeActions(int timestep);
    void assignToChefs(int timestep);
    void updateCooking(int timestep);
    void moveReadyToService(int timestep);
    void moveInServiceToFinished(int timestep);
    void updateScooters(int timestep);
    void updateTables(int timestep);

    void randomSimulate();

    LinkedQueue<Order*>& getPendODG();
    LinkedQueue<Order*>& getPendODN();
    LinkedQueue<Order*>& getPendOT();
    LinkedQueue<Order*>& getPendOVN();

    Pend_OVC& getPendOVC();
    PriQueue<Order*>& getPendOVG();

    LinkedQueue<Chef*>& getFreeCS();
    LinkedQueue<Chef*>& getFreeCN();

    Cook_Ords& getCooking();

    LinkedQueue<Order*>& getReadyOT();
    RDY_OV& getReadyOV();
    LinkedQueue<Order*>& getReadyOD();

    PriQueue<Scooter*>& getFreeScooters();
    PriQueue<Scooter*>& getBackScooters();
    LinkedQueue<Scooter*>& getMaintenance();

    Fit_Tables& getFreeTables();

    PriQueue<Order*>& getInService();

    Stack<Order*>& getFinished();
    LinkedQueue<Order*>& getCancelled();

    LinkedQueue<Request_Action*>& getRequests();
    LinkedQueue<Cancel_Action*>& getCancels();
};