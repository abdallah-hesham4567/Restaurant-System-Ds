#include "Restaurant.h"

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
    TH = 0;
    totalSimTime = 0;
    interactiveMode = true;
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

void Restaurant::loadTables()
{
    // Tables are loaded from file in loadFromFile()
    // Each table pair (count, capacity) is handled there
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

    // Line: TH (overwait threshold)
    fin >> TH;

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
        o->setStatus(CANCELLED_S);
        cancelledOrders.enqueue(o);
        return;
    }

    // 3. Try cooking
    o = cooking.CancelAndReturn(id);
    if (o)
    {
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

void Restaurant::assignToChefs(int timestep)
{
    int repeat = 30;

    while (repeat--)
    {
        Order* o = nullptr;
        Chef* c = nullptr;

        if (!pendODG.isEmpty() && !freeCS.isEmpty())
        {
            o = pendODG.dequeue();
            c = freeCS.dequeue();
        }
        else if (!pendOVG.isEmpty() && !freeCS.isEmpty())
        {
            o = pendOVG.dequeue();
            c = freeCS.dequeue();
        }
        else if (!pendODN.isEmpty())
        {
            o = pendODN.dequeue();
            if (!freeCN.isEmpty())      c = freeCN.dequeue();
            else if (!freeCS.isEmpty()) c = freeCS.dequeue();
        }
        else if (!pendOVN.isEmpty() && !freeCN.isEmpty())
        {
            o = pendOVN.dequeue();
            c = freeCN.dequeue();
        }
        else if (!pendOVC.isEmpty())
        {
            o = pendOVC.dequeue();
            if (!freeCN.isEmpty())      c = freeCN.dequeue();
            else if (!freeCS.isEmpty()) c = freeCS.dequeue();
        }
        else if (!pendOT.isEmpty() && !freeCN.isEmpty())
        {
            o = pendOT.dequeue();
            c = freeCN.dequeue();
        }

        if (!o || !c)
            break;

        o->setChef(c);
        o->setChefID(c->getID());
        o->setTA(timestep);
        o->setStatus(COOKING);

        int finish = timestep + (o->getSize() / c->getSpeed()) + 1;
        o->setFinishCookTime(finish);

        c->setBusy(true);
        c->addBusyTime(finish - timestep);

        cooking.enqueue(o, -finish);
    }
}

/*
==================================================
Cooking Update  [Feature 9]
==================================================
*/

void Restaurant::updateCooking(int timestep)
{
    while (!cooking.isEmpty())
    {
        Order* o = cooking.peek();

        if (o->getFinishCookTime() <= timestep)
        {
            cooking.dequeue();

            Chef* c = o->getChef();
            c->setBusy(false);

            if (c->getType() == "CS")
                freeCS.enqueue(c);
            else
                freeCN.enqueue(c);

            o->setTR(timestep);
            o->setStatus(READY);

            moveToReady(o);
        }
        else
            break;
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

        if (o->getFinishServiceTime() > timestep)
            break;

        inService.dequeue();

        o->setTF(timestep);
        o->setStatus(FINISHED_S);

        if (o->isDineIn())
        {
            Table* t = o->getTable();
            freeTables.enqueue(t, -t->getCapacity());
        }
        else if (o->isDelivery())
        {
            Scooter* s = o->getScooter();
            int back = timestep + (o->getDistance() / s->getSpeed()) + 1;
            s->setReturnTime(back);
            backScooters.enqueue(s, -back);
        }

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

        if (o->getCanShare())
            t = busy_sharable.getBest(o->getSeats());

        // 2. If no sharable table fits, try a free table
        if (!t)
            t = freeTables.getBest(o->getSeats());

        if (!t)
            break;

        readyOD.dequeue();

        o->setTable(t);
        o->setTableID(t->getID());

        t->reserveSeats(o->getSeats(), o->getDuration(), timestep, o->getCanShare());

        o->setTS(timestep);
        o->setStatus(INSERVICE);

        int finish = timestep + o->getDuration();
        o->setFinishServiceTime(finish);

        inService.enqueue(o, -finish);

        // 4. Re-classify the table after seating
        if (t->getFreeSeats() > 0 && t->isSharable())
            busy_sharable.enqueue(t, -t->getFreeSeats()); // best fit = least free seats first
        else if (t->getFreeSeats() > 0 && !t->isSharable())
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

        int finish = timestep + (o->getDistance() / s->getSpeed()) + 1;
        o->setFinishServiceTime(finish);

        s->incrementOrders();
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
                maintenanceScooters.enqueue(s);
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

void Restaurant::updateTables(int /*timestep*/)
{
    // reserved for future use
}

/*
==================================================
Generate Output File  [Feature 13]
==================================================
*/

void Restaurant::generateOutputFile()
{
    ofstream fout("output.txt");

    // Print finished orders sorted by TF descending (stack is LIFO = descending push order)
    fout << "Finished Orders (sorted by TF descending):\n";
    fout << "TF\tID\tTQ\tTA\tTR\tTS\tTi\tTC\tTw\tTserv\n";

    // We need to iterate the stack — depends on Stack implementation
    // Assuming Stack has a way to iterate or we use a temp stack
    Stack<Order*> temp;
    while (!finishedOrders.isEmpty())
    {
        Order* o = finishedOrders.pop();
        temp.push(o);

        int Ti = (o->getTA() - o->getTQ()) + (o->getTS() - o->getTR());
        int TC = o->getTR() - o->getTA();
        int Tw = Ti + TC;
        int Tserv = o->getTF() - o->getTS();

        fout << o->getTF() << "\t"
            << o->getID() << "\t"
            << o->getTQ() << "\t"
            << o->getTA() << "\t"
            << o->getTR() << "\t"
            << o->getTS() << "\t"
            << Ti << "\t"
            << TC << "\t"
            << Tw << "\t"
            << Tserv << "\n";
    }

    // Restore
    while (!temp.isEmpty())
        finishedOrders.push(temp.pop());

    // TODO: statistics block (Feature 13 full)
    fout << "\n--- Statistics ---\n";
    fout << "Total finished : " << finishedOrders.getCount() << "\n";
    fout << "Total cancelled: " << cancelledOrders.getCount() << "\n";

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

    while (!allDone() && timestep < 1000)
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
        assignToChefs(timestep);

        // Feature 9: move done cooking orders to ready
        updateCooking(timestep);

        // Features 10, 11, 12: assign ready orders to tables/scooters/takeaway
        moveReadyToService(timestep);

        // Features 6 & 7: move finished in-service orders out
        moveInServiceToFinished(timestep);

        // Feature 5: update scooter lists
        updateScooters(timestep);

        updateTables(timestep);

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