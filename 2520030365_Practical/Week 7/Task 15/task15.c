#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int pipefd[2];
    pid_t producer, consumer;

    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return 1;
    }

    producer = fork();

    if (producer == -1)
    {
        perror("fork");
        return 1;
    }

    if (producer == 0)
    {
        // Producer process
        close(pipefd[0]);

        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        execlp("ls", "ls", "-l", NULL);

        perror("execlp ls");
        exit(1);
    }

    consumer = fork();

    if (consumer == -1)
    {
        perror("fork");
        return 1;
    }

    if (consumer == 0)
    {
        // Consumer process
        close(pipefd[1]);

        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);

        execlp("grep", "grep", ".c", NULL);

        perror("execlp grep");
        exit(1);
    }

    // Parent process
    close(pipefd[0]);
    close(pipefd[1]);

    waitpid(producer, NULL, 0);
    waitpid(consumer, NULL, 0);

    printf("Producer and consumer processes completed.\n");

    return 0;
}
