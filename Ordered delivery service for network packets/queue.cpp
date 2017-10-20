/*
 * queue.cpp
 *
 *  Created on: Oct 14, 2017
 *      Author: yahia
 */

#include "queue.h"

queue::queue() {
	__queue_first = __queue_last = nullptr;
	__queue_size = 0;

}

queue::~queue() {
	clear();
}


// insert element inside queue
void queue::enqueue(queue_t val)
{
	/* Ridiculously, if it cannot allocate, a c++ exception
	 * should be thrown as this is the default behavior of new.
	 */
	queue_node* new_node = new queue_node;

	new_node->value = val;

	if(__queue_size == 0)
	{
		// first time
		new_node->next = nullptr;
		__queue_first = __queue_last = new_node;
	}
	else
	{
		new_node->next = __queue_last;
		__queue_last = new_node;
	}

	++__queue_size;
}

// return element  and remove
queue_t queue::dequeue()
{

	if(__queue_size == 0)  return EMPTY_QUEUE;

	queue_t ret_val = __queue_first->value;
	if(__queue_size == 1)
	{
		delete __queue_first;
		__queue_first = __queue_last = nullptr;
	}
	else
	{
		queue_node* tmp = __queue_last;

		// reach node before first
		while(tmp->next != __queue_first)
			tmp = tmp->next;
		__queue_first = tmp;
		delete tmp->next;
		tmp->next = nullptr;
	}
	--__queue_size;

	return ret_val;
}

// see what is in the front
queue_t queue::front()
{
	if(__queue_size == 0) return EMPTY_QUEUE;
	return __queue_first->value;
}

// see what is back in the queue
queue_t queue::back()
{
	if(__queue_size == 0) return EMPTY_QUEUE;
	return __queue_last->value;
}

// return size of queue
unsigned int queue::size()
{
	return __queue_size;
}

// clear queue
void queue::clear()
{
	if(__queue_size == 0) return;
	queue_node* tmp = __queue_last;
	while(tmp->next != nullptr)
	{
		__queue_last = __queue_last->next;
		delete tmp;
		tmp = __queue_last;
	}
	__queue_size = 0;
}

// is it empty queue ?
bool queue::empty()
{
	if(__queue_size == 0) return 1;

	return 0;
}
