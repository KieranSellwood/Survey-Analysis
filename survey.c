/* survey.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Compile-time constants
 */
#define MAX_WORDS 20
#define MAX_WORD_LEN 20
#define MAX_LINE_LEN 100

/*
 * Function prototypes
 */
int tokenize_line(char *line, char words[][MAX_WORD_LEN]);
void print_line(char words[][MAX_WORD_LEN], int num_words);

/* loops over stdin lines */
int main(int argc, char *argv[])
{
    if (argc != 1) {
            printf("Usage: %s\n", argv[0]);
            printf("Should receive no parameters\n");
            printf("Read from the stdin instead\n");
            exit(1);
    }

    int num_words = 0;
    char words[MAX_WORDS][MAX_WORD_LEN];
    
    char line[MAX_LINE_LEN];
    
    while ( fgets(line, sizeof(char) * MAX_LINE_LEN, stdin) ) {
        num_words = tokenize_line(line, words);
        print_line(words, num_words);
    }    

    exit(0);
}

/* tokenizes each line by splitting words 
    and stores them in a string array */
int tokenize_line(char *line, char words[][MAX_WORD_LEN]) {

    int num_words = 0;
    char *token;
    
    /* get the first token from line */
    token = strtok(line, " \n");
    
    /* for every token in line, store it */
    while (token) {

        strncpy(words[num_words], token, MAX_WORD_LEN);
        num_words++;
        
        /* get the next token from line or reach end of line */
        token = strtok(NULL, " \n");
    } 
    return num_words;           
}

/* prints the stored words one by one */ 
void print_line(char words[][MAX_WORD_LEN], int num_words) {
    for (int i = 0; i < num_words; i++) {
        printf("%s\n", words[i]);
    }
}

