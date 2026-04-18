#pragma once
#include "LinkedQueue.h"
#include "Order.h"

// Ready delivery orders
class RDY_OV : public LinkedQueue<Order*>
{
public:

    Order* CancelAndReturn(int id)
    {
        if (isEmpty())
            return nullptr;

        if (frontPtr->item->getID() == id)
            return dequeue();

        Node<Order*>* current = frontPtr;

        while (current->next)
        {
            if (current->next->item->getID() == id)
            {
                Node<Order*>* temp = current->next;
                Order* order = temp->item;

                current->next = temp->next;

                if (temp == rearPtr)
                    rearPtr = current;

                delete temp;
                count--;

                return order;
            }

            current = current->next;
        }

        return nullptr;
    }
};