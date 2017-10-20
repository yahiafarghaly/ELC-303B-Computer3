/*
 * stack.h
 *
 *  Created on: Apr 14, 2016
 *      Author: username
 */

#ifndef STACK_H_
#define STACK_H_

#include<iostream>

#define STACK_GROUND 0

template<class T> class stack
{
private:
	struct Node
	{
		T elem;
		Node* next;
	};

	Node* _head;

public:
	stack();
	~stack();
	void push(T e);
	bool pop(T &e);
	bool top(T &e);
	bool isEmpty();
	long stackSize();
	void showStack();
};

template<class T> stack<T>::stack()
{
	_head = STACK_GROUND;
}

template<class T> stack<T>::~stack()
{
	if(_head == STACK_GROUND) return;
	Node * del;
	while(_head !=  STACK_GROUND)
	{
		del = _head;
		_head = _head->next;
		delete del;
	}
}

template<class T> void stack<T>::push(T e)
{
	Node* addedNode = new Node;
	addedNode->elem  = e;

	if(_head == STACK_GROUND)
	{
		_head = addedNode;
		_head->next = STACK_GROUND;
	}
	else
	{
		addedNode->next = _head;
		_head = addedNode;
	}
}

template<class T> bool stack<T>::pop(T &e)
{
	if(_head  == STACK_GROUND) return false;

	e = _head->elem;
	Node* del = _head;
	_head = _head->next;
	delete del;
	return true;
}

template<class T> bool stack<T>::top(T &e)
{
	if(_head  == STACK_GROUND) return false;
	e = _head->elem;
	return true;
}

template<class T> inline bool stack<T>::isEmpty()
{
	return (_head == STACK_GROUND);
}

template<class T> long stack<T>::stackSize()
{
	long counter = 0;
	if(_head == STACK_GROUND) return 0;
	Node* ptr = _head;
	while(ptr != STACK_GROUND)
	{
		counter++;
		ptr = ptr->next;
	}

	return counter;
}

template<class T> void stack<T>::showStack()
{
	if(_head  == STACK_GROUND) return;
	Node* ptr = _head;
	while(ptr != STACK_GROUND)
	{
		std::cout<<ptr->elem<<std::endl;
		ptr = ptr->next;
	}

}

#endif /* STACK_H_ */
