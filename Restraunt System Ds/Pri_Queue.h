#pragma once
#include "Node.h"
template <class T>
class PriQueue {
protected:
    Node<T>* head;
    int      count;
public:
    PriQueue() : head(nullptr), count(0) {}

    void enqueue(T item, int priority) {
        Node<T>* n = new Node<T>(item, priority);     // insert in sorted position (highest priority first)
        if (!head || priority > head->priority) {
            n->next = head; head = n;
        }
        else {
            Node<T>* cur = head;
            while (cur->next && cur->next->priority >= priority) // cut to iterate 
                cur = cur->next;
            n->next = cur->next;
            cur->next = n;
        }
        count++;
    }

    T dequeue() {
        Node<T>* tmp = head;
        T item = tmp->item;
        head = head->next;
        delete tmp;
        count--;
        return item;
    }

    T peek() { return head->item; }

    bool isEmpty() { return count == 0; }

    int  getCount() { return count; }

    void print() {
        Node<T>* cur = head;
        while (cur) { cout << cur->item << " "; cur = cur->next; }
        cout << endl;
    }
};
