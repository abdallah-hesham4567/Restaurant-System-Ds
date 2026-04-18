#pragma once
#include <iostream>
#include "Pri_Queue.h"
#include "Order.h"
#include "Chef.h"

using namespace std;

// Cooking orders list
class Cook_Ords : public PriQueue<Order*>
{
public:

    // Remove order by ID and return pointer
    Order* CancelAndReturn(int id)
    {
        if (isEmpty())
            return nullptr;

        if (head->item->getID() == id)
            return dequeue();

        Node<Order*>* current = head;

        while (current->next)
        {
            if (current->next->item->getID() == id)
            {
                Node<Order*>* temp = current->next;
                Order* order = temp->item;

                current->next = temp->next;
                delete temp;
                count--;

                return order;
            }

            current = current->next;
        }

        return nullptr;
    }

    // Print [OrderID, ChefID]
    void printIDs() const
    {
        Node<Order*>* current = head;

        if (!current)
        {
            cout << "Empty";
            return;
        }

        while (current)
        {
            int chefID = -1;

            if (current->item->getChef())
                chefID = current->item->getChef()->getID();

            cout << "["
                << current->item->getID()
                << ", "
                << chefID
                << "]";

            if (current->next)
                cout << ", ";

            current = current->next;
        }
    }
};