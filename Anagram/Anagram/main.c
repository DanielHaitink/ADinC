#define ARRAYLENGTH 28

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

/*
/ typedef "sentence", which basically is a histogram for the letters and number of letters
*/
typedef int sentence[ARRAYLENGTH];

/*
/ fuction readSentences reads the input sentences. A histogram will be created of the sentences, which contain the number of 
/ letters and how often a letter appears in a sentence.
*/
sentence *readSentences(int numberOfSentences){
    int numberOfChar, i;
    char c;
    /* Calloc an array filled with sentences */
    sentence *sentenceArray = calloc(numberOfSentences, sizeof(sentence));
    assert(sentenceArray);
    
    numberOfChar = 0;
    
    /* for the number of sentences, getchar the input until a "." is detected. */
    for(i = 0; i<numberOfSentences; i++) {
        while(1) {
            c = getchar();
            if(c == '.'){
                break;
            }
            else{
                /* 
                /  filter out spaces and enters. If it is not one of those characters, increase number of characters and
                /  increase the histogram with one on the correct place (c-'a')
                */
                if(c != ' ' && c!= '\n') {
                    numberOfChar++;
                    sentenceArray[i][c - 'a']++;
                }
            }
        }
        
        sentenceArray[i][ARRAYLENGTH-1]=numberOfChar;
        numberOfChar = 0;
    }
    
    return sentenceArray;
}

/*
/ the function compare compares the test sentences to the other sentences. If the histograms are the same, the function will print the number of the sentence.
*/
int compare(sentence *sentences, sentence *testsentences, int numberOfSentences, int numberOftestsentences){
    int i = 0, j = 0, n = 0;
    
    /* loop for test sentences */
    for(i = 0; i<numberOftestsentences; i++) {
        /* loop for sentences */
        for(j = 0; j<numberOfSentences; j++) {
            /* loop for the histograms. Loops from ARRAYLENGTH-1 to 0, since the length of the sentence is at 27 */
            for(n = ARRAYLENGTH-1; n>=0; n--) {
                /* compare the histograms, if an arraylocation is not the same, break */
                if(testsentences[i][n] != sentences[j][n]) {
                    break;
                }
                /* program has not found a difference between sentence histogram and the test sentence histogram AND n is 0 or smaller (so it has looped through the complete histogram), thus print sentence */
                else if(n <= 0){
                    printf("%d ", j + 1);
                }
            }
        }
        printf("\n");
    }
    return 1;
}

int main(int argc, const char * argv[]) {
    sentence *testsentences, *sentences;
    int numberOfSentences = 0, numberOfTestsentences = 0;
    
    /*
    / First scan the number, save. Then go into readSentences(), save the output in sentences. The same is done for testsentences. 
    / After that, compare sentences to testsentences.
    */
    scanf("%d\n", &numberOfSentences);
    sentences = readSentences(numberOfSentences);
    
    scanf("%d\n", &numberOfTestsentences);
    testsentences = readSentences(numberOfTestsentences);
    
    compare(sentences, testsentences, numberOfSentences, numberOfTestsentences);
    
    /*
    / free both sentences as testsentences
    */
    free(sentences);
    free(testsentences);
    return 1;
}
