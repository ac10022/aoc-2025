#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LEFT -1
#define RIGHT 1

#define INSTRUCTIONS_SIZE 5000
#define START_INDEX 50

typedef struct {
    int type;
    int value;
} rot;

void processFile(const char* fp);

int main(int n, char** args) {
    if (n != 2) {
        fprintf(stderr, "Invalid number of arguments.");
    }
    else {
        char* fp = args[1];
        processFile(fp);
    }
}

rot* processLine(const char* line) {
    int len = strlen(line);

    int type = 0;
    int value = 0;

    if (len == 0) {
        fprintf(stderr, "Not a valid rot\n");
        exit(1);
    }

    if (line[0] == 'L') {
        type = LEFT;
    }
    else if (line[0] == 'R') {
        type = RIGHT;
    }
    else {
        fprintf(stderr, "Not a valid rot: got %s\n", line);
        exit(1);
    }

    char num[len];
    strncpy(num, line + 1, len - 1);
    num[len - 1] = '\0';

    value = atoi(num);

    rot* newrot = (rot*)malloc(sizeof(rot));

    if (!newrot) {
        fprintf(stderr, "Not enough memory to create a new rot.\n");
        exit(1);
    }

    newrot->type = type;
    newrot->value = value;

    return newrot;
}

long zeroPasses(long index, int type, long val) {
    if (type == RIGHT) {
        return (index + val) / 100;
    }
    else if (type == LEFT) {
        if (index - val > 0) return 0;
        return (labs(index - val) / 100) + (index != 0);
    }
    return 0;
}

void processFile(const char* fp) {
    FILE* openFile = fopen(fp, "r");

    char line[10];
    rot** instructions = (rot**)malloc(INSTRUCTIONS_SIZE * sizeof(rot*));
    int tally = 0;

    if (openFile != NULL) {
        if (tally >= INSTRUCTIONS_SIZE) {
            fprintf(stderr, "Too many instructions to store.\n");
            exit(1);
        }
        while (fgets(line, sizeof(line), openFile)) {
            *(instructions + (tally++)) = processLine(line);
        }
    }
    else {
        fprintf(stderr, "Couldn't open file: %s\n", fp);
        exit(1);
    }

    long index = START_INDEX;
    int zerotally = 0; 

    for (int i = 0; i < tally; i++)
    {
        rot* cur = *(instructions + i);

        zerotally += zeroPasses(index, cur->type, cur->value);

        int change = (cur->type) * (cur->value);
        index += change;
        index %= 100;
        if (index < 0) index += 100;

        free(cur);
    }

    printf("Final zerotally: %d\n", zerotally);
    
    free(instructions);
    fclose(openFile);
}
