#pragma once

#include <vector>

struct a3MessageEntryHead
{
    a3MessageEntryHead() :type(0) {}

	int type;
};

typedef std::vector<a3MessageEntryHead*> MessageEntryVector;

struct a3MessageQueueHead
{
    a3MessageQueueHead() :head(0), tail(0) {}
	int head;
	int tail;
};

class a3MessageQueue
{
public:
	a3MessageQueue();
	~a3MessageQueue();

	void init(char* buffer, int buffer_size, int msg_size);
	void uninit();

	bool isEmpty();
	bool isFull();
	int  getSize();

	bool enqueue(const a3MessageEntryHead& msg);
	bool dequeue(a3MessageEntryHead& msg);

private:
	MessageEntryVector entries;
	a3MessageQueueHead* mqHead;
	int messageNum;
	int messageSize;

private:
	a3MessageQueue(const a3MessageQueue&);
	void operator=(const a3MessageQueue&);
};