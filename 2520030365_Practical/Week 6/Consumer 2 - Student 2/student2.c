#include <stdio.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>

#define SUBJECTS 5

int main()
{
    const char *shm_name = "/student2_marks";
    int fd;
    int *marks;
    int total = 0;

    fd = shm_open(shm_name, O_RDONLY, 0666);

    if (fd == -1)
    {
        perror("shm_open");
        return 1;
    }

    marks = mmap(NULL, SUBJECTS * sizeof(int),
                 PROT_READ, MAP_SHARED, fd, 0);

    if (marks == MAP_FAILED)
    {
        perror("mmap");
        return 1;
    }

    printf("\nStudent 2\n");
    printf("--------------------------------\n");

    for (int i = 0; i < SUBJECTS; i++)
    {
        printf("Subject %d : %d\n", i + 1, marks[i]);
        total += marks[i];
    }

    printf("\nTotal : %d\n", total);
    printf("Average : %.2f\n", total / 5.0);

    munmap(marks, SUBJECTS * sizeof(int));
    close(fd);

    return 0;
}
