#pragma once
#include"LinkedQueue.h"
#include"Order.h"

class Pend_OVC : public LinkedQueue<Order*>
{
public:
	bool CancelOrder(int id)
	{
		Node<Order*>* curr = front;
		
		Node<Order*>* prev = nullptr;
		cout << "entered cancel order in pend_ovc\n";
		while (curr)
		{
		
			if (curr->item->getID() == id)
			{
				if (!prev) // head will be removed
				{
					front = curr->next;
				}
				else
				{
					prev->next = curr->next;
				}
				if (curr == rear) // tail will be removed
				{
					rear = prev;
				}
				delete curr;
				return true;
			}
			prev = curr;
			curr = curr->next;
		}
		return false;
	}

	// add alongside existing CancelOrder():
	Order* CancelAndReturn(int id) {
		if (isEmpty()) return nullptr;

		if (front->item->getID() == id) {
			Order* found = front->item;
			dequeue();
			return found;
		}

		Node<Order*>* cur = front;
		while (cur->next) {
			if (cur->next->item->getID() == id) {
				Node<Order*>* tmp = cur->next;
				Order* found = tmp->item;
				cur->next = tmp->next;
				if (tmp == rear) rear = cur;
				delete tmp;
				count--;
				return found;
			}
			cur = cur->next;
		}
		return nullptr;
	}
};
