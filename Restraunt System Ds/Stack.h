#pragma once
#include "Node.h"
template <class T>
class Stack {
protected:
    Node<T>* top;
    int      count;
public:
    Stack() : top(nullptr), count(0) {}

    void push(T item) {
        Node<T>* n = new Node<T>(item);
        n->next = top;
        top = n;
        count++;
    }

    T pop() {
        Node<T>* tmp = top;
        T item = tmp->item;
        top = top->next;
        delete tmp;
        count--;
        return item;
    }

    T peek() { return top->item; }

    bool isEmpty() { return count == 0; }

    int  getCount() { return count; }

    void print() {
        Node<T>* cur = top;
        while (cur) { cout << cur->item << " "; cur = cur->next; }
        cout << endl;
    }

    void printIDs() {
        Node<T>* cur = top;
        if (!cur) { cout << "Empty\n"; return; }
        while (cur) {
            cout << cur->item->getID() << " ";
            cur = cur->next;
        }
        cout << endl;
	}

};
