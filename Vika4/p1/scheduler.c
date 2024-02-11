#include "scheduler.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>

typedef struct _QueueItem {
    /*
     * The tid of this item.
     */
    int tid;
    /*
     * The next item in the queue.
     * NULL if there is no next item.
     */
    struct _QueueItem *next;
} QueueItem;

typedef struct _Queue {
    /*
     * The first item of the queue.
     * NULL if the queue is empty.
     */
    QueueItem *head;
    /*
     * The last item of the queue.
     * undefined if the queue is empty.
     */
    QueueItem *tail;
} Queue;

typedef enum _ThreadState {
    STATE_UNUSED = 0, // This entry in the _threads array is unused.

    STATE_READY,      // The thread is ready and should be on a ready queue for
                      //   selection by the scheduler
    STATE_RUNNING,    // The thread is running and should not be on a ready queue
    STATE_WAITING     // The thread is blocked and should not be on a ready queue
} ThreadState;

typedef struct _Thread {
    int threadId;
    ThreadState state;
    /*
     * Range: 0 ... MAX_PRIORITY (including)
     * 0 is highest, MAX_PRIORITY is lowest priority
     */
    int priority;
} Thread;

Thread _threads[MAX_THREADS] = {{0}};
Queue all_queues[MAX_PRIORITY] = {0}; // INFO: Just have an arrau of queues, their index indicates their priority.
short count_queues[MAX_PRIORITY] = {0}; // this array keeps track of how many times each queue has gotten to do their turn.
                                        // Values are modified by scheduleNextThread() and is purely used to keep track of the num. of executions.

/* TODO: Add global variables if needed. */

/*
 * Adds a new, waiting thread.
 */
int startThread(int threadId, int priority)
{
    if (((threadId < 0) || (threadId >= MAX_THREADS) ||
        (_threads[threadId].state != STATE_UNUSED)) ||
        (priority < 0) || (priority > MAX_PRIORITY)) {

        return -1;
    }

    _threads[threadId].threadId = threadId;
    _threads[threadId].state    = STATE_WAITING;
    _threads[threadId].priority = priority;
    return 0;
}

/*
 * Append to the tail of the queue.
 * Does nothing on error.
 */
void _enqueue(Queue *queue, int tid)
{
    QueueItem *item = (QueueItem *)malloc(sizeof(QueueItem));
    
    if (item == NULL)  // In case malloc fails.
    {
        return;
    }

    item->tid = tid;
    item->next = NULL;
    
    if (queue->head == NULL) 
    {
        queue->head = item;
        queue->tail = item;
    }
    else
    {
        queue->tail->next = item;
        queue->tail = item;
    }
    return;
}

/*
 * Remove and get the head of the queue.
 * Return -1 if the queue is empty.
 */
int _dequeue(Queue *queue)
{
    if (queue->head == NULL)
    {
        return -1;
    }
    else
    {
        QueueItem *removedItem = queue->head;
        int tempVal = removedItem->tid;
        queue->head = queue->head->next;
        free(removedItem);
        if (queue->head == NULL)
        {
            queue->tail = NULL;
        }
        return tempVal;
    }
}

void initScheduler()
{
    // TODO: Implement if you need to initialize any global variables you added
}

/*
 * Called whenever a waiting thread gets ready to run.
 */
void onThreadReady(int threadId)
{
    if ((threadId < 0) || (threadId >= MAX_THREADS) ||
        (_threads[threadId].state != STATE_WAITING)) {
        return;
    }

    _threads[threadId].state    = STATE_READY;
    int priority = _threads[threadId].priority;
    _enqueue(&all_queues[priority], threadId);
}

/*
 * Called whenever a running thread is forced of the CPU
 * (e.g., through a timer interrupt).
 */
void onThreadPreempted(int threadId)
{
    if ((threadId < 0) || (threadId >= MAX_THREADS) ||
        (_threads[threadId].state != STATE_RUNNING)) {
        return;
    }

    _threads[threadId].state    = STATE_READY;
    int priority = _threads[threadId].priority;
    _enqueue(&all_queues[priority], threadId);
}

/*
 * Called whenever a running thread needs to wait.
 */
void onThreadWaiting(int threadId)
{
    if ((threadId < 0) || (threadId >= MAX_THREADS) ||
        (_threads[threadId].state != STATE_RUNNING)) {
        return;
    }

    _threads[threadId].state    = STATE_WAITING;
}

/*
 * Gets the id of the next thread to run and sets its state to running.
 */
int scheduleNextThread()
{
    int tid = -1;
    int lastQueueWhichCanRun = -1;
    for (int i = 0; i < MAX_PRIORITY; i++)
    {
        if (all_queues[i].head != NULL)
        {
            if (lastQueueWhichCanRun < i)
            {
                lastQueueWhichCanRun = i;
            }
            count_queues[i]++;
            if (i == (MAX_PRIORITY-1)) // Don't ever have to skip lowest priority of queue
            {
                count_queues[i] = 0;
            }
            if (count_queues[i] != 5) // Starvation prevention
            {
                tid = _dequeue(&all_queues[i]);
                _threads[tid].state    = STATE_RUNNING;
                break;
            }
            else // Resetting starvation prevention since it skipped this time
            {
                count_queues[i] = 0;
            }
        }
        else // If a lower ranked process got the chance to run, but had nothing to run, then the 
             // higher priority queue should know it got the chance and should reset its queue.
        {
            if (i != 0) // ...as long as it aint the highest priority queue, since there is no -1 queue.
            {
                count_queues[i-1] = 0;
            }
        }
    }
    if (lastQueueWhichCanRun != -1 && tid == -1) // If there are queues in the system, but they were all skipped, run the lowest ranked one.
    {
        count_queues[lastQueueWhichCanRun]++;
        tid = _dequeue(&all_queues[lastQueueWhichCanRun]);
        _threads[tid].state    = STATE_RUNNING;
    }
    return tid;
}


#if 0
int main() {
	// Initially empty queue (head and tail is NULL)
	Queue q = {NULL,NULL};

	_enqueue( &q, 42 );
	_enqueue( &q, 99 );
	int x = _dequeue( &q );
	printf("Expect: 42, and I got: %d\n", x);

	x = _dequeue( &q );
	printf("Expect: 99, and I got: %d\n", x);
}
#endif
