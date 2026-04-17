#pragma once
#include "Pri_Queue.h"
#include "Order.h"

class Cook_Ords : public PriQueue<Order*>
{
public:
    Order* CancelOrder(int ID)
    {
        Node<Order*>* Curr = head;
        Node<Order*>* prev = nullptr;

        while (Curr)
        {
            if (Curr->item->getID() == ID && Curr->item->getType() == OVC)
            {
                if (prev == nullptr)
                    head = Curr->next;
                else
                    prev->next = Curr->next;

                Order* found = Curr->item;
                delete Curr;
                count--;
                return found;
            }
            prev = Curr;
            Curr = Curr->next;
        }
        return nullptr;
    }
};