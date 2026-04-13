#pragma once

template <class T>
struct Node {
    T   item;
    int  priority; 
    Node* next;
    Node(T i, int p = 0) : item(i), priority(p), next(nullptr) {}  // if p = 0 then it is normal node 
};
