#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 100

int main()
{
    char input[BUFFER_SIZE];

    while (1)
    {
        printf("myShell> ");
        fflush(stdout);

        fgets(input, BUFFER_SIZE, stdin);

        input[strcspn(input, "\n")] = '\0';

        if (strcmp(input, "exit") == 0)
        {
            printf("Exiting shell...\n");
            break;
        }

        if (strlen(input) == 0)
        {
            continue;
        }

        printf("You entered: %s\n", input);
    }

    return 0;
}
