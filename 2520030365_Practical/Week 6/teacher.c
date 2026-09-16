#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>

#define SUBJECTS 5

int main()
{
    const char *shm1 = "/student1_marks";
    const char *shm2 = "/student2_marks";

    int fd1, fd2;
    int *marks1, *marks2;

    int student1[SUBJECTS];
    int student2[SUBJECTS];

    pid_t student1_pid, student2_pid;

    printf("Teacher: Enter marks for Student 1\n");

    for (int i = 0; i < SUBJECTS; i++)
    {
        printf("Subject %d: ", i + 1);
        scanf("%d", &student1[i]);
    }

    printf("\nTeacher: Enter marks for Student 2\n");

    for (int i = 0; i < SUBJECTS; i++)
    {
        printf("Subject %d: ", i + 1);
        scanf("%d", &student2[i]);
    }

    /* Create shared memory for Student 1 */
    shm_unlink(shm1);

    fd1 = shm_open(shm1, O_CREAT | O_RDWR, 0666);

    if (fd1 == -1)
    {
        perror("shm_open student1");
        return 1;
    }

    if (ftruncate(fd1, SUBJECTS * sizeof(int)) == -1)
    {
        perror("ftruncate student1");
        return 1;
    }

    marks1 = mmap(NULL, SUBJECTS * sizeof(int),
                  PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);

    if (marks1 == MAP_FAILED)
    {
        perror("mmap student1");
        return 1;
    }

    /* Create shared memory for Student 2 */
    shm_unlink(shm2);

    fd2 = shm_open(shm2, O_CREAT | O_RDWR, 0666);

    if (fd2 == -1)
    {
        perror("shm_open student2");
        return 1;
    }

    if (ftruncate(fd2, SUBJECTS * sizeof(int)) == -1)
    {
        perror("ftruncate student2");
        return 1;
    }

    marks2 = mmap(NULL, SUBJECTS * sizeof(int),
                  PROT_READ | PROT_WRITE, MAP_SHARED, fd2, 0);

    if (marks2 == MAP_FAILED)
    {
        perror("mmap student2");
        return 1;
    }

    /* Write marks into shared memory */
    for (int i = 0; i < SUBJECTS; i++)
    {
        marks1[i] = student1[i];
        marks2[i] = student2[i];
    }

    printf("\nTeacher: Marks successfully written to shared memory.\n");

    /* Create Student 1 process */
    student1_pid = fork();

    if (student1_pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (student1_pid == 0)
    {
        int total = 0;

        printf("\nStudent 1\n");
        printf("--------------------------------\n");

        /* Student 1 accesses only student1_marks */
        for (int i = 0; i < SUBJECTS; i++)
        {
            printf("Subject %d : %d\n", i + 1, marks1[i]);
            total += marks1[i];
        }

        printf("\nTotal : %d\n", total);
        printf("Average : %.2f\n", total / 5.0);

        munmap(marks1, SUBJECTS * sizeof(int));
        close(fd1);

        exit(0);
    }

    /* Create Student 2 process */
    student2_pid = fork();

    if (student2_pid == -1)
    {
        perror("fork");
        return 1;
    }

    if (student2_pid == 0)
    {
        int total = 0;

        printf("\nStudent 2\n");
        printf("--------------------------------\n");

        /* Student 2 accesses only student2_marks */
        for (int i = 0; i < SUBJECTS; i++)
        {
            printf("Subject %d : %d\n", i + 1, marks2[i]);
            total += marks2[i];
        }

        printf("\nTotal : %d\n", total);
        printf("Average : %.2f\n", total / 5.0);

        munmap(marks2, SUBJECTS * sizeof(int));
        close(fd2);

        exit(0);
    }

    /* Teacher waits for both students */
    waitpid(student1_pid, NULL, 0);
    waitpid(student2_pid, NULL, 0);

    printf("\nTeacher: Both student processes completed.\n");

    /* Clean up */
    munmap(marks1, SUBJECTS * sizeof(int));
    munmap(marks2, SUBJECTS * sizeof(int));

    close(fd1);
    close(fd2);

    shm_unlink(shm1);
    shm_unlink(shm2);

    printf("Teacher: Shared memory cleaned up successfully.\n");

    return 0;
}

