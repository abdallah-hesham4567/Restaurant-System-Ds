#pragma once
#include <climits>
#include <iostream>
#include "Pri_Queue.h"
#include "Table.h"

using namespace std;

// Table lists with best-fit search
class Fit_Tables : public PriQueue<Table*>
{
public:

    // Return smallest suitable table
    Table* getBest(int requiredSeats)
    {
        if (isEmpty())
            return nullptr;

        Node<Table*>* current = head;
        Node<Table*>* previous = nullptr;

        Node<Table*>* bestNode = nullptr;
        Node<Table*>* bestPrev = nullptr;

        int bestSeats = INT_MAX;

        while (current)
        {
            int freeSeats = current->item->getFreeSeats();

            if (freeSeats >= requiredSeats &&
                freeSeats < bestSeats)
            {
                bestSeats = freeSeats;
                bestNode = current;
                bestPrev = previous;
            }

            previous = current;
            current = current->next;
        }

        if (!bestNode)
            return nullptr;

        if (bestPrev)
            bestPrev->next = bestNode->next;
        else
            head = bestNode->next;

        Table* table = bestNode->item;

        delete bestNode;
        count--;

        return table;
    }

    bool removeTable(int id)
    {
        if (isEmpty())
            return false;

        if (head->item->getID() == id)
        {
            dequeue();
            return true;
        }

        Node<Table*>* current = head;

        while (current->next)
        {
            if (current->next->item->getID() == id)
            {
                Node<Table*>* temp = current->next;

                current->next = temp->next;
                delete temp;

                count--;
                return true;
            }

            current = current->next;
        }

        return false;
    }

    void printIDs() const
    {
        Node<Table*>* current = head;

        if (!current)
        {
            cout << "Empty";
            return;
        }

        while (current)
        {
            Table* t = current->item;

            cout << "[T"
                << t->getID()
                << ", "
                << t->getCapacity()
                << ", "
                << t->getFreeSeats()
                << "]";

            if (current->next)
                cout << ", ";

            current = current->next;
        }
    }
};