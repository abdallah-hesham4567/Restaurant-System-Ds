#pragma once
#include"LinkedQueue.h"
#include"Order.h"

class Pend_OVC : public LinkedQueue<Order*>
{
public:
	bool Cancel_Order(int id)
	{
		Node<Order*>* curr = front;
		Node<Order*>* prev = nullptr;

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
};
