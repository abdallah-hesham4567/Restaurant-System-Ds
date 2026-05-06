#pragma once
#include <iostream>

using namespace std;

class Chef;
class Scooter;
class Table;

enum ORD_TYPE
{
    ODG,
    ODN,
    OT,
    OVC,
    OVG,
    OVN
};

enum ORD_STATUS
{
    PENDING,
    COOKING,
    READY,
    INSERVICE,
    FINISHED_S,
    CANCELLED_S
};

// Represents one restaurant order
class Order
{
private:
    int id;
    ORD_TYPE type;
    ORD_STATUS status;

    // Times
    int TQ, TA, TR, TS, TF;

    // Common data
    int size;
    double price;

    // Dine-in only
    int seats;
    int duration;
    bool canShare;

    // Delivery only
    int distance;

    // Assigned resources
    int chefID;
    int scooterID;
    int tableID;

    Chef* chef;
    Scooter* scooter;
    Table* table;

public:

    // Constructors
    Order(int ID, ORD_TYPE t, int tq, int sz, double pr);
    Order(int ID, ORD_TYPE t, int tq, int sz, double pr,
        int requiredSeats, int dur, bool share);
    Order(int ID, ORD_TYPE t, int tq, int sz, double pr,
        int dist);

    // Getters
    int getID() const;
    ORD_TYPE getType() const;
    ORD_STATUS getStatus() const;

    int getTQ() const;
    int getTA() const;
    int getTR() const;
    int getTS() const;
    int getTF() const;

    int getSize() const;
    double getPrice() const;

    int getSeats() const;
    int getDuration() const;
    bool isSharableOrder() const;

    int getDistance() const;

    int getChefID() const;
    int getScooterID() const;
    int getTableID() const;

    
    

    Chef* getChef() const;
    Scooter* getScooter() const;
    Table* getTable() const;

    // Setters
    void setStatus(ORD_STATUS s);

    void setTA(int time);
    void setTR(int time);
    void setTS(int time);
    void setTF(int time);

    void setChefID(int ID);
    void setScooterID(int ID);
    void setTableID(int ID);

    
    

    void setChef(Chef* c);
    void setScooter(Scooter* s);
    void setTable(Table* t);

    // Helpers
    bool isDineIn() const;
    bool isDelivery() const;
   

    // Calculations
    int getTi() const;
    int getTC() const;
    int getTW() const;
    int getTserv() const;

    // Priority for OVG
    double getPriority() const;

    friend ostream& operator<<(ostream& out, Order* o);
};