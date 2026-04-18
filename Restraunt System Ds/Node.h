#pragma once

// Generic node used in Queue / Stack / Priority Queue
template <typename T>
struct Node
{
    T item;               // Stored data
    int priority;         // Used only in priority queue
    Node<T>* next;        // Pointer to next node

    Node(const T& value, int p = 0)
    {
        item = value;
        priority = p;
        next = nullptr;
    }
};