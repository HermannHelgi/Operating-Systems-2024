#include "message_queue.h"
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <stdlib.h>
#include <stdio.h>

/*
 * The commands supported by the server
 */
typedef enum _Command {
    CmdAdd = 0x00,     // Adds the two message parameters
    CmdMul,            // Multiples the two message parameters
    CmdExit            // Stops the server
} Command;

/*
 * The message format to be sent to the server.
 */
typedef struct _Message {
    /*
     * One of the command constants.
     */
    Command command;
    /*
     * Used as operand 1 (if required)
     */
    int parameter1;
    /*
     * Used as operand 2 (if required)
     */
    int parameter2;
} Message;

#define FORMAT_STRING_ADD      "Calc: %d + %d = %d\n"
#define FORMAT_STRING_MUL      "Calc: %d * %d = %d\n"

mqd_t startClient(void)
{
    mqd_t client = mq_open("/QUEUE_NAME", O_RDWR);
    
    return client;
}

int sendExitTask(mqd_t client)
{
    Message exit_message;
    exit_message.command = CmdExit;

    if (mq_send(client, (const char*)&exit_message, sizeof(exit_message), 0) == -1)
    {
        return -1;
    }
    return 0;
}

int sendAddTask(mqd_t client, int operand1, int operand2)
{
    Message add_message;
    add_message.command = CmdAdd;
    add_message.parameter1 = operand1;
    add_message.parameter2 = operand2;

    return mq_send(client, (const char*)&add_message, sizeof(add_message), 0);
}

int sendMulTask(mqd_t client, int operand1, int operand2)
{
    Message mul_message;
    mul_message.command = CmdMul;
    mul_message.parameter1 = operand1;
    mul_message.parameter2 = operand2;

    return mq_send(client, (const char*)&mul_message, sizeof(mul_message), 0);
}

int stopClient(mqd_t client)
{
    mq_close(client);

    return 0;
}

int runServer(void)
{
    int didExit = 0, hadError = 0;
    Message msg;

    struct mq_attr attr;
    attr.mq_flags   = 0;
    attr.mq_maxmsg  = 10;           // Maximum number of messages in the queue
    attr.mq_msgsize = sizeof(msg);  // Maximum message size
    attr.mq_curmsgs = 0;
    (void) attr;

    // Creates and open the message queue. Server only needs to read from it.
    // Clients only need to write to it, allow for all users.

    mqd_t server = mq_open("/QUEUE_NAME", O_CREAT | O_RDWR, 0666, &attr);
    if(server == -1) {
	    return -1;
    }


    // This is the implementation of the server part, already completed:
    // TODO: You may have to make minor extensions in order to satisfy all requirements
    do {
        // Attempt to receive a message from the queue.
        ssize_t received = mq_receive(server, (char*)&msg, sizeof(msg), NULL);
        if (received != sizeof(msg)) {
            // This implicitly also checks for error (i.e., -1)
            hadError = 1;
            continue;
        }

        switch (msg.command)
        {
            case CmdExit:
                // End this loop.
                didExit = 1;
                break;

            case CmdAdd:
                // Print the required output.
                printf(FORMAT_STRING_ADD,
                       msg.parameter1,
                       msg.parameter2,
                       msg.parameter1 + msg.parameter2);
                break;

            case CmdMul:
                // Print the required output.
                printf(FORMAT_STRING_MUL,
                       msg.parameter1,
                       msg.parameter2,
                       msg.parameter1 * msg.parameter2);
                break;

            default:
                break;
        }
    } while (!didExit);

    mq_close(server);
    mq_unlink("/QUEUE_NAME");

    return hadError ? -1 : 0;
}
