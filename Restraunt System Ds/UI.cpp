#include "UI.h"
// UI.cpp
using namespace std;

//void UI::readMode(bool& interactive) {
//    int choice;
//    cout << "Select mode: 1-Interactive  2-Silent: ";
//    cin >> choice;
//    interactive = (choice == 1);
//}

//void UI::readFileNames(string& in, string& out) {
//    cout << "Enter input file name: ";  cin >> in;
//    cout << "Enter output file name: "; cin >> out;
//}

void  UI ::printTimestep(
    int timestep,
    LinkedQueue<Request_Action*>& Ractions,
    LinkedQueue<Cancel_Action*>& Cactions,
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
) {
    cout << "Current Timestep: " << timestep << endl;

    cout << "=============== Request Actions List ================\n";
    Ractions.print();   // calls LinkedQueue::print()

    cout << "=============== Cancel Actions List ================\n";

    Cactions.print();   // calls LinkedQueue::print()

    cout << "------------- Pending Orders ----------------\n";
    cout << pendODG.getCount() << " PEND_ODG: "; //pendODG.print();
    cout << pendODN.getCount() << " PEND_ODN: "; //pendODN.print();
    cout << pendOT.getCount() << " PEND_OT: "; //pendOT.print();
    cout << pendOVN.getCount() << " PEND_OVN: "; ///pendOVN.print();
    cout << pendOVC.getCount() << " PEND_OVC: "; //pendOVC.print();
    cout << pendOVG.getCount() << " PEND_OVG: "; //pendOVG.print();

    cout << "------------- Available Chefs ---------------\n";
    cout << freeCS.getCount() << " CS: "; //freeCS.print();
    cout << freeCN.getCount() << " CN: "; //freeCN.print();

    cout << "------------- Cooking Orders ----------------\n";
    cout << cooking.getCount() << " cooking: "; //cooking.print();


    cout << "------------- Ready Orders ------------------\n";
    rdyOT.print();
    rdyOV.print();
    rdyOD.print();

    cout << "------------- Available Scooters ------------\n";
    cout << freeScooters.getCount() << " Scooters: "; //freeScooters.print();

    cout << "------------- Available Tables --------------\n";
    cout << freeTables.getCount() << " Tables: "; //freeTables.print();

    cout << "------------- In-Service Orders -------------\n";
    cout << inService.getCount() << " Orders: ";// inService.print();

    cout << "------------- In-Maintenance Scooters -------\n";
    cout << maintScooters.getCount() << " Scooters: "; //maintScooters.print();

    cout << "------------- Scooters Back -----------------\n";
    cout << backScooters.getCount() << " Scooters: "; //backScooters.print();

    cout << "------------- Cancelled Orders --------------\n";
    cout << cancelled.getCount() << " Cancelled: "; //cancelled.print();

    cout << "------------- Finished Orders ---------------\n";
    cout << finished.getCount() << " Orders: "; //finished.print();
}