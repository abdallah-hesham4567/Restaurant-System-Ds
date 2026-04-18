#include "Restaurant.h"

/*
==================================================
Constructor
==================================================
*/

Restaurant::Restaurant()
{
    srand((unsigned)time(0));

    csCount = 3;
    cnCount = 4;

    csSpeed = 3;
    cnSpeed = 2;

    scooterCount = 5;
    tableCount = 7;

    interactiveMode = true;

    loadChefs();
    loadScooters();
    loadTables();
    generateRandomOrders();
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
        Scooter* s = new Scooter(i, 100, 4, 3);
        freeScooters.enqueue(s, -(int)s->getDistanceCut());
    }
}

void Restaurant::loadTables()
{
    for (int i = 1; i <= tableCount; i++)
    {
        int cap = 2 + rand() % 6;
        Table* t = new Table(i, cap);

        freeTables.enqueue(t, -cap);
    }
}

/*
==================================================
Orders
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

void Restaurant::Cancel_Order(int id)
{
    Order* o = nullptr;

    o = pendOVC.CancelAndReturn(id);

    if (o)
    {
        o->setStatus(CANCELLED_S);
        cancelledOrders.enqueue(o);
        return;
    }

    o = readyOV.CancelAndReturn(id);

    if (o)
    {
        o->setStatus(CANCELLED_S);
        cancelledOrders.enqueue(o);
        return;
    }

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
Generate Random Orders
==================================================
*/

void Restaurant::generateRandomOrders()
{
    int id = 1;

    for (int tq = 1; tq <= 20; tq++)
    {
        for (int k = 0; k < 25; k++)
        {
            int kind = rand() % 6;
            int size = rand() % 10 + 1;
            double money = rand() % 300 + 50;

            Request_Action* act = nullptr;

            if (kind == 0)
            {
                act = new Request_Action(ODG, tq, id, size, money);
                act->setDineIn(rand() % 4 + 1,
                    rand() % 10 + 2,
                    rand() % 2);
            }
            else if (kind == 1)
            {
                act = new Request_Action(ODN, tq, id, size, money);
                act->setDineIn(rand() % 4 + 1,
                    rand() % 10 + 2,
                    rand() % 2);
            }
            else if (kind == 2)
            {
                act = new Request_Action(OT, tq, id, size, money);
            }
            else if (kind == 3)
            {
                act = new Request_Action(OVN, tq, id, size, money);
                act->setDelivery(rand() % 3000 + 300);
            }
            else if (kind == 4)
            {
                act = new Request_Action(OVC, tq, id, size, money);
                act->setDelivery(rand() % 3000 + 300);
            }
            else
            {
                act = new Request_Action(OVG, tq, id, size, money);
                act->setDelivery(rand() % 3000 + 300);
            }

            requestList.enqueue(act);
            id++;
        }
    }
}

/*
==================================================
Actions
==================================================
*/

void Restaurant::executeActions(int timestep)
{
    while (!requestList.isEmpty() &&
        requestList.peek()->getTimeStep() <= timestep)
    {
        Request_Action* a = requestList.dequeue();
        a->Act(this);
        delete a;
    }

    while (!cancelList.isEmpty() &&
        cancelList.peek()->getTimeStep() <= timestep)
    {
        Cancel_Action* c = cancelList.dequeue();
        c->Act(this);
        delete c;
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
Assign Orders To Chefs
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

            if (!freeCN.isEmpty())
                c = freeCN.dequeue();
            else if (!freeCS.isEmpty())
                c = freeCS.dequeue();
        }
        else if (!pendOVN.isEmpty() && !freeCN.isEmpty())
        {
            o = pendOVN.dequeue();
            c = freeCN.dequeue();
        }
        else if (!pendOVC.isEmpty())
        {
            o = pendOVC.dequeue();

            if (!freeCN.isEmpty())
                c = freeCN.dequeue();
            else if (!freeCS.isEmpty())
                c = freeCS.dequeue();
        }
        else if (!pendOT.isEmpty() && !freeCN.isEmpty())
        {
            o = pendOT.dequeue();
            c = freeCN.dequeue();
        }

        if (!o || !c)
            continue;

        o->setChef(c);
        o->setChefID(c->getID());
        o->setTA(timestep);
        o->setStatus(COOKING);

        int finish =
            timestep +
            (o->getSize() / c->getSpeed()) + 1;

        o->setFinishCookTime(finish);

        c->setBusy(true);
        c->addBusyTime(finish - timestep);

        cooking.enqueue(o, -finish);
    }
}

/*
==================================================
Cooking Update
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
Ready -> Service
==================================================
*/

void Restaurant::moveReadyToService(int timestep)
{
    // Dine In Orders
    while (!readyOD.isEmpty())
    {
        Order* o = readyOD.peek();

        Table* t = freeTables.getBest(o->getSeats());

        if (!t)
            break;

        readyOD.dequeue();

        o->setTable(t);
        o->setTableID(t->getID());

        t->reserveSeats(
            o->getSeats(),
            o->getDuration(),
            timestep,
            o->getCanShare());

        o->setTS(timestep);
        o->setStatus(INSERVICE);

        int finish =
            timestep + o->getDuration();

        o->setFinishServiceTime(finish);

        inService.enqueue(o, -finish);
    }

    // Delivery Orders
    while (!readyOV.isEmpty() &&
        !freeScooters.isEmpty())
    {
        Order* o = readyOV.dequeue();
        Scooter* s = freeScooters.dequeue();

        o->setScooter(s);
        o->setScooterID(s->getID());

        o->setTS(timestep);
        o->setStatus(INSERVICE);

        int finish =
            timestep +
            (o->getDistance() / s->getSpeed()) + 1;

        o->setFinishServiceTime(finish);

        s->incrementOrders();
        s->addDistance(o->getDistance());

        inService.enqueue(o, -finish);
    }

    // Takeaway Orders
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
Finish Orders
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

            freeTables.enqueue(
                t,
                -t->getCapacity());
        }
        else if (o->isDelivery())
        {
            Scooter* s = o->getScooter();

            int backTime =
                timestep +
                (o->getDistance() / s->getSpeed()) + 1;

            s->setReturnTime(backTime);

            backScooters.enqueue(
                s,
                -backTime);
        }

        finishedOrders.push(o);
    }
}
/*
==================================================
Scooters / Tables
==================================================
*/

void Restaurant::updateScooters(int timestep)
{
    while (!backScooters.isEmpty())
    {
        Scooter* s = backScooters.peek();

        if (s->getReturnTime() <= timestep)
        {
            backScooters.dequeue();

            if (s->needsMaintenance())
                maintenanceScooters.enqueue(s);
            else
                freeScooters.enqueue(
                    s,
                    -(int)s->getDistanceCut());
        }
        else
            break;
    }

    if (!maintenanceScooters.isEmpty() &&
        rand() % 2 == 0)
    {
        Scooter* s =
            maintenanceScooters.dequeue();

        s->resetOrders();

        freeScooters.enqueue(
            s,
            -(int)s->getDistanceCut());
    }
}

void Restaurant::updateTables(int timestep)
{
    // reserved for future phase
}

/*
==================================================
Simulation
==================================================
*/

void Restaurant::randomSimulate()
{
    int timestep = 1;

    while (!allDone())
    {
        executeActions(timestep);

        assignToChefs(timestep);

        updateCooking(timestep);

        moveReadyToService(timestep);

        moveInServiceToFinished(timestep);

        updateScooters(timestep);

        updateTables(timestep);  

        ui.printTimestep(
            timestep,

            requestList,
            cancelList,

            pendODG,
            pendODN,
            pendOT,
            pendOVN,
            pendOVC,
            pendOVG,

            freeCS,
            freeCN,

            cooking,

            readyOT,
            readyOV,
            readyOD,

            freeScooters,
            freeTables,

            inService,

            maintenanceScooters,
            backScooters,

            finishedOrders,
            cancelledOrders
        );

        cin.get();
        timestep++;
    }

    cout << "\nSimulation Finished\n";
}