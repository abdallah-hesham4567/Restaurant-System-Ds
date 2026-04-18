#pragma once

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
#include <fstream>
#include <string>
#include <cmath>
using namespace std;

class Restaurant {
private:
    // ── 22 LISTS ──────────────────────────────────────
    LinkedQueue<Request_Action*>  RACTIONS_LIST;
    LinkedQueue<Cancel_Action*>   CACTIONS_LIST;
    LinkedQueue<Order*>   PEND_ODG;
    LinkedQueue<Order*>   PEND_ODN;
    LinkedQueue<Order*>   PEND_OT;
    LinkedQueue<Order*>   PEND_OVN;
    Pend_OVC              PEND_OVC;
    PriQueue<Order*>      PEND_OVG;
    LinkedQueue<Chef*>    FREE_CS;
    LinkedQueue<Chef*>    FREE_CN;
    Cook_Ords             COOKING;
    LinkedQueue<Order*>   RDY_OT;
    RDY_OV                RDY_OV_LIST;
    LinkedQueue<Order*>   RDY_OD;
    PriQueue<Order*>      INSERVICE;
    PriQueue<Scooter*>    FREE_SCOOTERS;
    PriQueue<Scooter*>    BACK_SCOOTERS;
    LinkedQueue<Scooter*> MAINT_SCOOTERS;
    Fit_Tables            FREE_TABLES;
    Fit_Tables            BUSY_SHARABLE;
    Fit_Tables            BUSY_NOSHARE;
    Stack<Order*>         FINISHED;
    LinkedQueue<Order*>   CANCELLED;

    // ── CONFIG ────────────────────────────────────────
    int    CS_count, CN_count;
    double CS_speed, CN_speed;
    int    scooter_count, table_count;
    UI     ui;
    bool   interactiveMode;

    // ── INLINE HELPERS (too small for .cpp) ───────────
   
    bool allDone() {
        return PEND_ODG.isEmpty() &&
            PEND_ODN.isEmpty() &&
            PEND_OT.isEmpty() &&
            PEND_OVN.isEmpty() &&
            PEND_OVC.isEmpty() &&
            PEND_OVG.isEmpty() &&
            COOKING.isEmpty() &&
            RDY_OT.isEmpty() &&
            RDY_OV_LIST.isEmpty() &&
            RDY_OD.isEmpty() &&
            INSERVICE.isEmpty() &&
            BACK_SCOOTERS.isEmpty() &&
            MAINT_SCOOTERS.isEmpty() &&
            RACTIONS_LIST.isEmpty() &&
            CACTIONS_LIST.isEmpty();
    }









    bool pendingEmpty() {
        return PEND_ODG.isEmpty() && PEND_ODN.isEmpty() &&
            PEND_OT.isEmpty() && PEND_OVN.isEmpty() &&
            PEND_OVC.isEmpty() && PEND_OVG.isEmpty();
    }

    bool readyEmpty() {
        return RDY_OT.isEmpty() && RDY_OV_LIST.isEmpty() && RDY_OD.isEmpty();
    }

    bool isDelivery(Order* o) {
        ORD_TYPE t = o->getType();
        return (t == OVG || t == OVN || t == OVC);
    }

    bool isDineIn(Order* o) {
        ORD_TYPE t = o->getType();
        return (t == ODG || t == ODN);
    }

    void moveToReady(Order* o) {
        ORD_TYPE t = o->getType();
        if (t == OT)                          RDY_OT.enqueue(o);
        else if (t == OVG || t == OVN || t == OVC) RDY_OV_LIST.enqueue(o);
        else                                        RDY_OD.enqueue(o);
    }

public:
    Restaurant();

    // ── ACTION HELPERS ────────────────────────────────
    void AddOrder(Order* ord);
    void Cancel_Order(int id);

    // ── GETTERS ───────────────────────────────────────
    LinkedQueue<Order*>& getPendODG();
    LinkedQueue<Order*>& getPendODN();
    LinkedQueue<Order*>& getPendOT();
    LinkedQueue<Order*>& getPendOVN();
    Pend_OVC& getPendOVC();
    PriQueue<Order*>& getPendOVG();
    Cook_Ords& getCooking();
    RDY_OV& getRdyOV();
    LinkedQueue<Order*>& getCancelled();

    // ── PHASE 1.2 ─────────────────────────────────────
    void loadChefs();
    void loadScooters();
    void loadTables();
    void generateRandomOrders();
    void randomSimulate();

    // ── PHASE 2 ───────────────────────────────────────
    void loadFromFile(string filename);
    void executeActions(int t);
    void assignToChefs(int t);
    void updateCooking(int t);
    void assignToService(int t);
    void updateScooters(int t);
    void updateTables(int t);
    void simulate(string inputFile, string outputFile);
    void produceOutput(string filename);
};
