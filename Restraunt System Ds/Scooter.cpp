#include "Scooter.h"
using namespace std;


Scooter::Scooter(int id, double sp, int mOrd, int mDur)
: ID(id), speed(sp), mainOrd(mOrd), mainDur(mDur),
ordCount(0), distanceCut(0) {
}

int Scooter::getID()
{
    return ID;
}


double Scooter::getSpeed()
{
    return speed;

}

int Scooter::getMainOrd()
{
    return mainOrd;
}

int Scooter::getMainDur()
{
    return mainDur;
}

int Scooter::getOrdCount()
{
    return ordCount;
}

double Scooter ::getDistanceCut()
{ 
    return distanceCut;
}

void Scooter ::  addDistance(double d)
{ 
    distanceCut += d; 
}
void  Scooter::incrementOrders()
{ 
    ordCount++; 
}
bool  Scooter::needsMaintenance()
{ 
    return ordCount >= mainOrd;
}
void  Scooter::resetOrdCount()
{ 
    ordCount = 0;
}


ostream& operator<<(ostream& out, Scooter* s) {
       out << "[S" << s->ID << "]";
       return out;
   }


