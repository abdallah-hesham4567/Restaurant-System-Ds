#pragma once
#include "Node.h"
#include <iostream>
using namespace std;

template <class T>
class LinkedQueue {
protected:
    Node<T>* front;
    Node<T>* rear;
    int      count;
public:
    LinkedQueue() : front(nullptr), rear(nullptr), count(0) {}

    void enqueue(T item) {
        Node<T>* NEW = new Node<T>(item);
        if (!rear) front = rear = NEW;

        else 
        { rear->next = NEW;
        rear = NEW; }

        count++;
    }

    T dequeue() {
        Node<T>* tmp = front;
        T item = tmp->item;
        front = front->next;
        if (!front) rear = nullptr;
        delete tmp;
        count--;
        return item;
    }

    T peek() { return front->item; }

    bool isEmpty() { return count == 0; }

    int  getCount() { return count; }

    void print() {
        Node<T>* cur = front;
		if (!cur) { cout << "Empty\n"; return; }
        while (cur) { 
        cout << cur->item << " "; 
        cur = cur->next; 
        }

        cout << endl;
    }

    void printIDs() {
        Node<T>* cur = front;
        if (!cur) { cout << "Empty\n"; return; }
        while (cur) {
            cout << cur->item->getID() << " ";
            cur = cur->next;
        }
        cout << endl;
    }
	Node<T>* getFront() { return front; }
};