#pragma once
#include <iostream>
#include "Node.h"

using namespace std;

template <typename T>
class Stack
{
private:
    Node<T>* topPtr;
    int count;

public:

    Stack()
    {
        topPtr = nullptr;
        count = 0;
    }

    ~Stack()
    {
        while (!isEmpty())
            pop();
    }

    bool isEmpty() const
    {
        return count == 0;
    }

    int getCount() const
    {
        return count;
    }

    void push(const T& item)
    {
        Node<T>* newNode = new Node<T>(item);

        newNode->next = topPtr;
        topPtr = newNode;

        count++;
    }

    T pop()
    {
        if (isEmpty())
            return T();

        Node<T>* temp = topPtr;
        T item = temp->item;

        topPtr = topPtr->next;

        delete temp;
        count--;

        return item;
    }

    T peek() const
    {
        if (isEmpty())
            return T();

        return topPtr->item;
    }

    void printIDs() const
    {
        Node<T>* current = topPtr;

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
};