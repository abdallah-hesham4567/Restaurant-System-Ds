#pragma once

#include "Pri_Queue.h"
#include "Table.h"

class Fit_Tables : public PriQueue<Table*> {
public:
    // returns and removes the best fit table for given seat count
    // best fit = smallest freeSeats >= required seats
    Table* getBest(int requiredSeats) {
        if (isEmpty()) return nullptr;

        Node<Table*>* cur = head;
        Node<Table*>* prev = nullptr;

        Node<Table*>* bestNode = nullptr;
        Node<Table*>* bestPrev = nullptr;
        int bestFree = INT_MAX;

        // walk entire list to find table with minimum freeSeats >= required
        while (cur) {
            int fs = cur->item->getFreeSeats();
            if (fs >= requiredSeats && fs < bestFree) {
                bestFree = fs;
                bestNode = cur;
                bestPrev = prev;
            }
            prev = cur;
            cur = cur->next;
        }

        if (!bestNode) return nullptr;  // no fitting table found

        // remove bestNode from list
        if (bestPrev)  bestPrev->next = bestNode->next;
        else          head = bestNode->next;

        Table* t = bestNode->item;
        delete bestNode;
        count--;
        return t;
    }

    void printIDs() {
        Node<Table*>* cur = head;
        if (!cur) { cout << "Empty\n"; return; }
        while (cur) {
            Table* t = cur->item;
            cout << "[T" << t->getID() << ", " << t->getCapacity()
                << ", " << t->getFreeSeats() << "] ";
            cur = cur->next;
        }
        cout << endl;
	}
};