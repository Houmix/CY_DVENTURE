#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdbool.h>



#define MAX_CHOICE_OPTIONS 10
#define MAX_CHOICE_BONUS 3
#define MAX_STOCK 3
#define GoldgoldDraw 10


typedef struct {
    int number;
    char description[1000];
    char nextStep;
} Choice;

typedef struct {
    char item1[200];
    char item2[200];
    char item3[200];
} Items;

typedef struct {
    char description[1000];
    char event[1000];
    Choice choices[MAX_CHOICE_OPTIONS];
    int numChoices;
    char monster[1000];
} Step;

typedef struct {
    char name[30];
    int life;
    int power;
    char step[20];
    Items stock;
    int gold;
    int shield;
} Hero;

typedef struct {
    char name[30];
    int life;
    int power;
    char weapon[20];
    int shield;
} Boss;

typedef struct {
    char name[30];
    int life;
    int power;
    int shield;
} Monster;
typedef struct {
    char description[200];
    char event[200];
    Choice choices[MAX_CHOICE_BONUS];
    int numChoices;
} Bonus;
