/*
 * Thread.cpp
 *
 *  Created on: 2017-3-27
 *      Author: Administrator
 */

#include "Thread.h"

#if 0
Thread::Thread()
{
	// TODO Auto-generated constructor stub
	tid = 0;
	threadStatus = THREAD_STATUS_NEW;
}
void *Thread::run1()
{
	threadStatus = THREAD_STATUS_RUNNING;
	tid = pthread_self();
	threadStatus = THREAD_STATUS_EXIT;
	tid = 0;
	pthread_exit(NULL);
};
void * thread_proxy_func(void *args)
{
	Thread * pThread = static_cast<Thread *>(args);
	pThread->run();
	return  NULL;
};
bool Thread::start()
{
	int iRet = 0;
	pthread_create(&tid,NULL,thread_proxy_func,this);
};
pthread_t Thread::getThreadID()
{
	return tid;
};

int Thread::getState()
{
	return threadStatus;
};

void Thread::join()
{
	if(tid>0)
	{
		pthread_join(tid,NULL);
	}
};

void Thread::detach()
{
	if(tid>0)
	{
		pthread_detach(tid);
	}
};

void Thread::join(unsigned long millisTime)
{
	if(tid==0)
	{
		return ;
	}
	if(millisTime ==0)
	{
		join();
	}
	else
	{
		unsigned long k=0;
		while(threadStatus!=THREAD_STATUS_EXIT&&k<millisTime)
		{
			usleep(100);
			k++;
		}
	}
};
Thread::~Thread()
{
	// TODO Auto-generated destructor stub
}

#endif

#if 1

Thread::Thread()
{
	this->isInterript = false;
}

Thread::~Thread()
{
	if (!this->isInterrupted())
	{
		this->interrupt();
	}

	if (this->th.joinable())
	{
		this->th.join();
	}
}

void Thread::start()
{
	std::thread thr(std::bind(&Thread::run, this));
	this->th = std::move(thr);
}

std::thread::id Thread::getId()
{
	return this->th.get_id();
}

void Thread::interrupt()
{
	this->isInterript = true;
}

bool Thread::isInterrupted()
{
	return this->isInterript;
}

void Thread::join()
{
	this->th.join();
}

void Thread::run()
{
}

#endif