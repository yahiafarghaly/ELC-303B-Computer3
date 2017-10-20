/*
 * queue.h
 *
 *  Created on: Oct 14, 2017
 *      Author: yahia
 */

#ifndef QUEUE_H_
#define QUEUE_H_

typedef unsigned int queue_t;

#define EMPTY_QUEUE ((queue_t)0);

typedef struct node
{
	queue_t value;
	node* next;
} queue_node;


class queue {
public:
	queue();
	~queue();
	// insert element inside queue
	void enqueue(queue_t val);
	// return element  and remove
	queue_t dequeue();
	// see what is in the front
	queue_t front();
	// see what is back in the queue
	queue_t back();
	// return size of queue
	unsigned int size();
	// clear queue
	void clear();
	// is it empty queue ?
	bool empty();
private:
	unsigned int 	__queue_size;
	queue_node* 	__queue_first;
	queue_node* 	__queue_last;
};

#endif /* QUEUE_H_ */
