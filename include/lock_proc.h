/*
 *
 * Copyright 2019 skynetcore
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 *
*/


/*
 * brief: this header defines macros that can be used to implement single instance applciations
 * define the following for semaphore based single process instance before including this header
 *
 * #define PROC_USE_LOGGER custom_printf
 * #define PROC_USE_SEMAPHORE
 *
 * if the above is not defined it will take flock based implementation and default
 * debug messages will be logged using printf
 *
*/

#ifndef _LOCK_PROCESS_H_
#define _LOCK_PROCESS_H_


#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>



#define PROC_EXIT(code) exit(code)

/* set custom logger using the following macro */
#ifdef PROC_USE_LOGGER
#define PROC_DBG PROC_USE_LOGGER
#else
#define PROC_DBG printf
#endif /* PROC_USE_LOGGER */

/* messages */
#define PROC_MSG_DUPLICATE "another process already running. exiting \n"
#define PROC_MSG_CLOSURE   "closing process lock. exiting \n"


/* enable PROC_USE_SEMAPHORE to implement process lock using semaphore */
#ifdef PROC_USE_SEMAPHORE

#define PROCESS_LOCK_NAME "/process_lock"
#define LOCKFUNC sem_open
#define LOCKEXIT sem_unlink
#define LOCKWAIT sem_trywait

#define PROC_AUTO   void proc_auto(){ \
                        PROC_DBG(PROC_MSG_CLOSURE); \
                        LOCKEXIT(PROCESS_LOCK_NAME); \
                    }\

#define PROC_LOCK   sem_t *sem; \
                    int locked; \
                    if(SEM_FAILED == (sem = LOCKFUNC(PROCESS_LOCK_NAME, O_CREAT, S_IRWXU, 1))) { \
                        PROC_DBG("sem_open failed error no : %d \n", errno); \
                    } \
                    locked = LOCKWAIT(sem); \
                    if(locked){ \
                        PROC_DBG(PROC_MSG_DUPLICATE); \
                        PROC_EXIT(0); \
                    } atexit(proc_auto);\

#else /* if semaphore based lock is not needed, flock is used */

#include <sys/file.h>
#include <errno.h>

#define PROCESS_LOCK_NAME "/var/run/process_lock.pid"
#define LOCK lock
#define LOCKFUNC open
#define LOCKEXIT unlink

#define PROC_AUTO   void proc_auto(){ \
                        PROC_DBG(PROC_MSG_CLOSURE); \
                        LOCKEXIT(PROCESS_LOCK_NAME); \
                    }\

#define PROC_LOCK   struct flock proc_lock; \
                    int proc_pid = LOCKFUNC(PROCESS_LOCK_NAME, O_CREAT | O_RDWR, 0666); \
                    int rc = f##LOCK(proc_pid, LOCK_EX | LOCK_NB ); \
                    if (rc || (EWOULDBLOCK == errno)) { \
                       PROC_DBG(PROC_MSG_DUPLICATE); \
                       PROC_EXIT(0); \
                    } atexit(proc_auto);\
                    // proc_lock.l_start = 0; \
                    // proc_lock.l_len = 0; \
                    // proc_lock.l_type = F_WRLCK; \
                    // proc_lock.l_whence = SEEK_SET; \
                    // if (fcntl(proc_pid, F_SETLK, &proc_lock) < 0) { \
                    //    PROC_DBG("[flock]"##PROC_MSG_DUPLICATE); \
                    //    PROC_EXIT(0); \
                    // } \


#endif /* PROC_USE_SEMAPHORE */




#endif /* _LOCK_PROCESS_H_ */

