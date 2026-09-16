#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <errno.h>

#define REQUEST_FIFO "client_to_server"
#define RESPONSE_FIFO "server_to_client_%d"
#define BUFFER_SIZE 256

typedef struct {
    pid_t client_pid;
    char message[BUFFER_SIZE];
} Request;

void handle_signal(int sig)
{
    printf("\nClient received signal %d.\n", sig);
    printf("Client shutting down...\n");
    exit(0);
}

int main()
{
    Request request;
    char response[BUFFER_SIZE];
    char response_fifo[BUFFER_SIZE];

    pid_t client_pid = getpid();

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    snprintf(response_fifo,
             sizeof(response_fifo),
             RESPONSE_FIFO,
             client_pid);

    if (mkfifo(response_fifo, 0666) == -1 && errno != EEXIST)
    {
        perror("mkfifo");
        return 1;
    }

    printf("Client started. PID = %d\n", client_pid);

    int request_fd = open(REQUEST_FIFO, O_WRONLY);

    if (request_fd == -1)
    {
        perror("Cannot connect to server");
        unlink(response_fifo);
        return 1;
    }

    printf("Connected to server.\n");

    while (1)
    {
        printf("\nEnter message (type 'exit' to quit): ");
        fflush(stdout);

        if (fgets(request.message, BUFFER_SIZE, stdin) == NULL)
        {
            break;
        }

        request.message[strcspn(request.message, "\n")] = '\0';

        if (strcmp(request.message, "exit") == 0)
        {
            break;
        }

        request.client_pid = client_pid;

        write(request_fd, &request, sizeof(request));

        int response_fd = open(response_fifo, O_RDONLY);

        if (response_fd == -1)
        {
            perror("Opening response FIFO");
            break;
        }

        memset(response, 0, sizeof(response));

        read(response_fd, response, sizeof(response));

        printf("Server: %s\n", response);

        close(response_fd);
    }

    close(request_fd);
    unlink(response_fifo);

    printf("Client terminated.\n");

    return 0;
}
