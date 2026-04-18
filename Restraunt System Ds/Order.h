#pragma once
#include <iostream>
using namespace std;

// forward declarations to avoid circular includes
class Chef;
class Scooter;
class Table;

enum ORD_TYPE { ODG, ODN, OT, OVC, OVG, OVN };
enum ORD_STATUS { PENDING, COOKING, READY, INSERVICE, FINISHED_S, CANCELLED_S };

class Order {
private:
    int        ID;
    ORD_TYPE   type;
    ORD_STATUS status;

    // timestamps
    int    TQ, TA, TR, TS, TF;

    // common
    int    size;
    double price;

    // dine-in only
    int    seats;
    double duration;
    bool   canShare;

    // delivery only
    int    distance;

    // IDs (for output)
    int    chefID, tableID, scooterID;

    // ── NEW: object pointers ──────────────────────
    Chef* chef;
    Scooter* scooter;
    Table* table;

    // ── NEW: finish times ─────────────────────────
    int    finishCookTime;     // TA + cookTime
    int    finishServiceTime;  // TS + deliveryTime or duration

public:
    // constructors
    Order(int id, ORD_TYPE t, int tq, int s, double p);                            // OT
    Order(int id, ORD_TYPE t, int tq, int s, double p, int st, double dur, bool share); // dine-in
    Order(int id, ORD_TYPE t, int tq, int s, double p, int dist);                  // delivery

    // getters — original
    int        getID();
    ORD_TYPE   getType();
    ORD_STATUS getStatus();
    int        getTQ();
    int        getTA();
    int        getTR();
    int        getTS();
    int        getTF();
    int        getSize();
    float      getPrice();
    int        getChefID();
    int        getSeats();
    int        getDuration();
    bool       getCanShare();
    int        getTableID();
    int        getDistance();
    int        getScooterID();

    // getters — new
    Chef* getChef();
    Scooter* getScooter();
    Table* getTable();
    int      getFinishCookTime();
    int      getFinishServiceTime();

    // setters — original
    void setStatus(ORD_STATUS s);
    void setTA(int ta);
    void setTR(int tr);
    void setTS(int ts);
    void setTF(int tf);
    void setChefID(int id);
    void setTableID(int id);
    void setScooterID(int id);

    // setters — new
    void setChef(Chef* c);
    void setScooter(Scooter* s);
    void setTable(Table* t);
    void setFinishCookTime(int t);
    void setFinishServiceTime(int t);

    // calculations
    int getTi();
    int getTC();
    int getTW();
    int getTserv();

    // helpers
    bool isDineIn();
    bool isDelivery();
    bool isTakeaway();
    bool isGrilled();
    bool isVIP();

    // priority (OVG only)
    double getPriority();

    friend ostream& operator<<(ostream& out, Order* o);
};