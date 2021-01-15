/*
 * Thread.h
 *
 *  Created on: 2017-3-27
 *      Author: Administrator
 */

#ifndef THREAD_H_
#define THREAD_H_

#if 0
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

using namespace std;

class Thread
{
private:
	pthread_t tid;
	int threadStatus;
	friend void *thread_proxy_func(void *arg);
	void *run1();

public:
	static const int THREAD_STATUS_NEW = 0;
	static const int THREAD_STATUS_RUNNING = 1;
	static const int THREAD_STATUS_EXIT = -1;
	Thread();
	virtual void run() = 0;
	bool start();
	pthread_t getThreadID();
	int getState();
	void join();
	void join(unsigned long millisTime);
	void detach();
	virtual ~Thread();
};
#endif

#if 1
#include <iostream>
#include <thread>
#include <atomic>
#include <functional>

class Thread
{
public:
	Thread();
	virtual ~Thread();

	void start();
	std::thread::id getId();
	void interrupt();
	bool isInterrupted();
	void join();
	virtual void run();

private:
	std::atomic<bool> isInterript;
	std::thread th;
};
#endif

#endif /* THREAD_H_ */
