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
#include "Action.h"
#include "UI.h"
#include <fstream>
#include <string>
using namespace std;

class Restaurant {
private:
    // ── 22 LISTS ──────────────────────────────────────

    // (1) Actions
    LinkedQueue<Action*>   ACTIONS_LIST;

    // (6) Pending orders
    LinkedQueue<Order*>    PEND_ODG;   // dine-in group
    LinkedQueue<Order*>    PEND_ODN;   // dine-in normal
    LinkedQueue<Order*>    PEND_OT;    // takeaway
    LinkedQueue<Order*>    PEND_OVN;   // delivery normal
    Pend_OVC               PEND_OVC;   // delivery cancel — derived, has CancelOrder()
    PriQueue<Order*>       PEND_OVG;   // delivery VIP

    // (2) Free chefs
    LinkedQueue<Chef*>     FREE_CS;    // senior chefs
    LinkedQueue<Chef*>     FREE_CN;    // normal chefs

    // (1) Cooking
    Cook_Ords              COOKING;    // derived, has CancelOrder()

    // (3) Ready orders
    LinkedQueue<Order*>    RDY_OT;     // ready takeaway
    RDY_OV                 RDY_OV_LIST;// ready delivery — derived, has CancelOrder()
    LinkedQueue<Order*>    RDY_OD;     // ready dine-in

    // (1) In-service
    PriQueue<Order*>       INSERVICE;

    // (3) Scooters
    PriQueue<Scooter*>     FREE_SCOOTERS;   // priority = shortest distance
    PriQueue<Scooter*>     BACK_SCOOTERS;   // priority = earliest return time
    LinkedQueue<Scooter*>  MAINT_SCOOTERS;  // FCFS maintenance queue

    // (3) Tables
    Fit_Tables             FREE_TABLES;     // derived, has getBest()
    Fit_Tables             BUSY_SHARABLE;   // tables shared by multiple orders
    Fit_Tables             BUSY_NOSHARE;    // tables not shared

    // (2) Done
    Stack<Order*>          FINISHED;        // LIFO — latest finished prints first
    LinkedQueue<Order*>    CANCELLED;       // FCFS cancelled orders

    // ── CONFIG DATA ───────────────────────────────────
    int    CS_count, CN_count;
    double CS_speed, CN_speed;
    int    scooter_count;
    int    table_count;

    UI     ui;                  // UI object for printing
    bool   interactiveMode;

    // ── PRIVATE HELPERS ───────────────────────────────

    // check if all lists are empty (simulation done)
    bool allDone() {
        return PEND_ODG.isEmpty() && PEND_ODN.isEmpty() &&
            PEND_OT.isEmpty() && PEND_OVN.isEmpty() &&
            PEND_OVC.isEmpty() && PEND_OVG.isEmpty() &&
            COOKING.isEmpty() &&
            RDY_OT.isEmpty() && RDY_OV_LIST.isEmpty() && RDY_OD.isEmpty() &&
            INSERVICE.isEmpty() &&
            ACTIONS_LIST.isEmpty();
    }

    bool pendingEmpty() {
        return PEND_ODG.isEmpty() && PEND_ODN.isEmpty() &&
            PEND_OT.isEmpty() && PEND_OVN.isEmpty() &&
            PEND_OVC.isEmpty() && PEND_OVG.isEmpty();
    }

    bool readyEmpty() {
        return RDY_OT.isEmpty() && RDY_OV_LIST.isEmpty() && RDY_OD.isEmpty();
    }

    // route ready order to correct ready list
    void moveToReady(Order* o) {
        ORD_TYPE t = o->getType();
        if (t == OT)
            RDY_OT.enqueue(o);
        else if (t == OVG || t == OVN || t == OVC)
            RDY_OV_LIST.enqueue(o);
        else  // ODG, ODN
            RDY_OD.enqueue(o);
    }

    // check if order is delivery type
    bool isDelivery(Order* o) {
        ORD_TYPE t = o->getType();
        return (t == OVG || t == OVN || t == OVC);
    }

    // check if order is dine-in type
    bool isDineIn(Order* o) {
        ORD_TYPE t = o->getType();
        return (t == ODG || t == ODN);
    }

public:
    // ── CONSTRUCTORS ─────────────────────────────────
    Restaurant() : CS_count(0), CN_count(0),
        CS_speed(0), CN_speed(0),
        scooter_count(0), table_count(0),
        interactiveMode(true) {
    }

    // ── GETTERS (needed by Action classes) ───────────
    LinkedQueue<Order*>& getPendODG() { return PEND_ODG; }
    LinkedQueue<Order*>& getPendODN() { return PEND_ODN; }
    LinkedQueue<Order*>& getPendOT() { return PEND_OT; }
    LinkedQueue<Order*>& getPendOVN() { return PEND_OVN; }
    Pend_OVC& getPendOVC() { return PEND_OVC; }
    PriQueue<Order*>& getPendOVG() { return PEND_OVG; }
    Cook_Ords& getCooking() { return COOKING; }
    RDY_OV& getRdyOV() { return RDY_OV_LIST; }
    LinkedQueue<Order*>& getCancelled() { return CANCELLED; }

    // ── PHASE 1.2: LOAD RESOURCES ────────────────────

    void loadChefs() {
        // create CS chefs and add to FREE_CS
        for (int i = 1; i <= CS_count; i++) {
            Chef* c = new Chef(i, "CS", CS_speed);
            FREE_CS.enqueue(c);
        }
        // create CN chefs and add to FREE_CN
        for (int i = CS_count + 1; i <= CS_count + CN_count; i++) {
            Chef* c = new Chef(i, "CN", CN_speed);
            FREE_CN.enqueue(c);
        }
    }

    void loadScooters() {
        // for phase 1.2 hardcode some scooters
        for (int i = 1; i <= scooter_count; i++) {
            // speed=2, maintenance every 5 orders, maintenance takes 3 timesteps
            Scooter* s = new Scooter(i, 2.0, 5, 3);
            FREE_SCOOTERS.enqueue(s, 0); // priority=0 initially (no distance yet)
        }
    }

    void loadTables() {
        // for phase 1.2 hardcode tables with different capacities
        int caps[] = { 2, 2, 4, 4, 6, 6, 8 };
        for (int i = 1; i <= table_count && i <= 7; i++) {
            Table* t = new Table(i, caps[i - 1]);
            FREE_TABLES.enqueue(t, caps[i - 1]); // priority = capacity
        }
    }

    // ── PHASE 1.2: RANDOM GENERATOR ──────────────────

    void generateRandomOrders() {
        // generate 500+ random orders into pending lists
        int types[] = { ODG, ODN, OT, OVN, OVC, OVG };
        for (int i = 1; i <= 500; i++) {
            ORD_TYPE t = (ORD_TYPE)(types[rand() % 6]);
            int  tq = rand() % 100 + 1;
            int  size = rand() % 10 + 1;
            double price = (rand() % 200 + 50) * 1.0;

            Order* o = nullptr;

            if (t == ODG || t == ODN) {
                int seats = rand() % 6 + 2;         // 2-8 seats
                int dur = rand() % 20 + 5;         // 5-25 timesteps
                bool share = (rand() % 2 == 0);
                Order* o = new Order(i, t, tq, size, price, seats, dur, share);
            }

            else if (t == OVG || t == OVN || t == OVC) {
                double dist = (rand() % 50 + 5) * 1.0;
                Order* o = new Order(i, t, tq, size, price, dist);
            }
            else
				Order* o = new Order(i, t, tq, size, price);

            // push to correct pending list
            switch (t) {
            case ODG: PEND_ODG.enqueue(o);              break;
            case ODN: PEND_ODN.enqueue(o);              break;
            case OT:  PEND_OT.enqueue (o);               break;
            case OVN: PEND_OVN.enqueue(o);              break;
            case OVC: PEND_OVC.enqueue(o);              break;
            case OVG: PEND_OVG.enqueue(o, (int)price);  break;
            }
        }
    }

    // ── PHASE 1.2: RANDOM SIMULATE ───────────────────

    void randomSimulate() {
        // setup resources (hardcoded for phase 1.2)
        CS_count = 15; CN_count = 20;
        CS_speed = 3.0; CN_speed = 2.0;
        scooter_count = 5; table_count = 7;

        loadChefs();
        loadScooters();
        loadTables();
        generateRandomOrders();

        ui.readMode(interactiveMode);

        int timestep = 0;

        while (!allDone()) {
            timestep++;

            // ── STEP 3.1: assign pending → cooking (up to 30 times) ──
            for (int i = 0; i < 30 && !pendingEmpty() &&
                (!FREE_CS.isEmpty() || !FREE_CN.isEmpty()); i++) {

                // pick a random non-empty pending list
                Order* o = nullptr;
                int pick = rand() % 6;
                if (pick == 0 && !PEND_ODG.isEmpty()) o = PEND_ODG.dequeue();
                else if (pick == 1 && !PEND_ODN.isEmpty()) o = PEND_ODN.dequeue();
                else if (pick == 2 && !PEND_OT.isEmpty())  o = PEND_OT.dequeue();
                else if (pick == 3 && !PEND_OVN.isEmpty()) o = PEND_OVN.dequeue();
                else if (pick == 4 && !PEND_OVC.isEmpty()) o = PEND_OVC.dequeue();
                else if (pick == 5 && !PEND_OVG.isEmpty()) o = PEND_OVG.dequeue();
                if (!o) continue;

                // pick random chef (CS or CN)
               /* Chef* c = nullptr;
                if (!FREE_CS.isEmpty() && !FREE_CN.isEmpty())
                    c = (rand() % 2 == 0) ? FREE_CS.dequeue() : FREE_CN.dequeue();
                else if (!FREE_CS.isEmpty()) c = FREE_CS.dequeue();
                else if (!FREE_CN.isEmpty()) c = FREE_CN.dequeue();*/
                   
				


                o->setTA(timestep);
                // priority in cooking = cook time = ceil(size / chefSpeed)
                int cookTime = (int)ceil(o->getSize() / c->getSpeed());
                COOKING.enqueue(o, cookTime);
                c->setBusy(true);
                // NOTE: in phase 2 we track which chef got which order
            }

            // ── STEP 3.2: cooking → ready (75% chance, up to 15 times) ──
            for (int i = 0; i < 15; i++) {
                if (COOKING.isEmpty()) break;
                if ((rand() % 100) < 75) {
                    Order* o = COOKING.dequeue();
                    o->setTR(timestep);
                    moveToReady(o);
                }
            }

            // ── STEP 3.3: ready → inservice (up to 10 times) ──
            for (int i = 0; i < 10 && !readyEmpty(); i++) {
                Order* o = nullptr;
                if (!RDY_OT.isEmpty())       o = RDY_OT.dequeue();
                else if (!RDY_OV_LIST.isEmpty())  o = RDY_OV_LIST.dequeue();
                else if (!RDY_OD.isEmpty())        o = RDY_OD.dequeue();
                if (!o) break;

                o->setTS(timestep);

                if (o->getType() == OT) {
                    // takeaway: done immediately
                    o->setTF(timestep);
                    FINISHED.push(o);
                }
                else if (isDelivery(o)) {
                    // assign scooter
                    if (!FREE_SCOOTERS.isEmpty()) {
                        Scooter* s = FREE_SCOOTERS.dequeue();
                        int delivTime = (int)ceil(o->getDistance() / s->getSpeed());
                        INSERVICE.enqueue(o, delivTime);
                        // NOTE: in phase 2 track scooter per order
                    }
                }
                else { // dine-in
                    // assign table
                    Table* t = FREE_TABLES.getBest(o->getSeats());
                    if (t) {
                        t->reserveSeats(o->getSeats(), o->getDuration(),
                            timestep, o->getCanShare());
                        INSERVICE.enqueue(o, o->getDuration());
                    }
                }
            }

            // ── STEP 3.4: cancel random OVC from PEND_OVC ──
            int cancelID = rand() % 1000;
             //////////////    abdallah hesham doesnot make yet 

            if (PEND_OVC.CancelOrder(cancelID)) {
                // find the order and add to CANCELLED
                // (simplified for phase 1.2)
            }

            // ── STEP 3.5: cancel random OV from RDY_OV ──
            RDY_OV_LIST.CancelOrder(rand() % 1000);


            // ── STEP 3.6: cancel random OV from COOKING ──
            COOKING.CancelOrder(rand() % 1000);

            // ── STEP 3.7: inservice → finished (25% chance) ──
            if ((rand() % 100) < 25 && !INSERVICE.isEmpty()) {
                Order* o = INSERVICE.dequeue();
                o->setTF(timestep);

                if (isDelivery(o) && !BACK_SCOOTERS.isEmpty()) {
                    // scooter comes back after delivery
                    // priority = return time
                    Scooter* s = BACK_SCOOTERS.dequeue(); // simplified
                    FREE_SCOOTERS.enqueue(s, (int)s->getDistanceCut());
                }
                if (isDineIn(o)) {
                    // free the table
                    // in phase 2: track which table was assigned
                }
                FINISHED.push(o);
            }

            // ── STEP 3.8: scooter back → free or maintenance (50%) ──
            if ((rand() % 100) < 50 && !BACK_SCOOTERS.isEmpty()) {
                Scooter* s = BACK_SCOOTERS.dequeue();
                s->incrementOrders();
                if (s->needsMaintenance()) {
                    s->resetOrdCount();
                    MAINT_SCOOTERS.enqueue(s);
                }
                else {
                    FREE_SCOOTERS.enqueue(s, (int)s->getDistanceCut());
                }
            }

            // ── STEP 3.9: maintenance → free (50%) ──
            if ((rand() % 100) < 50 && !MAINT_SCOOTERS.isEmpty()) {
                Scooter* s = MAINT_SCOOTERS.dequeue();
                FREE_SCOOTERS.enqueue(s, (int)s->getDistanceCut());
            }

            // ── STEP 3.10: print current state ──
            ui.printTimestep(timestep,
                ACTIONS_LIST, PEND_ODG, PEND_ODN, PEND_OT,
                PEND_OVN, PEND_OVC, PEND_OVG,
                FREE_CS, FREE_CN, COOKING,
                RDY_OT, RDY_OV_LIST, RDY_OD,
                FREE_SCOOTERS, FREE_TABLES, INSERVICE,
                MAINT_SCOOTERS, BACK_SCOOTERS,
                FINISHED, CANCELLED);

            // interactive mode: wait for user keypress
            if (interactiveMode) {
                cout << "\nPress ENTER to move to next timestep...\n";
                cin.ignore();
                cin.get();
            }
        }

        cout << "\n=== SIMULATION DONE at timestep " << timestep << " ===\n";
       
    }

   
};


