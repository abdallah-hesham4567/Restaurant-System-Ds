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

    void printIDs() {
        Node<T>* cur = head;
        if (!cur) { cout << "Empty\n"; return; }
        while (cur) {
            cout << cur->item->getID() << " ";
            cur = cur->next;
        }
        cout << endl;
	}
	Node<T>* getFront() { return head; }
    // add to PriQueue class:
    int getRandomID() {
        if (isEmpty()) return -1;
        int step = rand() % count;
        Node<T>* cur = head;
        for (int i = 0; i < step; i++)
            cur = cur->next;
        return cur->item->getID();
    }
};
