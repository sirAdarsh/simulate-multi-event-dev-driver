// user_test.c
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <string.h>

#define THREADS 4
#define READ_SIZE 128

int fd;

void* reader_thread(void *arg)
{
    char buf[READ_SIZE];
    int id = *(int*)arg;

    while (1) {
        ssize_t n = read(fd, buf, sizeof(buf)-1);
        if (n > 0) {
            buf[n] = '\0';
            printf("[Thread %d] Read: %s\n", id, buf);
        } else {
            // No data — wait briefly
            usleep(50000);
        }
    }
    return NULL;
}

int main()
{
    fd = open("/dev/simirq", O_RDONLY);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    printf("Opened /dev/simirq successfully.\n");

    pthread_t t[THREADS];
    int ids[THREADS];

    for (int i = 0; i < THREADS; i++) {
        ids[i] = i;
        pthread_create(&t[i], NULL, reader_thread, &ids[i]);
    }

    printf("Spawned %d reader threads. Reading events...\n", THREADS);

    // Keep program alive
    for (int i = 0; i < THREADS; i++)
        pthread_join(t[i], NULL);

    return 0;
}
