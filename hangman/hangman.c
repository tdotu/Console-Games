#include <stdio.h>
#include <string.h>

#include "hangman.h"



int main(int argc, char **argv)
{
    char solution[] = "university";

    askWord(solution, 10);

    return 0;
}


truth askWord(char *word, int maxGuesses)
{
    int wordLength = strlen(word);
    int tryCounter = 0;

    char currentWord[wordLength];
    char triedLetters[LETTERS_IN_ALPHABET];

    guessedCorrectly guessCorrect;

    memset(currentWord, '_', sizeof(currentWord));
    currentWord[wordLength] = '\0';

    memset(triedLetters, '\0', sizeof(triedLetters));

    while(tryCounter < maxGuesses)
    {
        printf("Word mask: %s\n", currentWord);
        printf("Remaining tries: %d\n", maxGuesses - tryCounter);

        guessCorrect = askCharacter(word, currentWord, triedLetters);

        if(guessCorrect == NO)
            tryCounter++;
        else if(guessCorrect == YES && checkSolved(word, currentWord) == TRUE)
            return TRUE;

        printf("\n");
    }

    return FALSE;
}


guessedCorrectly askCharacter(char *finalWord, char *currentWord, char *triedCharacters)
{
    char guess = '\0';

    printf("Next guess: ");
    fflush(stdin);
    guess = getchar();
    getchar();              // clears the input buffer (may not work on all computers)

    if(checkCharacter(guess, triedCharacters))
    {
        printf("You have already tried this character.\n");
        return AGAIN;
    }

    if(checkCharacter(guess, finalWord))
    {
        printf("Excellent!\n");
        uncoverCharacters(guess, finalWord, currentWord);
        addCharacter(guess, triedCharacters);
        return YES;
    }
    else
    {
        printf("Unfortunately incorrect!\n");
        addCharacter(guess, triedCharacters);
        return NO;
    }
}


truth checkSolved(char *finalWord, char *currentWord)
{
    if(!strncmp(finalWord, currentWord, MAX_WORDLENGTH))
        return TRUE;

    return FALSE;
}


truth checkCharacter(char character, char *word)
{
    int counter = 0;
    int wordlength = strlen(word);

    for(counter = 0; counter < wordlength; counter++)
        if(character == word[counter])
            return TRUE;

    return FALSE;
}


void uncoverCharacters(char character, char *finalWord, char *currentWord)
{
    int counter = 0;
    int wordlength = strlen(finalWord);

    for(counter = 0; counter < wordlength; counter++)
        if(character == finalWord[counter])
            currentWord[counter] = character;
}


void addCharacter(char character, char *charlist)
{
    int listsize = strlen(charlist);
    charlist[listsize] = character;
    charlist[++listsize] = '\0';
}
