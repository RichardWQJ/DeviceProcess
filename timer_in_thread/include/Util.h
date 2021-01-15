#ifndef VIEW_UTIL_H
#define VIEW_UTIL_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <memory>
#include <unordered_map>
#include <map>
#include <thread>
#include <mutex>
#include <vector>
#include <set>
#include <list>
#include <queue>
#include <chrono>
#include <sys/time.h>
#include <time.h>
#include <semaphore.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>

using namespace std;

#define IS_DEBUG_ON 1
#define COMPONENT_NAME "TEST"

typedef struct
{
    char data[2048];
    pthread_mutex_t mtxSharedMemData;
} SharedMem;

#define SHARE_MEM_NAME "/myshareddata"

#define EOK 0

#define NONE "\033[m\r\n"
#define RED "\033[0;32;31m"
#define LIGHT_RED "\033[1;31m"
#define GREEN "\033[0;32;32m"
#define LIGHT_GREEN "\033[1;32m"
#define BLUE "\033[0;32;34m"
#define LIGHT_BLUE "\033[1;34m"
#define DARY_GRAY "\033[1;30m"
#define CYAN "\033[0;36m"
#define LIGHT_CYAN "\033[1;36m"
#define PURPLE "\033[0;35m"
#define LIGHT_PURPLE "\033[1;35m"
#define BROWN "\033[0;33m"
#define YELLOW "\033[1;33m"
#define LIGHT_GRAY "\033[0;37m"
#define WHITE "\033[1;37m"

#define DebugPrint(fmt, ...)                                                                                                                                                                             \
    if (IS_DEBUG_ON)                                                                                                                                                                                     \
    {                                                                                                                                                                                                    \
        time_t logTime = time(NULL);                                                                                                                                                                     \
        struct tm *currTime = localtime(&logTime);                                                                                                                                                       \
        printf("[DEBUG][%s]:[%02d:%02d:%02d]:[%lld]:[%s]:[%d]" fmt, COMPONENT_NAME, currTime->tm_hour, currTime->tm_min, currTime->tm_sec, std::this_thread::get_id(), __func__, __LINE__, ##__VA_ARGS__); \
    }

#define InfoPrint(fmt, ...)                                                                                                                                                                                      \
    if (IS_DEBUG_ON)                                                                                                                                                                                             \
    {                                                                                                                                                                                                            \
        time_t logTime = time(NULL);                                                                                                                                                                             \
        struct tm *currTime = localtime(&logTime);                                                                                                                                                               \
        printf("%s[INFO][%s]:[%02d:%02d:%02d]:[%lld]:[%s]:[%d]" fmt, GREEN, COMPONENT_NAME, currTime->tm_hour, currTime->tm_min, currTime->tm_sec, std::this_thread::get_id(), __func__, __LINE__, ##__VA_ARGS__); \
        printf(NONE);                                                                                                                                                                                            \
    }

#define WarnPrint(fmt, ...)                                                                                                                                                                                          \
    if (IS_DEBUG_ON)                                                                                                                                                                                                 \
    {                                                                                                                                                                                                                \
        time_t logTime = time(NULL);                                                                                                                                                                                 \
        struct tm *currTime = localtime(&logTime);                                                                                                                                                                   \
        printf("%s[WARN][%s]:[%02d:%02d:%02d]:[%lld]:[%s]:[%d]" fmt, DARY_GRAY, COMPONENT_NAME, currTime->tm_hour, currTime->tm_min, currTime->tm_sec, std::this_thread::get_id(), __func__, __LINE__, ##__VA_ARGS__); \
        printf(NONE);                                                                                                                                                                                                \
    }

#define ErrPrint(fmt, ...)                                                                                                                                                                                      \
    if (IS_DEBUG_ON)                                                                                                                                                                                            \
    {                                                                                                                                                                                                           \
        time_t logTime = time(NULL);                                                                                                                                                                            \
        struct tm *currTime = localtime(&logTime);                                                                                                                                                              \
        printf("%s[ERROR][%s]:[%02d:%02d:%02d]:[%lld]:[%s]:[%d]" fmt, RED, COMPONENT_NAME, currTime->tm_hour, currTime->tm_min, currTime->tm_sec, std::this_thread::get_id(), __func__, __LINE__, ##__VA_ARGS__); \
        printf(NONE);                                                                                                                                                                                           \
    }

#define CriticalPrint(fmt, ...)                                                                                                                                                                                          \
    if (IS_DEBUG_ON)                                                                                                                                                                                                     \
    {                                                                                                                                                                                                                    \
        time_t logTime = time(NULL);                                                                                                                                                                                     \
        struct tm *currTime = localtime(&logTime);                                                                                                                                                                       \
        printf("%s[CRITICAL][%s]:[%02d:%02d:%02d]:[%lld]:[%s]:[%d]" fmt, LIGHT_RED, COMPONENT_NAME, currTime->tm_hour, currTime->tm_min, currTime->tm_sec, std::this_thread::get_id(), __func__, __LINE__, ##__VA_ARGS__); \
        printf(NONE);                                                                                                                                                                                                    \
    }

#endif // !VIEW_UTIL_H
