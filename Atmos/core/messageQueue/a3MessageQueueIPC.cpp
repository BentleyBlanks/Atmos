#include <core/messageQueue/a3MessageQueueIPC.h>

#include <cassert>

void a3Mutex::create(const wchar_t* name)
{
	mutex = ::CreateMutex(NULL, false, name);
	assert(mutex);

	this->name = name;
}

void a3Mutex::open(const wchar_t* name)
{
	mutex = ::OpenMutex(SYNCHRONIZE, false, name);
	assert(mutex);

	this->name = name;
}

void a3Mutex::destroy()
{
	if (mutex) {
		::CloseHandle(mutex);
		mutex = NULL;
	}
	name.clear();
}

void a3Mutex::lock()
{
	if (mutex) {
		::WaitForSingleObject(mutex, INFINITE);
	}
}

void a3Mutex::unlock()
{
	if (mutex) {
		::ReleaseMutex(mutex);
	}
}


void MessageQueueBuffer::create(const wchar_t* _name, int _size)
{
    // 创建共享内存块
	fileMapping = ::CreateFileMapping(
		INVALID_HANDLE_VALUE,
		NULL,
		PAGE_READWRITE,
		0, _size,
		_name);
	
	assert(fileMapping);

    // 获取共享内存指针
	buffer = (char*)::MapViewOfFile(fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	assert(buffer);

	size   = _size;
	name    = _name;
}

void MessageQueueBuffer::open(const wchar_t* _name, int _size)
{
	fileMapping = ::OpenFileMapping(FILE_MAP_ALL_ACCESS, false, _name);
	assert(fileMapping);

	buffer = (char*)::MapViewOfFile(fileMapping, FILE_MAP_ALL_ACCESS, 0, 0, 0);
	assert(buffer);

	size = _size;
	name = _name;
}

void MessageQueueBuffer::destroy()
{
	if (buffer) {
		::UnmapViewOfFile(buffer);
		buffer = NULL;
	}
	if (fileMapping) {
		CloseHandle(fileMapping);
		fileMapping = NULL;
	}
	size = 0;
	name.clear();
}

char* MessageQueueBuffer::getBuffer()
{
	return buffer;
}

int MessageQueueBuffer::getBufferSize()
{
	return size;
}

void a3MessageQueueIPC::init(const wchar_t* _name, bool create, int msg_max_num, int msg_max_size)
{
	std::wstring mq_name(_name);
	std::wstring mq_lock_name = mq_name + std::wstring(L"_lock");

    // 消息取最大跨距做单一消息内存分配
	int buffer_size = sizeof(a3MessageQueueHead) + (msg_max_num+1) * msg_max_size;
	if (!create) {
		buffer.open(mq_name.c_str(), buffer_size);
		mutex.open(mq_lock_name.c_str());
	}
	else {
		buffer.create(mq_name.c_str(), buffer_size);
		mutex.create(mq_lock_name.c_str());
	}
	queue.init(buffer.getBuffer(), buffer_size, msg_max_size);

	this->name = mq_name;
}

void a3MessageQueueIPC::uninit()
{
	queue.uninit();
	buffer.destroy();
	mutex.destroy();
}

bool a3MessageQueueIPC::isEmpty()
{
	a3MutexGuard guard(mutex);

	return queue.isEmpty();
}

bool a3MessageQueueIPC::isFull()
{
	a3MutexGuard guard(mutex);

	return queue.isFull();
}

int a3MessageQueueIPC::getSize()
{
	a3MutexGuard guard(mutex);

	return queue.getSize();
}

bool a3MessageQueueIPC::enqueue(const a3MessageEntryHead& msg)
{
	a3MutexGuard guard(mutex);

	return queue.enqueue(msg);
}

bool a3MessageQueueIPC::dequeue(a3MessageEntryHead& msg)
{
	a3MutexGuard guard(mutex);

	return queue.dequeue(msg);
}
