#pragma once
#include <iostream>
#include "Node.h"

using namespace std;

template <typename T>
class LinkedQueue
{
protected:
    Node<T>* frontPtr;
    Node<T>* rearPtr;
    int count;

public:

    LinkedQueue()
    {
        frontPtr = rearPtr = nullptr;
        count = 0;
    }

    ~LinkedQueue()
    {
        while (!isEmpty())
            dequeue();
    }

    bool isEmpty() const
    {
        return count == 0;
    }

    int getCount() const
    {
        return count;
    }

    void enqueue(const T& item)
    {
        Node<T>* newNode = new Node<T>(item);

        if (isEmpty())
            frontPtr = rearPtr = newNode;
        else
        {
            rearPtr->next = newNode;
            rearPtr = newNode;
        }

        count++;
    }

    T dequeue()
    {
        if (isEmpty())
            return T();

        Node<T>* temp = frontPtr;
        T item = temp->item;

        frontPtr = frontPtr->next;

        if (!frontPtr)
            rearPtr = nullptr;

        delete temp;
        count--;

        return item;
    }

    T peek() const
    {
        if (isEmpty())
            return T();

        return frontPtr->item;
    }

    Node<T>* getFront() const
    {
        return frontPtr;
    }

    void printIDs() const
    {
        Node<T>* current = frontPtr;

        if (!current)
        {
            cout << "Empty";
            return;
        }

        while (current)
        {
            cout << current->item->getID();

            if (current->next)
                cout << ", ";

            current = current->next;
        }
    }

    int getRandomID() const
    {
        if (isEmpty())
            return -1;

        int steps = rand() % count;
        Node<T>* current = frontPtr;

        for (int i = 0; i < steps; i++)
            current = current->next;

        return current->item->getID();
    }
};