#ifndef POSIX_PROCESS_H
#define POSIX_PROCESS_H

#include <windows.h>

struct PosixThreadStart
{
	unsigned(__stdcall* start)(void*);
	void* argument;
	PosixHandle* handle;
};

inline void* posixThreadEntry(void* data)
{
	PosixThreadStart* start = (PosixThreadStart*) data;
	start->start(start->argument);
	PosixHandle* h = start->handle;
	delete start;
	pthread_mutex_lock(&h->mutex);
	h->finished = true;
	pthread_cond_broadcast(&h->cond);
	pthread_mutex_unlock(&h->mutex);
	return NULL;
}

inline uintptr_t _beginthreadex(
	void*, unsigned stackSize, unsigned(__stdcall* start)(void*), void* argument, unsigned, unsigned* threadId
)
{
	PosixHandle* h = newPosixHandle(PosixHandle::THREAD);
	PosixThreadStart* data = new PosixThreadStart();
	data->start = start;
	data->argument = argument;
	data->handle = h;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	if (stackSize) {
		pthread_attr_setstacksize(&attr, stackSize < 65536 ? 65536 : stackSize);
	}
	if (pthread_create(&h->thread, &attr, posixThreadEntry, data) != 0) {
		pthread_attr_destroy(&attr);
		delete data;
		CloseHandle(h);
		return 0;
	}
	pthread_attr_destroy(&attr);
	if (threadId) {
		*threadId = (unsigned) (uintptr_t) h->thread;
	}
	return (uintptr_t) h;
}

#endif // POSIX_PROCESS_H
