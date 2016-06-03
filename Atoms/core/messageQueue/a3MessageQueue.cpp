#include <core/messageQueue/a3MessageQueue.h>
#include <cassert>

a3MessageQueue::a3MessageQueue()
{

}

a3MessageQueue::~a3MessageQueue()
{

}

void a3MessageQueue::init(char* buffer, int buffer_size, int msg_size)
{
	assert(buffer);
	assert(buffer_size > 0);
	assert(msg_size > 0);

	int queue_head_size = sizeof(a3MessageQueueHead);

	// at least enough for one message.
	assert(buffer_size > (queue_head_size + msg_size));

	int total_msg_size = buffer_size - queue_head_size;
	int total_msg_num = total_msg_size / msg_size;

	mqHead = (a3MessageQueueHead*)buffer;
	entries.resize(total_msg_num);

	buffer += queue_head_size;
	for (int i = 0; i < total_msg_num; i++) {
		a3MessageEntryHead* msg = (a3MessageEntryHead*)buffer;
		entries[i] = msg;
		buffer += msg_size;
	}

	messageNum = total_msg_num;
	messageSize = msg_size;
}

void a3MessageQueue::uninit()
{
	entries.clear();
	messageNum = 0;
	messageSize = 0;
	mqHead = NULL;
}

bool a3MessageQueue::isEmpty()
{
	assert(mqHead);
	return mqHead->tail == mqHead->head;
}

bool a3MessageQueue::isFull()
{
	assert(head);
	assert(messageNum > 0);

	int tail = mqHead->tail;
	int head = mqHead->head;
	return (tail + 1) % messageNum == head;
}

int a3MessageQueue::getSize()
{
	assert(head);
	assert(messageNum > 0);

	int tail = mqHead->tail;
	int head = mqHead->head;
	return (tail - head + messageNum) % messageNum;
}

bool a3MessageQueue::enqueue(const a3MessageEntryHead& msg)
{
	assert(mqHead);
	assert(messageNum > 0);

	if (isFull()) {
		return false;
	}
	
	int tail = mqHead->tail;
	a3MessageEntryHead* dst = entries[tail];

	const char* src_buffer = (const char*)&msg;
	char* dst_buffer = (char*)dst;
	memcpy(dst_buffer, src_buffer, messageSize);

	tail = (tail + 1) % messageNum;
	mqHead->tail = tail;

	return true;
}

bool a3MessageQueue::dequeue(a3MessageEntryHead& msg)
{
	assert(head);
	assert(messageNum > 0);

	if (isEmpty()) {
		return false;
	}

	int head = mqHead->head;
	a3MessageEntryHead* src = entries[head];

	const char* src_buffer = (const char*)src;
	char* dst_buffer = (char*)&msg;
	memcpy(dst_buffer, src_buffer, messageSize);

	head = (head + 1) % messageNum;
    mqHead->head = head;

	return true;
}
