#include <iostream>
#include <ctime>
#include <cstdlib>

#include "Restaurant.h"

using namespace std;

int main()
{
    Restaurant r;

    string filename;
    cout << "Enter input file name: ";
    cin >> filename;

    if (!r.loadFromFile(filename))
        return 1;

    r.simulate();

    return 0;
}