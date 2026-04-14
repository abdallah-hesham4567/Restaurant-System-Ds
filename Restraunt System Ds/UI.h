#pragma once
#include <iostream>
#include <string>
using namespace std;

#include "LinkedQueue.h"
#include "Pri_Queue.h"
#include "Stack.h"
#include "Pend_OVC.h"
#include "Cook_Ords.h"
#include "RDY_OV.h"
#include "Fit_Tables.h"

// Entity classes
#include "Order.h"
#include "Chef.h"
#include "Scooter.h"
#include "Table.h"

// UI.h
class UI {
public:
    // ── reads ──────────────────────────────
    void readMode(bool& interactive);          // ask user: interactive or silent
    void readFileNames(string& in, string& out); // ask for input/output file names

    // ── print ──────────────────────────────
    void printTimestep(
        int timestep,
        LinkedQueue<Action*>& actions,
        LinkedQueue<Order*>& pendODG,
        LinkedQueue<Order*>& pendODN,
        LinkedQueue<Order*>& pendOT,
        LinkedQueue<Order*>& pendOVN,
        Pend_OVC& pendOVC,
        PriQueue<Order*>& pendOVG,
        LinkedQueue<Chef*>& freeCS,
        LinkedQueue<Chef*>& freeCN,
        Cook_Ords& cooking,
        LinkedQueue<Order*>& rdyOT,
        RDY_OV& rdyOV,
        LinkedQueue<Order*>& rdyOD,
        PriQueue<Scooter*>& freeScooters,
        Fit_Tables& freeTables,
        PriQueue<Order*>& inService,
        LinkedQueue<Scooter*>& maintScooters,
        PriQueue<Scooter*>& backScooters,
        Stack<Order*>& finished,
        LinkedQueue<Order*>& cancelled
    );
};
