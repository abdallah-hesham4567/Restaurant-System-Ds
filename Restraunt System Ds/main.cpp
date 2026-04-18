#include <iostream>
#include <ctime>
#include <cstdlib>

#include "Restaurant.h"

using namespace std;

int main()
{
    // Different random values each run
    srand((unsigned)time(0));

    Restaurant restaurant;

    restaurant.randomSimulate();

    return 0;
}