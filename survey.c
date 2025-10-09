/* survey.c */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Compile-time constants
 */
#define MAX_WORD_LEN 100
#define MAX_LINE_LEN 1024
#define MAX_QUESTIONS 20
#define MAX_OPTIONS 10
#define MAX_RESPONDENTS 100

/*
 * Function prototypes
 */
int tokenize_line(char *line, char words[][MAX_WORD_LEN], const char *delim);
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

    int phase = 1;
    int show_freq = 0;
    int show_avg = 0;
    int num_questions = 0;
    int num_options = 0;
    int num_respondents = 0;
    int num_words = 0;
    
    char questions[MAX_QUESTIONS][MAX_WORD_LEN];
    char options[MAX_OPTIONS][MAX_WORD_LEN];
    char responses[MAX_RESPONDENTS][MAX_QUESTIONS][MAX_WORD_LEN];
    
    char line[MAX_LINE_LEN];

    
    while ( fgets(line, sizeof(char) * MAX_LINE_LEN, stdin) ) {
        if (line[0] == '#') continue;
        switch (phase){
            case 1:
                // case 1 is for the configuration bits
                sscanf(line, "%d,%d" , &show_freq, &show_avg);
                printf("Config: freq = %d, avg = %d\n", show_freq, show_avg);
                phase++;
                break;

            case 2:
                // case 2 is for the questions (split by ;)
                num_questions = tokenize_line(line,questions,";");
                printf("Read %d questions. \n", num_questions);
                phase++;
                break;

            case 3:
                // case 3 is for the answer options
                num_options = tokenize_line(line,options,",");
                printf("Read %d options. \n", num_options);
                phase++;
                break;
            
            case 4:
                // case 4 is for the respondents answer
                tokenize_line(line, responses[num_respondents],",");
                num_respondents++;
                break;
        }
    }    
    printf("\nTotal respondents: %d\n", num_respondents);
    exit(0);
}

/* tokenizes each line by splitting words 
    and stores them in a string array */
int tokenize_line(char *line, char words[][MAX_WORD_LEN], const char *delim) {

    int num_words = 0;
    char *token;
    
    /* get the first token from line */
    token = strtok(line, delim);
    
    /* for every token in line, store it */
    while (token && num_words < MAX_QUESTIONS) {

        strncpy(words[num_words], token, MAX_WORD_LEN-1);
        words[num_words][MAX_WORD_LEN-1] = '\0';
        num_words++;
        
        /* get the next token from line or reach end of line */
        token = strtok(NULL, delim);
    } 
    return num_words;           
}
