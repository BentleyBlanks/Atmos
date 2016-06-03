#pragma once

#include <string>
#include <windows.h>
#include <core/messageQueue/a3MessageQueue.h>

struct a3Mutex
{
public:
	a3Mutex():mutex(NULL){}
	~a3Mutex() {}

	void create(const wchar_t* name);
	void open(const wchar_t* name);

	void destroy();

	void lock();
	void unlock();
public:
	std::wstring name;
	HANDLE mutex = NULL;

private:
	a3Mutex(const a3Mutex&);
	void operator=(const a3Mutex&);
};


struct a3MutexGuard
{
public:
	a3MutexGuard(a3Mutex& mutex) : mutex(mutex) {
		mutex.lock();
	}

	~a3MutexGuard() {
		mutex.unlock();
	}
public:
	a3Mutex& mutex;
};


struct MessageQueueBuffer
{
public:
	MessageQueueBuffer():fileMapping(0), buffer(NULL), size(0) {}
	~MessageQueueBuffer() {}

	void create(const wchar_t* name, int size);
	void open(const wchar_t* name, int size);

	void destroy();

	char* getBuffer();
	int getBufferSize();

public:
	std::wstring name;
	HANDLE fileMapping;
	char* buffer;
	int size;

private:
	MessageQueueBuffer(const MessageQueueBuffer&);
	void operator=(const MessageQueueBuffer&);
};

class a3MessageQueueIPC
{
public:
	a3MessageQueueIPC(){}
	~a3MessageQueueIPC(){}

	void init(const wchar_t* name, bool create, int msgMaxNum, int msgMaxSize);
	void uninit();

	bool isEmpty();
	bool isFull();
	int  getSize();

	bool enqueue(const a3MessageEntryHead& msg);
	bool dequeue(a3MessageEntryHead& msg);

private:
	std::wstring name;
	a3Mutex mutex;
	MessageQueueBuffer buffer;
	a3MessageQueue queue;

private:
	a3MessageQueueIPC(const a3MessageQueueIPC&);
	void operator=(const a3MessageQueueIPC&);
};