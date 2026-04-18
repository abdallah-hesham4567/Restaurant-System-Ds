#pragma once
#include "Pri_Queue.h"
#include "Order.h"
#include "Chef.h"  
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

    Order* CancelAndReturn(int id) {
        if (isEmpty()) return nullptr;

        if (head->item->getID() == id) {
            Order* found = head->item;
            dequeue();
            return found;
        }

        Node<Order*>* cur = head;
        while (cur->next) {
            if (cur->next->item->getID() == id) {
                Node<Order*>* tmp = cur->next;
                Order* found = tmp->item;
                cur->next = tmp->next;
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
        Node<Order*>* cur = head;
        for (int i = 0; i < step; i++)
            cur = cur->next;
        return cur->item->getID();
    }

    void printIDs() {
        Node<Order*>* cur = head;
        int printed = 0;
        while (cur) {
            int chefID = -1;
			Chef* c = cur->item->getChef();
            if (c)
                chefID = c->getID();
            cout << "[" << cur->item->getID() << ", " << chefID << "]";
            printed++;
            cur = cur->next;
            if (cur) cout << ", ";
        }
        if (printed == 0) cout << "none";
        cout << endl;
    }
};