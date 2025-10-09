/* survey.c */
/*
Author: Kieran Sellwood
Student Number: V01069864
Date: October 8, 2025
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*
 * Compile-time constants
 */
#define MAX_WORD_LEN 200
#define MAX_LINE_LEN 1024
#define MAX_QUESTIONS 20
#define MAX_OPTIONS 10
#define MAX_RESPONDENTS 100

/*
 * Function prototypes
 */
int tokenize_line(char *line, char words[][MAX_WORD_LEN], const char *delim);
void print_line(char words[][MAX_WORD_LEN], int num_words);
int get_option_index(char *answer, char options[][MAX_WORD_LEN], int num_options);

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
    int freq[MAX_QUESTIONS][MAX_OPTIONS] = {0};
    char line[MAX_LINE_LEN];

    
    while ( fgets(line, sizeof(char) * MAX_LINE_LEN, stdin) ) {
        if (line[0] == '#') continue;
        switch (phase){
            case 1:
                // case 1 is for the configuration bits
                sscanf(line, "%d,%d" , &show_freq, &show_avg);
                //printf("Config: freq = %d, avg = %d\n", show_freq, show_avg);
                phase++;
                break;

            case 2:
                // case 2 is for the questions (split by ;)
                num_questions = tokenize_line(line,questions,";");
                //printf("Read %d questions. \n", num_questions);
                phase++;
                break;

            case 3:
                // case 3 is for the answer options
                num_options = tokenize_line(line,options,",");
                //printf("Read %d options. \n", num_options);
                phase++;
                break;
            
            case 4:
                // case 4 is for the respondents answer
                tokenize_line(line, responses[num_respondents],",");
                num_respondents++;
                break;
        }
    }
    // function that counts how many respondents chose each option for each question
    for (int r = 0; r < num_respondents; r ++){
        for (int q = 0; q < num_questions; q++){
            int index = get_option_index(responses[r][q], options, num_options);
            if (index >= 0){
                freq[q][index]++;
            }
        }
    }
    //printf("\nTotal respondents: %d\n", num_respondents);
    //printf("\nRelative frequencies\n");
    if (show_avg || show_freq){
        printf("ECS Student Survey\n");
        printf("SURVEY RESPONSE STATISTICS\n\n");
        printf("NUMBER OF RESPONDENTS: %d\n\n", num_respondents);
    }


    // function to print the frequency
    if (show_freq){
        printf("#####\n");
        printf("FOR EACH QUESTION/ASSERTION BELOW, RELATIVE PERCENTUAL FREQUENCIES ARE COMPUTED FOR EACH LEVEL OF AGREEMENT\n\n");

        for (int q = 0; q < num_questions; q++){
            printf("%d. %s\n", q + 1, questions[q]);
            for (int o = 0; o < num_options; o++){
                double percentage = 100.0 * freq[q][o] / num_respondents;
                printf("%.2f: %s\n", percentage, options[o]);
            }
        if (q != num_questions-1 ){
             printf("\n");
        }
        }
    }

    // function to print the averages
    // checks if file says to show averages
    if (show_avg){
        printf("\n#####\n");
        printf("FOR EACH QUESTION/ASSERTION BELOW, THE AVERAGE RESPONSE IS SHOWN (FROM 1-DISAGREEMENT TO 4-AGREEMENT)\n\n");
        for (int q = 0; q < num_questions; q++){
            double sum = 0.0;
            for (int o = 0; o < num_options; o++){
                sum += (o + 1) * freq[q][o];
            }
            double avg = sum / num_respondents;

            printf("%d. %s - %.2f\n" , q + 1, questions[q], avg);
        }
    }
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

        token[strcspn(token, "\r\n")] = '\0';
        strncpy(words[num_words], token, MAX_WORD_LEN-1);
        words[num_words][MAX_WORD_LEN-1] = '\0';
        num_words++;
        
        /* get the next token from line or reach end of line */
        token = strtok(NULL, delim);
    } 
    return num_words;           
}

// function to find the index for which option number corresponds to the answer string
int get_option_index(char *answer, char options[][MAX_WORD_LEN], int num_options){
    for (int i = 0; i < num_options; i++){
        if (strcmp(answer, options[i]) == 0){
            return i;
        }
    }
    return -1; // didnt find it
}