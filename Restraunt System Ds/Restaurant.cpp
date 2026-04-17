#include "Restaurant.h"



void randomSimulate() {
    // Step 1: initialize — hardcode or randomly generate resources
    loadChefs();      // fill FREE_CS, FREE_CN
    loadScooters();   // fill FREE_SCOOTERS
    loadTables();     // fill FREE_TABLES

    // Step 2: randomly generate 500+ orders into pending lists
    generateRandomOrders();

    int timestep = 0;

    // Step 3: run until all orders are finished or cancelled
    while (!allDone()) {
        timestep++;

        // 3.1 repeat 30 times: pick pending order → assign random chef → COOKING
        for (int i = 0; i < 30; i++) {
            if (pendingEmpty()) break;
            Order* o = pickRandomPending();
            Chef* c = pickRandomChef();
            if (!o || !c) break;
            o->setTA(timestep);
            COOKING.enqueue(o, o->getSize());  // priority = size (cook time)
        }

        // 3.2 repeat 15 times with 75% chance: move cooking → ready
        for (int i = 0; i < 15; i++) {
            if ((rand() % 100) < 75 && !COOKING.isEmpty()) {
                Order* o = COOKING.dequeue();
                o->setTR(timestep);
                moveToReady(o);  // puts in RDY_OT / RDY_OV / RDY_OD based on type
            }
        }

        // 3.3 repeat 10 times: pick ready order → assign table/scooter → INSERVICE
        for (int i = 0; i < 10; i++) {
            if (readyEmpty()) break;
            Order* o = pickRandomReady();
            if (!o) break;
            o->setTS(timestep);
            if (o->getType() == OT) { FINISHED.push(o); }
            else if (o->getType() == OVG || o->getType() == OVN || o->getType() == OVC) {
                Scooter* s = FREE_SCOOTERS.dequeue();
                if (s) INSERVICE.enqueue(o, o->getDistance() / s->getSpeed());
            }
            else {  // OD
                Table* t = FREE_TABLES.getBest(o->getSeats());
                if (t) INSERVICE.enqueue(o, o->getDuration());
            }
        }

        // 3.4 cancel random OVC from PEND_OVC
        PEND_OVC.CancelOrder(rand() % 1000);

        // 3.5 cancel random OVC from RDY_OV
        RDY_OV.CancelOrder(rand() % 1000);

        // 3.6 cancel random OV from COOKING
        Order* cancelled = COOKING.CancelOrder(rand() % 1000);
        if (cancelled) CANCELLED.enqueue(cancelled);

        // 3.7 with 25%: move top inservice → finished
        if ((rand() % 100) < 25 && !INSERVICE.isEmpty()) {
            Order* o = INSERVICE.dequeue();
            o->setTF(timestep);
            if (isDelivery(o)) BACK_SCOOTERS.enqueue(scooterOf(o), o->getDistance());
            if (isDineIn(o))   FREE_TABLES.enqueue(tableOf(o), tableOf(o)->getFreeSeats());
            FINISHED.push(o);
        }

        // 3.8 with 50%: scooter from BACK → free or maintenance
        if ((rand() % 100) < 50 && !BACK_SCOOTERS.isEmpty()) {
            Scooter* s = BACK_SCOOTERS.dequeue();
            if (s->needsMaintenance()) MAINT_SCOOTERS.enqueue(s);
            else                       FREE_SCOOTERS.enqueue(s, 0);
        }

        // 3.9 with 50%: scooter from MAINT → free
        if ((rand() % 100) < 50 && !MAINT_SCOOTERS.isEmpty()) {
            Scooter* s = MAINT_SCOOTERS.dequeue();
            FREE_SCOOTERS.enqueue(s, 0);
        }

        // 3.10 print everything
        UI.printTimestep(timestep, ...all lists...);

        cout << "PRESS ANY KEY TO MOVE TO NEXT STEP!\n";
        cin.get();
    }
}

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


    }
}
bool Restaurant::CancelOrder(int id)
{
    if (PEND_OVC.Cancel_Order(id))
        return true;
    else
        return false;
}