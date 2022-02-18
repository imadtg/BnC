#include <stdio.h>
#include <string.h>
#include <conio.h>
#define SIZE 4 // 1 .. 10
#define NO_PADDING_ZERO 1 // 1 | 0
#if NO_PADDING_ZERO
    #if SIZE == 1
        #define LIMIT 9
        #define TLIMIT 10
    #elif SIZE == 2
        #define LIMIT 81
    #elif SIZE == 3
        #define LIMIT 648
    #elif SIZE == 4
        #define LIMIT 4536
    #elif SIZE == 5
        #define LIMIT 27216
    #elif SIZE == 6
        #define LIMIT 136080
    #elif SIZE == 7
        #define LIMIT 544320
    #elif SIZE == 8
        #define LIMIT 1632960
    #else
        #define LIMIT 3265920
    #endif
#else
    #if SIZE == 1
        #define LIMIT 10
    #elif SIZE == 2
        #define LIMIT 90
    #elif SIZE == 3
        #define LIMIT 720
    #elif SIZE == 4
        #define LIMIT 5040
    #elif SIZE == 5
        #define LIMIT 30240
    #elif SIZE == 6
        #define LIMIT 151200
    #elif SIZE == 7
        #define LIMIT 604800
    #elif SIZE == 8
        #define LIMIT 1814400
    #else
        #define LIMIT 3628800
    #endif
#endif

void show(char number[]){ // not using strings so have to do this.
    for(int i = 0; i < SIZE; i++)
        printf("%c", number[i]);
}

void showStat(int stat[]){ // shows statistics.
    for(int i = 0; i < 10; i++){
        printf("%d : %d %4.2f\n", i, stat[i], (float) (100 * stat[i]) / LIMIT);
    }
}

void peek(char table[][SIZE], int l){ // peek through the multiverse of possibilities!
    printf("{");
    if(l) show(table[0]);
    for(int i = 1; i < l; i++){
        printf(", ");
        show(table[i]);
    }
    printf("}");
}

int inplace(char target[], char guess[]){ // how many digits of the guess are inplace
    int c = 0;
    for(int i = 0; i < SIZE; i++){
        if(target[i] == guess[i]){
            c++;
        }
    }
    return c;
}

int correct(char target[], char guess[]){ // how many digits of the guess are in the target
    int c = 0;
    for(int i = 0; i < SIZE; i++){
        for(int j = 0; j < SIZE; j++){
            if(target[i] == guess[j]){
                c++;
                break;
            }
        }
    }
    return c;
}

/*void create(char table[][SIZE]){ // fill table with all SIZE digit numbers with no repeating digits, numbers may have a padding zero.
    int i = 0;
    char a, b, c, d;
    for(a = '0' + NO_PADDING_ZERO; a <= '9'; a++)
        for(b = '0'; b <= '9'; b++) if(b != a)
            for(c = '0'; c <= '9'; c++) if(c != b && c != a)
                for(d = '0'; d <= '9'; d++) if(d != c && d != b && d != a){
                    table[i][0] = a; table[i][1] = b;
                    table[i][2] = c; table[i][3] = d;
                    i++;
                }
}*/

int available(char a, char target[], int l){ // checks if a character does not exist in the target.
    for(int i = 0; i < l; i++){
        if(a == target[i]){
            return 0;
        }
    }
    return 1;
}

void create(char table[][SIZE] , int *c, char dummy[], int l, int NPZ){ // the big bang of a new universe of possibilities!
    if(l == SIZE){
        memcpy(table[*c], dummy, SIZE);
        (*c)++;
        return;
    }
    for(char i = '0' + NPZ; i <= '9'; i++){
        if(available(i, dummy, l)){
            dummy[l] = i;
            create(table, c, dummy, l + 1, 0);
        }
    }
}

void filter(char table[][SIZE], int *l, char gues[], int corr, int inp){ // removes all impossible cases of a table by the given feedback.
    int index = 0;
    for(int i = 0; i < *l; i++){
        if(correct(table[i], gues) == corr && inplace(table[i], gues) == inp){
            memcpy(table[index], table[i], SIZE);
            index++;
        }
    }
    *l = index;
}

int filteredCount(char table[][SIZE], int l, char gues[], int corr, int inp){ // gives how many possibilites are filtered by a guess and a hint.
    int c = 0;
    for(int i = 0; i < l; i++){
        if(correct(table[i], gues) != corr || inplace(table[i], gues) != inp){
            c++;
        }
    }
    return c;
}

int information(char table[][SIZE], int l, char gues[]){ // returns how informative a guess is.
    int hints[SIZE + 1][SIZE + 1] = {0}, inf = 0;
    for(int i = 0; i < l; i++){
        hints[correct(table[i], gues)][inplace(table[i], gues)]++;
    }
    for(int i = 0; i <= SIZE; i++){
        for(int j = 0; j <= i; j++){
            if(hints[i][j])
                inf += hints[i][j] * filteredCount(table, l, gues, i, j);
        }
    }
    return inf;
}

void guess(char gues[], char universe[][SIZE], int l, char multiverse[][SIZE]){ // decide the next guess.
    if(l == 1 || l == 2 || l == LIMIT){
        memcpy(gues, universe[0], SIZE);
        return;
    }
    int imax = 0;
    int infmax = information(universe, l, multiverse[0]); int inf;
    for(int i = 1; i < LIMIT; i++){
        inf = information(universe, l, multiverse[i]);
        if(inf > infmax){
            imax = i;
            infmax = inf;
        }
    }
    memcpy(gues, multiverse[imax], SIZE);
}

void play(char target[]){ // attempts to guess the target.
    char multiverse[LIMIT][SIZE], universe[LIMIT][SIZE], gues[SIZE], dummy[SIZE];
    int i, corr, inp, l = 0;
    create(universe, &l, dummy, 0, NO_PADDING_ZERO);
    memcpy(multiverse, universe, LIMIT * SIZE);
    do{
        guess(gues, universe, l, multiverse);
        corr = correct(target, gues);
        inp = inplace(target, gues);
        show(gues);
        printf("\n");
        peek(universe, l);
        printf("\n");
        printf("%d %d\n\n", corr, inp);
        filter(universe, &l, gues, corr, inp);
    }while(inp != SIZE);
}

void playInteractive(int seed){ // attempts to guess the number based on hints given by the user.
    char multiverse[LIMIT][SIZE], universe[LIMIT][SIZE], dummy[SIZE], gues[SIZE];
    int i, corr, inp, l = 0;
    create(universe, &l, dummy, 0, NO_PADDING_ZERO);
    memcpy(multiverse, universe, LIMIT * SIZE);
    memcpy(gues, universe[seed], SIZE);
    do{
        show(gues);
        printf("\n");
        scanf("%d %d", &corr, &inp);
        printf("\n\n");
        filter(universe, &l, gues, corr, inp);
        if(l == 0) {
            printf("impossible\n"); return;
        }
        guess(gues, universe, l, multiverse);
    }while(inp != SIZE);
}

int playStat(char target[], int v, char multiverse[][SIZE]){ // a version of play optimized for statistics and returns number of steps, v is for verbose mode.
    char universe[LIMIT][SIZE], gues[SIZE];
    int i, corr, inp, l = LIMIT, s = 0;
    memcpy(universe, multiverse, LIMIT * SIZE);
    do{
        guess(gues, universe, l, multiverse);
        s++;
        corr = correct(target, gues);
        inp = inplace(target, gues);
        if(v){
            show(gues);
            printf("\n");
            peek(universe, l);
            printf("\n");
            printf("%d %d\n\n", corr, inp);
        }
        filter(universe, &l, gues, corr, inp);
    }while(inp != SIZE);
    return s;
}

void statistics(int stat[], int k){ // gives statistics of the current guessing strategy.
    char universe[LIMIT][SIZE], dummy[SIZE];
    int l = 0;
    create(universe, &l, dummy, 0, NO_PADDING_ZERO);
    for(int i = 0; i < LIMIT; i++){
        stat[playStat(universe[i], 0, universe)]++;
        if(i % k == 0) printf(".%d.", i);
    }
}

int main(){/*
    int stat[12] = {0};
    statistics(stat, 10);
    printf("\n");
    showStat(stat);
    getch();
    */
    int i = 148;
    while(1){
        printf("\n------------------\n");
        playInteractive(i);
        i = (i * i) % LIMIT;
    }
    return 0;
}
