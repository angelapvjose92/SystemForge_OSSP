#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define INITIAL_SIZE 16

typedef struct Node
{
    char *command;
    struct Node *next;
} Node;

/* Add command to history */
void addHistory(Node **head, const char *command)
{
    Node *newNode = malloc(sizeof(Node));

    if (newNode == NULL)
    {
        printf("Memory allocation failed.\n");
        return;
    }

    newNode->command = malloc(strlen(command) + 1);

    if (newNode->command == NULL)
    {
        free(newNode);
        printf("Memory allocation failed.\n");
        return;
    }

    strcpy(newNode->command, command);

    newNode->next = *head;
    *head = newNode;
}

/* Display command history */
void displayHistory(Node *head)
{
    int number = 1;

    while (head != NULL)
    {
        printf("%d  %s\n", number, head->command);
        head = head->next;
        number++;
    }
}

/* Free linked list memory */
void freeHistory(Node *head)
{
    Node *temp;

    while (head != NULL)
    {
        temp = head;
        head = head->next;

        free(temp->command);
        free(temp);
    }
}

int main()
{
    char *buffer;
    int capacity = INITIAL_SIZE;
    Node *history = NULL;

    buffer = malloc(capacity);

    if (buffer == NULL)
    {
        printf("Memory allocation failed.\n");
        return 1;
    }

    while (1)
    {
        int position = 0;

        printf("myShell> ");
        fflush(stdout);

        while (1)
        {
            int c = getchar();

            if (c == EOF)
            {
                free(buffer);
                freeHistory(history);
                return 0;
            }

            /* Enter key */
            if (c == '\n')
            {
                buffer[position] = '\0';
                printf("\n");

                if (position == 0)
                {
                    break;
                }

                /* Exit command */
                if (strcmp(buffer, "exit") == 0)
                {
                    printf("Exiting shell...\n");

                    free(buffer);
                    freeHistory(history);

                    return 0;
                }

                /* History command */
                if (strcmp(buffer, "history") == 0)
                {
                    displayHistory(history);
                }
                else if (strcmp(buffer, "help") == 0)
                {
                    printf("Available commands:\n");
                    printf("help\n");
                    printf("history\n");
                    printf("exit\n");
                }
                else
                {
                    printf("Command: %s\n", buffer);
                }

                addHistory(&history, buffer);

                break;
            }

            /* Backspace */
            if (c == 127 || c == '\b')
            {
                if (position > 0)
                {
                    position--;
                    printf("\b \b");
                    fflush(stdout);
                }

                continue;
            }

            /* Dynamic buffer resizing */
            if (position >= capacity - 1)
            {
                capacity = capacity * 2;

                char *temp = realloc(buffer, capacity);

                if (temp == NULL)
                {
                    printf("\nMemory allocation failed.\n");

                    free(buffer);
                    freeHistory(history);

                    return 1;
                }

                buffer = temp;
            }

            buffer[position] = c;
            position++;

            putchar(c);
            fflush(stdout);
        }
    }

    free(buffer);
    freeHistory(history);

    return 0;
}
