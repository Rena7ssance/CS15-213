//
//  main.c
//  CSAPP
//
//  Created by 复甦Roy7wt on 4/19/17.
//  Copyright © 2017 SJTU. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <time.h>
#include <math.h>

#include "cachelab.h"

struct oneSet {
    int *validBit;
    long *tagBit;
    clock_t *lastAccessTime;
};

int getset(unsigned long *);
long gettag(unsigned long *);

void get_opt(int, char **);
void init();
void execute();
void func_free();
void load(unsigned long  *, int);
void store(unsigned long *, int);
void modify(unsigned long *, int);

struct oneSet *cache;
int s, E, b;
int hit, miss, evict;
char *file = NULL;

int main(int argc, char * argv[]) {
    get_opt(argc, argv);
    init();
    execute();
    func_free();
    
    printSummary(hit, miss, evict);
    return 0;
    
}

void get_opt(int argc, char **argv)
{
    int c;
    while ((c = getopt(argc, argv, "s:E:b:t:")) != -1)
    {
        switch (c)
        {
            case 's':
                s = atoi(optarg);
                break;
            case 'E':
                E = atoi(optarg);
                break;
            case 'b':
                b = atoi(optarg);
                break;
            case 't':
                file = optarg;
                break;
            default:
                printf("illegal opt\n");
                exit(0);
        }
    }
}

void execute() {
    FILE *pfile = fopen(file, "r");
    char accessType;
    unsigned long address;
    int size;
    while (fscanf(pfile, " %c %lx,%d", &accessType, &address, &size) > 0) {
        switch (accessType) {
            case 'L':
                load(&address, size);
                break;
            case 'S':
                store(&address, size);
                break;
			case 'M':
				modify(&address, size);
            default:
                break;
        }
    }
    fclose(pfile);
}

int getset(unsigned long *addr) {
    return (int) ((*addr >> b) & ((1 << s)-1));
}

long gettag(unsigned long *addr) {
    int tagShift = (s + b);
    return (long) (*addr >> tagShift);
}

void init() {
    hit = 0; miss = 0; evict = 0;
    int numberOfSet = 1 << s;
    if (numberOfSet <= 0) {
        fprintf(stderr, "number of set is not positive");
        exit(-1);
    }
    cache = (struct oneSet*)malloc(sizeof(struct oneSet) * numberOfSet);
    if (cache == NULL) {
        fprintf(stderr, "cache fails to init");
        exit(-1);
    }
    for (int i = 0; i < numberOfSet; i++) {
        cache[i].validBit = (int *)malloc(sizeof(int) * E);
        cache[i].tagBit = (long *)malloc(sizeof(long) * E);
        cache[i].lastAccessTime = (clock_t *)malloc(sizeof(clock_t) * E);
        
        for (int j = 0; j < E; j++) {
            cache[i].validBit[j] = (int)0;
            cache[i].tagBit[j] = (long)0;
            cache[i].lastAccessTime[j] = (clock_t)0;
        }
    }
}

void func_free() {
    int numberOfSet = 1 << s;
    for (int i = 0; i < numberOfSet; i++) {
        free(cache[i].validBit);
        free(cache[i].tagBit);
        free(cache[i].lastAccessTime);
    }
    free(cache);
}


void load(unsigned long *addr, int size) {
    
    int setNumber = getset(addr);
    long tagNumber = gettag(addr);
    struct oneSet *set = &cache[setNumber];
    
    int isFull = 1;
    int isHit = 0;
    int emtpyIndex= -1;
    int lastTimeIndex = 0;
    clock_t lastTime = set->lastAccessTime[0];
    
    for (int i = 0; i < E; i++) {
        if (set->validBit[i] == 1 && set->tagBit[i] == tagNumber) {
            isHit = 1;
            set->lastAccessTime[i] = clock();
            break;
        }
        if (isFull == 1 && set->validBit[i] == 0) { // empty block
            emtpyIndex = i;
            isFull = 0;
        }
        if (set->lastAccessTime[i] < lastTime) {
            lastTime = set->lastAccessTime[i];
            lastTimeIndex = i;
        }
    }
    
    if (isHit) {
        hit += 1;
        return;
    } else {
        miss += 1;
        if (isFull) {
            set->validBit[lastTimeIndex] = 1;
            set->tagBit[lastTimeIndex] = tagNumber;
            set->lastAccessTime[lastTimeIndex] = clock();
            evict += 1;
        } else {
            set->validBit[emtpyIndex] = 1;
            set->tagBit[emtpyIndex] = tagNumber;
            set->lastAccessTime[emtpyIndex] = clock();
        }
    }
    
}

void store(unsigned long *addr, int size) {
    int setNumber = getset(addr);
    long tagNumber = gettag(addr);
    struct oneSet *set = &cache[setNumber];
    
    int isHit = 0;
    for (int i = 0; i < E; i++) {
        if (set->validBit[i] == 1 && set->tagBit[i] == tagNumber) {
            isHit = 1;
            break;
        }
    }
    
    if (isHit) {
        hit += 1;
        return;
    } else {
        load(addr, size);
    }
}

void modify(unsigned long *addr, int size) {
    load(addr, size);
	store(addr, size);
}













