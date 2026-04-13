#pragma once
#include "LinkedQueue.h"
#include "Order.h"

class RDY_OV : public LinkedQueue<Order*>
{
public:
    Order* CancelOrder(int ID)
    {
        Node<Order*>* Curr = front;
        Node<Order*>* prev = nullptr;

        while (Curr)
        {
            if (Curr->item->getID() == ID)
            {
                if (prev == nullptr)
                {
                    front = Curr->next;
                    if (front == nullptr)
                        rear = nullptr;
                }
                else
                {
                    prev->next = Curr->next;
                    if (Curr->next == nullptr)
                        rear = prev;
                }

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