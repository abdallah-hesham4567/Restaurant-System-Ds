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

    // identical to Pend_OVC — same base class LinkedQueue
    Order* CancelAndReturn(int id) {
        if (isEmpty()) return nullptr;

        if (front->item->getID() == id) {
            Order* found = front->item;
            dequeue();
            return found;
        }

        Node<Order*>* cur = front;
        while (cur->next) {
            if (cur->next->item->getID() == id) {
                Node<Order*>* tmp = cur->next;
                Order* found = tmp->item;
                cur->next = tmp->next;
                if (tmp == rear) rear = cur;
                delete tmp;
                count--;
                return found;
            }
            cur = cur->next;
        }
        return nullptr;
    }

    int getRandomID() {
        if (isEmpty()) return -1;
        int step = rand() % count;
        Node<Order*>* cur = front;
        for (int i = 0; i < step; i++)
            cur = cur->next;
        return cur->item->getID();
    }

};