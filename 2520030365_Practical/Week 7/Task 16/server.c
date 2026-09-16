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

int server_fd;

void handle_signal(int sig)
{
    printf("\nServer received signal %d.\n", sig);

    close(server_fd);
    unlink(REQUEST_FIFO);

    printf("Server shutting down...\n");
    exit(0);
}

int main()
{
    Request request;
    char response[BUFFER_SIZE];
    char response_fifo[BUFFER_SIZE];

    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);

    if (mkfifo(REQUEST_FIFO, 0666) == -1 && errno != EEXIST)
    {
        perror("mkfifo");
        return 1;
    }

    printf("Server started.\n");
    printf("Waiting for clients...\n");

    server_fd = open(REQUEST_FIFO, O_RDONLY);

    if (server_fd == -1)
    {
        perror("open");
        unlink(REQUEST_FIFO);
        return 1;
    }

    while (1)
    {
        ssize_t bytes_read = read(server_fd, &request, sizeof(request));

        if (bytes_read > 0)
        {
            printf("\nClient %d: %s\n",
                   request.client_pid,
                   request.message);

            snprintf(response_fifo,
                     sizeof(response_fifo),
                     RESPONSE_FIFO,
                     request.client_pid);

            snprintf(response,
                     sizeof(response),
                     "Server received your message: %s",
                     request.message);

            int response_fd = open(response_fifo, O_WRONLY);

            if (response_fd != -1)
            {
                write(response_fd, response, strlen(response) + 1);
                close(response_fd);
            }
            else
            {
                perror("Opening response FIFO");
            }
        }
        else if (bytes_read == -1 && errno != EINTR)
        {
            perror("read");
            break;
        }
    }

    close(server_fd);
    unlink(REQUEST_FIFO);

    return 0;
}
