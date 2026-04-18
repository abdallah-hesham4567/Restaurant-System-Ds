#pragma once
#include <iostream>
#include "Node.h"

using namespace std;

template <typename T>
class PriQueue
{
protected:
    Node<T>* head;
    int count;

public:

    PriQueue()
    {
        head = nullptr;
        count = 0;
    }

    ~PriQueue()
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

    Node<T>* getFront() const
    {
        return head;
    }

    void enqueue(const T& item, int priority)
    {
        Node<T>* newNode = new Node<T>(item, priority);

        if (!head || priority > head->priority)
        {
            newNode->next = head;
            head = newNode;
        }
        else
        {
            Node<T>* current = head;

            while (current->next &&
                current->next->priority >= priority)
            {
                current = current->next;
            }

            newNode->next = current->next;
            current->next = newNode;
        }

        count++;
    }

    T dequeue()
    {
        if (isEmpty())
            return T();

        Node<T>* temp = head;
        T item = temp->item;

        head = head->next;

        delete temp;
        count--;

        return item;
    }

    T peek() const
    {
        if (isEmpty())
            return T();

        return head->item;
    }

    void printIDs() const
    {
        Node<T>* current = head;

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
        Node<T>* current = head;

        for (int i = 0; i < steps; i++)
            current = current->next;

        return current->item->getID();
    }
};