#pragma once
#include <iostream>
using namespace std;


enum ORD_TYPE { OD, ODG, ODN, OT, OV, OVC, OVG, OVN };
enum ORD_STATUS { PENDING, COOKING, READY, SERV, DONE };

class Order
{
private:
    // Common attributes
    int ID;
    ORD_TYPE type;
    ORD_STATUS status;

    int TQ, TA, TR, TS, TF;
    int size;
    double price;
    int chefID;

    // Dine-in
    int seats;
    double duration;
    bool canShare;
    int tableID;

    // Delivery
    int distance;
    int scooterID;

public:
    // Constructors
    Order(int id, ORD_TYPE t, int tq, int s, double p);
    Order(int id, ORD_TYPE t, int tq, int s, double p, int st, double dur, bool share);
    Order(int id, ORD_TYPE t, int tq, int s, double p, int dist);

    // Getters
    int getID();
    ORD_TYPE getType();
    ORD_STATUS getStatus();

    int getTQ();
    int getTA();
    int getTR();
    int getTS();
    int getTF();

    int getSize();
    float getPrice();
    int getChefID();

    int getSeats();
    int getDuration();
    bool getCanShare();
    int getTableID();

    int getDistance();
    int getScooterID();

    // Setters
    void setStatus(ORD_STATUS s);

    void setTA(int ta);
    void setTR(int tr);
    void setTS(int ts);
    void setTF(int tf);

    void setChefID(int id);
    void setTableID(int id);
    void setScooterID(int id);

    // Calculations
    int getTi();
    int getTC();
    int getTW();
    int getTserv();

    // Helper functions
    bool isDineIn();
    bool isDelivery();
    bool isTakeaway();
    bool isGrilled();
    bool isVIP(); // OVG

    double getPriority(); // for OVG

    // Output
    friend ostream& operator<<(ostream& out, Order* o);
};