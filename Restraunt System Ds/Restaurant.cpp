#include "Restaurant.h"

// ── CONSTRUCTOR ───────────────────────────────────────────────────────────────
Restaurant::Restaurant()
    : CS_count(0), CN_count(0),
    CS_speed(0), CN_speed(0),
    scooter_count(0), table_count(0),
    interactiveMode(true) {
}

// ── GETTERS ───────────────────────────────────────────────────────────────────
LinkedQueue<Order*>& Restaurant::getPendODG() { return PEND_ODG; }
LinkedQueue<Order*>& Restaurant::getPendODN() { return PEND_ODN; }
LinkedQueue<Order*>& Restaurant::getPendOT() { return PEND_OT; }
LinkedQueue<Order*>& Restaurant::getPendOVN() { return PEND_OVN; }
Pend_OVC& Restaurant::getPendOVC() { return PEND_OVC; }
PriQueue<Order*>& Restaurant::getPendOVG() { return PEND_OVG; }
Cook_Ords& Restaurant::getCooking() { return COOKING; }
RDY_OV& Restaurant::getRdyOV() { return RDY_OV_LIST; }
LinkedQueue<Order*>& Restaurant::getCancelled() { return CANCELLED; }

// ── ADD ORDER ─────────────────────────────────────────────────────────────────
void Restaurant::AddOrder(Order* ord)
{
    switch (ord->getType()) {
    case ODG: PEND_ODG.enqueue(ord);                          break;
    case ODN: PEND_ODN.enqueue(ord);                          break;
    case OT:  PEND_OT.enqueue(ord);                           break;
    case OVN: PEND_OVN.enqueue(ord);                          break;
    case OVG: PEND_OVG.enqueue(ord, (int)ord->getPriority()); break;
    case OVC: PEND_OVC.enqueue(ord);                          break;
    }
}

// ── CANCEL ORDER ──────────────────────────────────────────────────────────────
void Restaurant::Cancel_Order(int id)
{
    Order* found = nullptr;

    found = PEND_OVC.CancelAndReturn(id);
    if (found) { CANCELLED.enqueue(found); return; }

    found = COOKING.CancelAndReturn(id);
    if (found) {
        Chef* c = found->getChef();
        if (c) {
            c->setBusy(false);
            if (c->getType() == "CS") FREE_CS.enqueue(c);
            else                       FREE_CN.enqueue(c);
        }
        CANCELLED.enqueue(found);
        return;
    }

    found = RDY_OV_LIST.CancelAndReturn(id);
    if (found) { CANCELLED.enqueue(found); return; }
}

// ── LOAD RESOURCES ────────────────────────────────────────────────────────────
void Restaurant::loadChefs() {
    for (int i = 1; i <= CS_count; i++)
        FREE_CS.enqueue(new Chef(i, "CS", CS_speed));
    for (int i = CS_count + 1; i <= CS_count + CN_count; i++)
        FREE_CN.enqueue(new Chef(i, "CN", CN_speed));
}

void Restaurant::loadScooters() {
    for (int i = 1; i <= scooter_count; i++)
        FREE_SCOOTERS.enqueue(new Scooter(i, 2.0, 5, 3), 0);
}

void Restaurant::loadTables() {
    int caps[] = { 2, 2, 4, 4, 6, 6, 8 };
    int n = (table_count < 7) ? table_count : 7;
    for (int i = 1; i <= n; i++)
        FREE_TABLES.enqueue(new Table(i, caps[i - 1]), caps[i - 1]);
}

// ── FIX 1: generateRandomOrders ───────────────────────────────────────────────
// BUG: int t = rand()%6 was used as ORD_TYPE index but passed to switch as int
//      so all orders became ODG=0 (first enum value)
// FIX: use explicit ORD_TYPE array and index into it



void Restaurant::generateRandomOrders() {
    ORD_TYPE types[] = { ODG, ODN, OT, OVN, OVC, OVG }; // explicit enum array
    int orderID = 1;

    for (int t = 1; t <= 100; t++) {
        int count = rand() % 3 + 1;
        for (int j = 0; j < count; j++) {
            
            int      sz = rand() % 10 + 1;
            double   pr = (rand() % 200 + 50) * 1.0;

            int randomtype = rand() % 6;
            Request_Action* a = nullptr;
            switch (randomtype)
            {
            case(0):  a = new Request_Action(ODG, t, orderID++, sz, pr);
				break;
			case(1):  a = new Request_Action(ODN, t, orderID++, sz, pr);
				break;
			case(2):  a = new Request_Action(OT, t, orderID++, sz, pr);
				break;
			case(3):  a = new Request_Action(OVN, t, orderID++, sz, pr);
				break;
			case(4):  a = new Request_Action(OVC, t, orderID++, sz, pr);
				break;
			case(5):  a = new Request_Action(OVG, t, orderID++, sz, pr);
				break;

            }

            if (randomtype == 0 || randomtype == 1) {
                int  seats = rand() % 6 + 2;
                int  dur = rand() % 20 + 5;
                bool share = (rand() % 2 == 0);
                a->SetOD(seats, dur, share);
            }
            else if (randomtype == 5 || randomtype == 3 || randomtype == 4) {
                int dist = rand() % 50 + 5;
                a->SetOV(dist);
            }
            // OT needs nothing extra

            RACTIONS_LIST.enqueue(a);
        }
        
    }
}

// ── EXECUTE ACTIONS ───────────────────────────────────────────────────────────
void Restaurant::executeActions(int t) {
    while (!RACTIONS_LIST.isEmpty() &&
        RACTIONS_LIST.peek()->get_TimeStep() == t) {
        Request_Action* a = RACTIONS_LIST.dequeue();
        a->Act(this);
    }
    while (!CACTIONS_LIST.isEmpty() &&
        CACTIONS_LIST.peek()->get_TimeStep() == t) {
        Cancel_Action* a = CACTIONS_LIST.dequeue();
        a->Act(this);
    }
}

// ── STEP 3.1: PENDING → COOKING ──────────────────────────────────────────────
void Restaurant::assignToChefs(int t) {
    for (int i = 0; i < 30 && !pendingEmpty() &&
        (!FREE_CS.isEmpty() || !FREE_CN.isEmpty()); i++) {

        Order* o = nullptr;
        int    pick = rand() % 6;
        if (pick == 0 && !PEND_ODG.isEmpty()) o = PEND_ODG.dequeue();
        else if (pick == 1 && !PEND_ODN.isEmpty()) o = PEND_ODN.dequeue();
        else if (pick == 2 && !PEND_OT.isEmpty())  o = PEND_OT.dequeue();
        else if (pick == 3 && !PEND_OVN.isEmpty()) o = PEND_OVN.dequeue();
        else if (pick == 4 && !PEND_OVC.isEmpty()) o = PEND_OVC.dequeue();
        else if (pick == 5 && !PEND_OVG.isEmpty()) o = PEND_OVG.dequeue();
        if (!o) continue;

        Chef* c = nullptr;
        if (!FREE_CS.isEmpty() && !FREE_CN.isEmpty())
            c = (rand() % 2 == 0) ? FREE_CS.dequeue() : FREE_CN.dequeue();
        else if (!FREE_CS.isEmpty()) c = FREE_CS.dequeue();
        else                          c = FREE_CN.dequeue();

        o->setTA(t);
        o->setChef(c);
        o->setChefID(c->getID());
        c->setBusy(true);

        int cookTime = (int)ceil((double)o->getSize() / c->getSpeed());
        o->setFinishCookTime(t + cookTime);
        COOKING.enqueue(o, cookTime);
    }
}

// ── STEP 3.2: COOKING → READY ────────────────────────────────────────────────
void Restaurant::updateCooking(int t) {
    for (int i = 0; i < 15; i++) {
        if (COOKING.isEmpty()) break;
        Order* top = COOKING.peek();
        if (top->getFinishCookTime() > t) break;

        Order* o = COOKING.dequeue();
        o->setTR(t);

        Chef* c = o->getChef();
        if (c) {
            c->setBusy(false);
            if (c->getType() == "CS") FREE_CS.enqueue(c);
            else                       FREE_CN.enqueue(c);
        }
        moveToReady(o);
    }
}

// ── STEP 3.3: READY → IN-SERVICE ─────────────────────────────────────────────
void Restaurant::assignToService(int t) {
    for (int i = 0; i < 10 && !readyEmpty(); i++) {
        Order* o = nullptr;
        if (!RDY_OT.isEmpty())      o = RDY_OT.dequeue();
        else if (!RDY_OV_LIST.isEmpty()) o = RDY_OV_LIST.dequeue();
        else if (!RDY_OD.isEmpty())       o = RDY_OD.dequeue();
        if (!o) break;

        o->setTS(t);

        if (o->getType() == OT) {
            o->setTF(t);
            FINISHED.push(o);
        }
        else if (o->isDelivery()) {
            if (!FREE_SCOOTERS.isEmpty()) {
                Scooter* s = FREE_SCOOTERS.dequeue();
                int returnT = t + (int)ceil((double)o->getDistance() / s->getSpeed());

                o->setScooter(s);
                o->setScooterID(s->getID());
                o->setFinishServiceTime(returnT);
                s->addDistance(o->getDistance());

                INSERVICE.enqueue(o, returnT);
                // FIX: scooter goes to BACK with return time as priority
                BACK_SCOOTERS.enqueue(s, returnT);
            }
        }
        else { // dine-in
            Table* tbl = FREE_TABLES.getBest(o->getSeats());
            if (tbl) {
                int finishT = t + o->getDuration();
                tbl->reserveSeats(o->getSeats(), o->getDuration(), t, o->getCanShare());
                o->setTable(tbl);
                o->setTableID(tbl->getID());
                o->setFinishServiceTime(finishT);

                if (tbl->getFreeSeats() > 0 && o->getCanShare())
                    BUSY_SHARABLE.enqueue(tbl, tbl->getFreeSeats());
                else
                    BUSY_NOSHARE.enqueue(tbl, tbl->getFreeSeats());

                INSERVICE.enqueue(o, finishT);
            }
        }
    }
}

// ── FIX 2: updateScooters — 3 stages every timestep ──────────────────────────
// BUG: BACK_SCOOTERS was filled but never processed → scooters stuck forever
// FIX: check BACK priority <= t → move to FREE or MAINT
//      check MAINT mainEndTime <= t → move to FREE
void Restaurant::updateScooters(int t) {

    // STAGE 1: finish INSERVICE orders whose time is up
    while (!INSERVICE.isEmpty() &&
        INSERVICE.peek()->getFinishServiceTime() <= t) {
        Order* o = INSERVICE.dequeue();
        o->setTF(t);

        if (o->isDineIn()) {
            Table* tbl = o->getTable();
            if (tbl) {
                tbl->releaseSeats(o->getSeats());
                BUSY_SHARABLE.removeTable(tbl->getID());
                BUSY_NOSHARE.removeTable(tbl->getID());
                FREE_TABLES.enqueue(tbl, tbl->getFreeSeats());
            }
        }
        FINISHED.push(o);
    }

    // STAGE 2: scooters whose return time <= t → FREE or MAINTENANCE
    while (!BACK_SCOOTERS.isEmpty() &&
        BACK_SCOOTERS.peek()->getMainEndTime() <= t) {
        Scooter* s = BACK_SCOOTERS.dequeue();
        s->incrementOrders();
        if (s->needsMaintenance()) {
            s->resetOrdCount();
            s->setMainEndTime(t + s->getMainDur());
            MAINT_SCOOTERS.enqueue(s);
        }
        else {
            FREE_SCOOTERS.enqueue(s, (int)s->getDistanceCut());
        }
    }

    // STAGE 3: maintenance scooters whose end time <= t → FREE
    while (!MAINT_SCOOTERS.isEmpty() &&
        MAINT_SCOOTERS.peek()->getMainEndTime() <= t) {
        Scooter* s = MAINT_SCOOTERS.dequeue();
        FREE_SCOOTERS.enqueue(s, (int)s->getDistanceCut());
    }
}

void Restaurant::updateTables(int t) {
    // tables freed inside updateScooters STAGE 1
    // reserved for Phase 2 sharable table logic
}

// ── RANDOM SIMULATE ───────────────────────────────────────────────────────────
void Restaurant::randomSimulate() {
    CS_count = 3; CN_count = 4;
    CS_speed = 3.0; CN_speed = 2.0;
    scooter_count = 5; table_count = 7;

    loadChefs();
    loadScooters();
    loadTables();
    generateRandomOrders();

    int timestep = 0;
    while (!allDone()) {
        timestep++;
        if (timestep == 300) break;

        executeActions(timestep);
        assignToChefs(timestep);
        updateCooking(timestep);
        assignToService(timestep);

        if (!PEND_OVC.isEmpty())
            Cancel_Order(PEND_OVC.getRandomID());

        if (!RDY_OV_LIST.isEmpty()) {
            Order* f = RDY_OV_LIST.CancelAndReturn(RDY_OV_LIST.getRandomID());
            if (f) CANCELLED.enqueue(f);
        }

        if (!COOKING.isEmpty()) {
            Order* f = COOKING.CancelAndReturn(COOKING.getRandomID());
            if (f) {
                Chef* c = f->getChef();
                if (c) {
                    c->setBusy(false);
                    if (c->getType() == "CS") FREE_CS.enqueue(c);
                    else                       FREE_CN.enqueue(c);
                }
                CANCELLED.enqueue(f);
            }
        }

        updateScooters(timestep);
        updateTables(timestep);

        ui.printTimestep(timestep,
            RACTIONS_LIST, CACTIONS_LIST,
            PEND_ODG, PEND_ODN, PEND_OT, PEND_OVN, PEND_OVC, PEND_OVG,
            FREE_CS, FREE_CN, COOKING,
            RDY_OT, RDY_OV_LIST, RDY_OD,
            FREE_SCOOTERS, FREE_TABLES, INSERVICE,
            MAINT_SCOOTERS, BACK_SCOOTERS,
            FINISHED, CANCELLED);

        if (interactiveMode) { cout << "Press ENTER...\n"; cin.get(); }
    }
    cout << "\n=== SIMULATION DONE at timestep " << timestep << " ===\n";
}

// ── PHASE 2 ───────────────────────────────────────────────────────────────────
void Restaurant::loadFromFile(string filename) {
    ifstream fin(filename);
    if (!fin) { cout << "Error opening " << filename << "\n"; return; }
    fin >> CS_count >> CN_count >> CS_speed >> CN_speed;
    fin >> scooter_count >> table_count;
    loadChefs(); loadScooters(); loadTables();
    char type;
    while (fin >> type) {
        if (type == 'Q') {
            string ots; int time, id, sz; double pr;
            fin >> ots >> time >> id >> sz >> pr;
            ORD_TYPE ot = ODG;
            if (ots == "ODN") ot = ODN;
            else if (ots == "OT")  ot = OT;
            else if (ots == "OVN") ot = OVN;
            else if (ots == "OVC") ot = OVC;
            else if (ots == "OVG") ot = OVG;
            Request_Action* a = new Request_Action(ot, time, id, sz, pr);
            if (ot == ODG || ot == ODN) { int se, du; char sh; fin >> se >> du >> sh; a->SetOD(se, du, sh == 'Y'); }
            else if (ot != OT) { int di; fin >> di; a->SetOV(di); }
            RACTIONS_LIST.enqueue(a);
        }
        else if (type == 'X') {
            int time, id; fin >> time >> id;
            CACTIONS_LIST.enqueue(new Cancel_Action(time, id));
        }
    }
    fin.close();
}

void Restaurant::simulate(string inputFile, string outputFile) {
    loadFromFile(inputFile);
    int t = 0;
    while (!allDone()) {
        t++;
        executeActions(t); updateScooters(t); updateTables(t);
        updateCooking(t);  assignToChefs(t);  assignToService(t);
        ui.printTimestep(t,
            RACTIONS_LIST, CACTIONS_LIST,
            PEND_ODG, PEND_ODN, PEND_OT, PEND_OVN, PEND_OVC, PEND_OVG,
            FREE_CS, FREE_CN, COOKING,
            RDY_OT, RDY_OV_LIST, RDY_OD,
            FREE_SCOOTERS, FREE_TABLES, INSERVICE,
            MAINT_SCOOTERS, BACK_SCOOTERS,
            FINISHED, CANCELLED);
        if (interactiveMode) { cout << "Press ENTER...\n"; cin.get(); }
    }
    produceOutput(outputFile);
}

void Restaurant::produceOutput(string filename) {
    ofstream fout(filename);
    fout << "=== FINISHED ORDERS ===\n";
    while (!FINISHED.isEmpty()) { Order* o = FINISHED.pop(); fout << o << "\n"; }
    fout << "\n=== CANCELLED ORDERS ===\n";
    while (!CANCELLED.isEmpty()) { Order* o = CANCELLED.dequeue(); fout << "ID:" << o->getID() << "\n"; }
    fout.close();
}





