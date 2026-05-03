#include "Restaurant.h"
#include <iomanip>
/*
==================================================
Constructor
==================================================
*/

Restaurant::Restaurant()
{
    csCount = cnCount = 0;
    csSpeed = cnSpeed = 0;
    scooterCount = scooterSpeed = 0;
    maintOrders = maintDuration = 0;
    tableCount = 0;
    totalSimTime = 0;
    interactiveMode = true;
    failProbability = 0.0;
}

/*
==================================================
Checks
==================================================
*/

bool Restaurant::pendingEmpty()
{
    return pendODG.isEmpty() &&
        pendODN.isEmpty() &&
        pendOT.isEmpty() &&
        pendOVN.isEmpty() &&
        pendOVC.isEmpty() &&
        pendOVG.isEmpty();
}

bool Restaurant::readyEmpty()
{
    return readyOT.isEmpty() &&
        readyOV.isEmpty() &&
        readyOD.isEmpty();
}

bool Restaurant::allDone()
{
    return requestList.isEmpty() &&
        cancelList.isEmpty() &&
        pendingEmpty() &&
        cooking.isEmpty() &&
        readyEmpty() &&
        inService.isEmpty();
}

/*
==================================================
Load Resources
==================================================
*/

void Restaurant::loadChefs()
{
    for (int i = 1; i <= csCount; i++)
    {
        Chef* c = new Chef(i, "CS", csSpeed);
        freeCS.enqueue(c);
    }

    for (int i = 1; i <= cnCount; i++)
    {
        Chef* c = new Chef(100 + i, "CN", cnSpeed);
        freeCN.enqueue(c);
    }
}

void Restaurant::loadScooters()
{
    for (int i = 1; i <= scooterCount; i++)
    {
        Scooter* s = new Scooter(i, scooterSpeed, maintOrders, maintDuration);
        freeScooters.enqueue(s, -(int)s->getDistanceCut());
    }
}



/*
==================================================
Load From File  [Feature 2]
==================================================
*/

bool Restaurant::loadFromFile(const string& filename)
{
    ifstream fin(filename);
    if (!fin.is_open())
    {
        cout << "Error: cannot open file " << filename << endl;
        return false;
    }

    // Line 1: CN CS
    fin >> cnCount >> csCount;

    // Line 2: CN_speed CS_speed
    fin >> cnSpeed >> csSpeed;

    // Line 3: S_count S_speed
    fin >> scooterCount >> scooterSpeed;

    // Line 4: Maint_Orders Maint_Duration
    fin >> maintOrders >> maintDuration;

    // Line 5: Total_no_of_Tables
    fin >> tableCount;

    // Table pairs: count capacity (pairs sum to tableCount)
    int id = 1;
    int remaining = tableCount;
    while (remaining > 0)
    {
        int cnt, cap;
        fin >> cnt >> cap;
        for (int i = 0; i < cnt; i++)
        {
            Table* t = new Table(id++, cap);
            freeTables.enqueue(t, -cap);
        }
        remaining -= cnt;
    }


    // Number of actions
    int M;
    fin >> M;

    for (int i = 0; i < M; i++)
    {
        char actionType;
        fin >> actionType;

        if (actionType == 'Q')
        {
            // Q TYP TQ ID SIZE PRICE [SEATS DURATION CANSHARE | DISTANCE]
            string typ;
            int tq, ordID, size;
            double price;
            fin >> typ >> tq >> ordID >> size >> price;

            ORD_TYPE t;

            if (typ == "ODG") t = ODG;
            else if (typ == "ODN") t = ODN;
            else if (typ == "OT")  t = OT;
            else if (typ == "OVN") t = OVN;
            else if (typ == "OVC") t = OVC;
            else                   t = OVG;

            Request_Action* ra = new Request_Action(t, tq, ordID, size, price);

            if (t == ODG || t == ODN)
            {
                int seats, duration;
                char shareChar;
                fin >> seats >> duration >> shareChar;
                bool canShare = (shareChar == 'Y' || shareChar == 'y');
                ra->setDineIn(seats, duration, canShare);
            }
            else if (t == OVN || t == OVC || t == OVG)
            {
                int dist;
                fin >> dist;
                ra->setDelivery(dist);
            }
            // OT needs no extra fields

            requestList.enqueue(ra);
        }

   
        else if (actionType == 'X')
        {
            int tcancel, oid;
            fin >> tcancel >> oid;
            Cancel_Action* ca = new Cancel_Action(tcancel, oid);
            cancelList.enqueue(ca);
        }
    }

    fin.close();

    // Now load chefs and scooters with the parsed config
    loadChefs();
    loadScooters();
    return true;
}

/*
==================================================
Add Order
==================================================
*/

void Restaurant::AddOrder(Order* order)
{
    ORD_TYPE t = order->getType();

    if (t == ODG)
        pendODG.enqueue(order);
    else if (t == ODN)
        pendODN.enqueue(order);
    else if (t == OT)
        pendOT.enqueue(order);
    else if (t == OVN)
        pendOVN.enqueue(order);
    else if (t == OVC)
        pendOVC.enqueue(order);
    else if (t == OVG)
        pendOVG.enqueue(order, (int)order->getPriority());
}

/*
==================================================
Cancel Order  [Feature 4]
==================================================
*/

void Restaurant::Cancel_Order(int id)
{
    Order* o = nullptr;

    // 1. Try pending OVC
    
    o = pendOVC.CancelAndReturn(id);
    if (o)
    {
        o->setStatus(CANCELLED_S);
        cancelledOrders.enqueue(o);
        return;
    }

    // 2. Try ready OV
    o = readyOV.CancelAndReturn(id);
    if (o)
    {
        ORD_TYPE t = o->getType();
        if (t != OVC) return;

        o->setStatus(CANCELLED_S);
        cancelledOrders.enqueue(o);
        return;
    }

    // 3. Try cooking
    o = cooking.CancelAndReturn(id);
    if (o)
    {
        ORD_TYPE t = o->getType();
        if (t != OVC) return;

        Chef* c = o->getChef();
        if (c)
        {
            c->setBusy(false);
            if (c->getType() == "CS")
                freeCS.enqueue(c);
            else
                freeCN.enqueue(c);
        }
        o->setStatus(CANCELLED_S);
        cancelledOrders.enqueue(o);
    }
}

/*
==================================================
Process Actions  [Feature 3 & 4]
Move due Request/Cancel actions to pending lists
==================================================
*/

void Restaurant::processActions(int timestep)
{
    // Process request actions whose TQ <= current timestep
    while (!requestList.isEmpty())
    {
        Request_Action* ra = requestList.peek();
        if (ra->getTimeStep() > timestep)
            break;

        requestList.dequeue();
        ra->Act(this);
    }

    // Process cancel actions whose Tcancel <= current timestep
    while (!cancelList.isEmpty())
    {
        Cancel_Action* ca = cancelList.peek();
        if (ca->getTimeStep() > timestep)
            break;

        cancelList.dequeue();
        ca->Act(this);
    }
}

/*
==================================================
Move To Ready
==================================================
*/

void Restaurant::moveToReady(Order* o)
{
    ORD_TYPE t = o->getType();

    if (t == OT)
        readyOT.enqueue(o);
    else if (t == OVC || t == OVG || t == OVN)
        readyOV.enqueue(o);
    else
        readyOD.enqueue(o);
}

/*
==================================================
Assign Orders To Chefs  [Feature 8]
==================================================
*/

void Restaurant::MovePendingToCooking(int timestep)
{
    // Keep assigning as long as possible
    while (true)
    {
        Order* o = nullptr;
        Chef* c = nullptr;

        // ===============================
        // 1) ODG → CS only 
        // ===============================
        if (!pendODG.isEmpty() && !freeCS.isEmpty())
        {
            o = pendODG.dequeue();
            c = freeCS.dequeue();
        }
        else if (!pendODN.isEmpty() && !freeCS.isEmpty())
        {
            o = pendODN.dequeue();

            if (!freeCN.isEmpty())
                c = freeCN.dequeue();
            else if (!freeCS.isEmpty())
                c = freeCS.dequeue();
        }

        // ===============================
        // 3) OT → CN only
        // ===============================
        else if (!pendOT.isEmpty() && !freeCN.isEmpty())
        {
            o = pendOT.dequeue();
            c = freeCN.dequeue();
        }

        // ===============================
        // 4) OVG → CS only
        // ===============================
        else if (!pendOVG.isEmpty() && !freeCS.isEmpty())
        {
            o = pendOVG.dequeue();
            c = freeCS.dequeue();
        }

        // ===============================
        // 5) OVC → CN then CS
        // ===============================
        else if (!pendOVC.isEmpty())
        {
            o = pendOVC.dequeue();

            if (!freeCN.isEmpty())
                c = freeCN.dequeue();
            else if (!freeCS.isEmpty())
                c = freeCS.dequeue();
        }

        // ===============================
        // 6) OVN → CN only
        // ===============================
        else if (!pendOVN.isEmpty() && !freeCN.isEmpty())
        {
            o = pendOVN.dequeue();
            c = freeCN.dequeue();
        }

        //return the order back to pending list if it was dequeued but no chef assigned
		if (!o || !c) {
			if (o) {
                // Return the order back to the pending list
                switch (o->getType()) {
                    case ODG: pendODG.enqueue(o); break;
                    case ODN: pendODN.enqueue(o); break;
                    case OT:  pendOT.enqueue(o); break;
                    case OVN: pendOVN.enqueue(o); break;
                    case OVC: pendOVC.enqueue(o); break;
                    case OVG: pendOVG.enqueue(o, (int)o->getPriority()); break;
				}
                }
			break;
        } 

        // ===============================
        // Assign order to chef
        // ===============================
        o->setChef(c);
        o->setChefID(c->getID());
        o->setTA(timestep);
        o->setStatus(COOKING);

        // Calculate cooking time correctly
        int cookTime = ceil((double)o->getSize() / c->getSpeed());
        int finishTime = timestep + cookTime;

        o->setFinishCookTime(finishTime);

        // Update chef
        c->setBusy(true);
        c->addBusyTime(cookTime);

        // Add to cooking list (earliest finish first)
        cooking.enqueue(o, -finishTime);
    }
}

void Restaurant::MoveCookingToReady(int timestep)
{
    // Move all finished cooking orders to READY
    while (!cooking.isEmpty())
    {
        Order* o = cooking.peek();

        // Not finished yet → stop
        if (o->getFinishCookTime() > timestep)
            break;

        // Remove from cooking
        cooking.dequeue();

        // Release chef
        Chef* c = o->getChef();
        c->setBusy(false);

        if (c->getType() == "CS")
            freeCS.enqueue(c);
        else
            freeCN.enqueue(c);

        // Update order
        o->setTR(timestep);
        o->setStatus(READY);

        // Move to appropriate READY list
        moveToReady(o);
    }
}

/*
==================================================
Move InService -> Finished  [Feature 6 & 7]
==================================================
*/

void Restaurant::moveInServiceToFinished(int timestep)
{
    while (!inService.isEmpty())
    {
        Order* o = inService.peek();

        // If not finished yet → stop
        if (o->getFinishServiceTime() > timestep)
            break;

        inService.dequeue();

        o->setTF(timestep);
        o->setStatus(FINISHED_S);

        // =========================
        // DINE-IN ORDERS 
        // =========================
        if (o->isDineIn())
        {
            Table* t = o->getTable();
           
			t->releaseSeats(o->getSeats());
			// If now is empty, move to free
            if (t->getFreeSeats() == t->getCapacity())

                freeTables.enqueue(t, -t->getFreeSeats());
			// If still has free seats, put back in the  busy_sharable list
            else 
            
                busy_sharable.enqueue(t, -t->getFreeSeats());
            
			
        }
        else if (o->isDelivery())
        {
            Scooter* s = o->getScooter();
            s->incrementOrders();
            int backTime = timestep + ceil((double)o->getDistance() / s->getSpeed());

            s->setReturnTime(backTime);

            backScooters.enqueue(s, -backTime);
        }

        // =========================
        // FINISH
        // =========================
        finishedOrders.push(o);
    }
}

/*
==================================================
Ready -> Service  [Features 10, 11, 12]
==================================================
*/

void Restaurant::moveReadyToService(int timestep)
{
    // --- Dine-In Orders [Feature 10] ---
    while (!readyOD.isEmpty())
    {
        Order* o = readyOD.peek();
        Table* t = nullptr;

        if (o->isSharableOrder())
            t = busy_sharable.getBest(o->getSeats());

        // 2. If no sharable table fits, try a free table
        if (!t)
            t = freeTables.getBest(o->getSeats());

        if (!t)
            break;

        readyOD.dequeue();

        o->setTable(t);
        o->setTableID(t->getID());

        t->reserveSeats(o->getSeats(), o->getDuration(), timestep);

        o->setTS(timestep);
        o->setStatus(INSERVICE);

        int finish = timestep + o->getDuration();
        o->setFinishServiceTime(finish);

        inService.enqueue(o, -finish);

        // 4. Re-classify the table after seating
        if (t->getFreeSeats() > 0 && o->isSharableOrder())
            busy_sharable.enqueue(t, -t->getFreeSeats()); // best fit = least free seats first
        else if (t->getFreeSeats() > 0 && !o->isSharableOrder())
            busy_noshare.enqueue(t, -t->getFreeSeats());
        // if no free seats left, table sits out of all lists until moveInServiceToFinished returns it
    }

    // --- Delivery Orders [Feature 11] ---
    while (!readyOV.isEmpty() && !freeScooters.isEmpty())
    {
        Order* o = readyOV.dequeue();
        Scooter* s = freeScooters.dequeue();

        o->setScooter(s);
        o->setScooterID(s->getID());
        o->setTS(timestep);
        o->setStatus(INSERVICE);

		int finish = timestep + ceil((double)o->getDistance() / s->getSpeed());
        o->setFinishServiceTime(finish);

       
        s->addDistance(o->getDistance());

        inService.enqueue(o, -finish);
    }

    // --- Takeaway Orders [Feature 12] ---
    while (!readyOT.isEmpty())
    {
        Order* o = readyOT.dequeue();

        o->setTS(timestep);
        o->setTF(timestep + 1);
        o->setStatus(FINISHED_S);

        finishedOrders.push(o);
    }
}

/*
==================================================
Scooters Update  [Feature 5]
==================================================
*/

void Restaurant::updateScooters(int timestep)
{
    // Back_Scooters -> Free or Maintenance
    while (!backScooters.isEmpty())
    {
        Scooter* s = backScooters.peek();

        if (s->getReturnTime() <= timestep)
        {
            backScooters.dequeue();

            if (s->needsMaintenance())
            {
                s->setMaintenanceEnd(timestep + maintDuration);
                maintenanceScooters.enqueue(s);
                 
            }
            else
                freeScooters.enqueue(s, -(int)s->getDistanceCut());
        }
        else
            break;
    }

   // Maint_Scooters -> Free (check if maintenance duration is done)
    while (!maintenanceScooters.isEmpty())
    {
        Scooter* s = maintenanceScooters.peek();
        if (s->getMaintenanceEnd() <= timestep)
        {
            maintenanceScooters.dequeue();
            s->resetOrders();
            freeScooters.enqueue(s, -(int)s->getDistanceCut());
        }
        else
            break;
    }
}



/*
==================================================
Generate Output File  [Feature 13]
==================================================
*/

void Restaurant::generateOutputFile()
{
    ofstream fout("output.txt");

    // collect all finished into temp + compute stats while iterating
    Stack<Order*> temp;
    int finCount = 0;
    double sumTi = 0, sumTC = 0, sumTw = 0, sumTserv = 0;
    int cntODG = 0, cntODN = 0, cntOT = 0, cntOVN = 0, cntOVC = 0, cntOVG = 0;

    fout << "Finished Orders (sorted by TF descending):\n";
    fout << left
        << setw(6) << "TF"
        << setw(6) << "ID"
        << setw(6) << "TQ"
        << setw(6) << "TA"
        << setw(6) << "TR"
        << setw(6) << "TS"
        << setw(6) << "Ti"
        << setw(6) << "TC"
        << setw(6) << "Tw"
        << setw(8) << "Tserv"
        << "\n";

    while (!finishedOrders.isEmpty())
    {
        Order* o = finishedOrders.pop();
        temp.push(o);
        finCount++;

        int TQ = o->getTQ();
        int TA = o->getTA();
        int TR = o->getTR();
        int TS = o->getTS();
        int TF = o->getTF();

        int Ti = (TA - TQ) + (TS - TR);
        int TC = TR - TA;
        int Tw = Ti + TC;
        int Tserv = TF - TS;

        sumTi += Ti;
        sumTC += TC;
        sumTw += Tw;
        sumTserv += Tserv;

        ORD_TYPE t = o->getType();
        if (t == ODG) cntODG++;
        else if (t == ODN) cntODN++;
        else if (t == OT)  cntOT++;
        else if (t == OVN) cntOVN++;
        else if (t == OVC) cntOVC++;
        else if (t == OVG) cntOVG++;

        fout << left
            << setw(6) << TF
            << setw(6) << o->getID()
            << setw(6) << TQ
            << setw(6) << TA
            << setw(6) << TR
            << setw(6) << TS
            << setw(6) << Ti
            << setw(6) << TC
            << setw(6) << Tw
            << setw(8) << Tserv
            << "\n";
    }

    // restore stack
    while (!temp.isEmpty())
        finishedOrders.push(temp.pop());

    int canCount = cancelledOrders.getCount();
    int total = finCount + canCount;

    // chef busy time
    int csBusy = 0, cnBusy = 0;
    // iterate free chefs (all chefs are free at end of sim)
    Node<Chef*>* cur = freeCS.getFront();
    while (cur) { csBusy += cur->item->getBusyTime(); cur = cur->next; }
    cur = freeCN.getFront();
    while (cur) { cnBusy += cur->item->getBusyTime(); cur = cur->next; }
    int totalChefBusy = csBusy + cnBusy;
    int totalChefs = csCount + cnCount;
    double chefUtil = totalChefs > 0 && totalSimTime > 0
        ? (totalChefBusy * 100.0) / (totalSimTime * totalChefs) : 0;

    fout << "\n--- Statistics ---\n";
    fout << "Total orders     : " << total << "\n";
    fout << "Finished orders  : " << finCount << "\n";
    fout << "Cancelled orders : " << canCount << "\n";
    fout << "\nOrders per type:\n";
    fout << "  ODG:" << cntODG << "  ODN:" << cntODN << "  OT:" << cntOT
        << "  OVN:" << cntOVN << "  OVC:" << cntOVC << "  OVG:" << cntOVG << "\n";
    fout << "\nAvg waiting time (Ti)   : " << fixed << setprecision(2) << (finCount ? sumTi / finCount : 0) << "\n";
    fout << "Avg cooking time (TC)   : " << fixed << setprecision(2) << (finCount ? sumTC / finCount : 0) << "\n";
    fout << "Avg total wait   (Tw)   : " << fixed << setprecision(2) << (finCount ? sumTw / finCount : 0) << "\n";
    fout << "Avg service time (Tserv): " << fixed << setprecision(2) << (finCount ? sumTserv / finCount : 0) << "\n";
    fout << "Chef utilization        : " << fixed << setprecision(1) << chefUtil << "%\n";
    fout << "Total simulation time   : " << totalSimTime << "\n";

    fout.close();
    cout << "Output file created: output.txt\n";
}

/*
==================================================
Main Simulation Loop  [Feature 1]
==================================================
*/

void Restaurant::simulate()
{
    // Ask user for mode
    cout << "Select mode: (1) Interactive  (2) Silent\n> ";
    int choice; cin >> choice;
    interactiveMode = (choice == 1);

    if (!interactiveMode)
        cout << "Simulation Starts in Silent mode ...\n";

    int timestep = 1;

    while (!allDone() )
    {
        cout << "TS=" << timestep
            << " pend=" << pendODG.getCount() + pendODN.getCount() + pendOT.getCount() + pendOVN.getCount() + pendOVC.getCount() + pendOVG.getCount()
            << " cook=" << cooking.getCount()
            << " ready=" << readyOT.getCount() + readyOV.getCount() + readyOD.getCount()
            << " inSvc=" << inService.getCount()
            << " req=" << requestList.getCount()
            << endl;

        // Feature 3 & 4: move due actions to pending lists / handle cancellations
        processActions(timestep);

        // Feature 8: assign pending orders to chefs
        MovePendingToCooking(timestep);

        // Feature 9: move done cooking orders to ready
        MoveCookingToReady(timestep);

        // Features 10, 11, 12: assign ready orders to tables/scooters/takeaway
        moveReadyToService(timestep);


        // bonus
        checkRescue(timestep);
        // Features 6 & 7: move finished in-service orders out
        moveInServiceToFinished(timestep);

        
        // Feature 5: update scooter lists
        updateScooters(timestep);

        if (interactiveMode)
        {
            ui.printTimestep(
                timestep,
                requestList, cancelList,
                pendODG, pendODN, pendOT, pendOVN, pendOVC, pendOVG,
                freeCS, freeCN,
                cooking,
                readyOT, readyOV, readyOD,
                freeScooters, freeTables,
                inService,
                maintenanceScooters, backScooters,
                finishedOrders, cancelledOrders);

            cout << "\nPRESS ANY KEY TO MOVE TO NEXT STEP !\n";
            cin.get();
        }

        totalSimTime = timestep;
        timestep++;
    }

    // Feature 13: always produce output file
    generateOutputFile();

    if (!interactiveMode)
        cout << "Simulation ends, Output file created\n";
}


//bonus part
void Restaurant::checkRescue(int timestep)
{
    // Only check every 5 timesteps
    if (timestep % 5 != 0) return;

    if (freeScooters.isEmpty()) return;

    LinkedQueue<Order*> tempList;
    while (!inService.isEmpty())
    {
        Order* o = inService.peek();
        inService.dequeue();
        tempList.enqueue(o);
    }

    while (!tempList.isEmpty())
    {
        Order* o = tempList.dequeue();

        if (o->isDelivery() &&
            o->getScooter() != nullptr &&
            !o->getScooter()->hasFailed() &&
            !freeScooters.isEmpty())
        {
            double roll = (rand() % 1000) / 1000.0;
			// if (roll < failProbability)  // use it as a random failure trigger if you want non-deterministic failures
			if (false)  // for testing, force a failure every 5 timesteps if there's a free scooter to rescue with
            {
                Scooter* failed = o->getScooter();

                int elapsed = timestep - o->getTS();
                double covered = elapsed * failed->getSpeed();
                double remaining = o->getDistance() - covered;
                if (remaining < 0) remaining = 0;

                failed->setFailed(timestep, covered);
                int failedBack = timestep + (int)(covered / failed->getSpeed()) + 1;
                failed->setReturnTime(failedBack);
                backScooters.enqueue(failed, -failedBack);

                Scooter* rescue = freeScooters.dequeue();
                int rescueArrival = timestep + (int)(covered / rescue->getSpeed()) + 1;
                int newFinish = rescueArrival + (int)(remaining / rescue->getSpeed()) + 1;

                o->setScooter(rescue);
                o->setScooterID(rescue->getID());
                o->setFinishServiceTime(newFinish);

                rescue->incrementOrders();
                rescue->addDistance(o->getDistance());

                // Only print when rescue actually happens
                cout << "[RESCUE] TS=" << timestep
                    << " S" << failed->getID() << " failed!"
                    << " S" << rescue->getID()
                    << " rescuing Order " << o->getID()
                    << " -> new finish=" << newFinish << "\n";
            }
        }
        inService.enqueue(o, -o->getFinishServiceTime());
    }
}