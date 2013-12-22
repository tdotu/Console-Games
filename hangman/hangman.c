#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hangman.h"



wordList *words = NULL;



int main(int argc, char **argv)
{
    initRandomGenerator();

    mainMenue();

    return 0;
}


void mainMenue()
{
    int option = 0;

    do
    {
        option = printMenue();

        switch(option)
        {
            case 1:
                mainMenueOption1();
            break;

            case 2:
                mainMenueOption2();
            break;

            case 3:
                mainMenueOption3();
            break;

            case 4:
                mainMenueOption4();
            break;

            case 5:
                mainMenueOption5();
            break;

            case 6:
                mainMenueOption6();
            break;

            case 7:
                mainMenueOption7();
            break;

            case 8:
                mainMenueOption8();
            break;

            default:
                mainMenueOptionDefault();
            break;
        }
    }while(option != 8);
}


int printMenue()
{
    int choice = 0;

    printf("Main menu\n");
    printf("---------\n");
    printf("\n");
    printf("1. Play word guessing (random word)\n");
    printf("2. Play word guessing (self entered word)\n");
    printf("3. Load a word library\n");
    printf("4. Enter a word by hand\n");
    printf("5. Save the loaded words to a new word library\n");
    printf("6. Unload all words\n");
    printf("7. Show the credits\n");
    printf("8. Exit this program\n");
    printf("\n");
    printf("Your selection: < >\b\b");
    scanf("%d", &choice);
    printf("\n");

    return choice;
}


void mainMenueOption1()
{
    if(words == NULL)
    {
        printf("Something went wrong (the word list is not existing)\n");
        return;
    }

    if(words->numOfWords == 0)
    {
        printf("You have to put words in the word list before you can play!\n");
        return;
    }

    if(askWord(getRandomWord(words), STANDART_NUMBER_OF_GUESSES) == TRUE)
    {
        printf("Gratulations!\n");
        printf("You found the word!\n");
    }
    else
    {
        printf("Don't worry.\n");
        printf("Maybe you'll win next time.\n");
    }

    waitKey();
}


void mainMenueOption2()
{
    char word[MAX_WORD_LENGTH];
    enterWord(word);

    printf("\n");

    if(askWord(word, STANDART_NUMBER_OF_GUESSES) == TRUE)
    {
        printf("Gratulations!\n");
        printf("You found the word!\n");
    }
    else
    {
        printf("Don't worry.\n");
        printf("Maybe you'll win next time.\n");
    }

    waitKey();
}


void mainMenueOption3()
{
    char libraryName[MAX_FILENAME_LENGTH];
    enterFilename(libraryName);

    printf("Loading library %s...\n", libraryName);
    loadWordFileExtended(libraryName, words);
    printf("Finished.\n");

    waitKey();
}


void mainMenueOption4()
{
    char *word = (char *)malloc(sizeof(char) * MAX_WORD_LENGTH);
    enterWord(word);

    appendWordList(words, word);

    printf("List appended.\n");

    waitKey();
}


void mainMenueOption5()
{
    char filename[MAX_FILENAME_LENGTH];
    saveMode mode;

    enterFilename(filename);
    mode = enterSaveMode();

    printf("Saving Library...\n");
    saveWordList(filename, mode, words);
    printf("Finished.\n");

    waitKey();
}


void mainMenueOption6()
{
    printf("Unloading all words...\n");
    deleteAllElements(words);
    printf("All words unloaded.\n");

    waitKey();
}


void mainMenueOption7()
{
    printCredits();

    waitKey();
}


void mainMenueOption8()
{
    printf("Program will be ended...\n");
}


void mainMenueOptionDefault()
{
    printf("Your choosen selection does not exist.\n");

    waitKey();
}


void printCredits()
{
    printf("Credits\n");
    printf("-------\n");
    printf("\n");
    printf("Idea:\t\t\tPeter Hoesch\n");
    printf("Programming:\t\tPeter Hoesch\n");
    printf("Tests:\t\t\tPeter Hoesch\n");
}


char *enterFilename(char *buffer)
{
    printf("Enter the filename: ");
    scanf("%s", buffer);

    return buffer;
}


saveMode enterSaveMode()
{
    saveMode mode = UNKNOWN;

    do
    {
        printf("\n");
        printf("Mode:\n");
        printf("1. Append\n");
        printf("2. Overwrite\n");
        printf("Your selection: < >\b\b");
        scanf("%u", &mode);

        if(mode != APPEND && mode != OVERWRITE)
            printf("\nThis selection is not valid.\n");

    }while(mode != APPEND && mode != OVERWRITE);

    return mode;
}


char *enterWord(char *buffer)
{
    printf("Enter the word: ");
    scanf("%s", buffer);

    return buffer;
}


int waitKey()
{
    int key = 0;

    printf("\nPress enter to continue...\n");

    getchar();

    do
    {
        key = getchar();
    }while(key != '\n');

    return key;
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
        {
            printf("\n");
            printf("Solved word: %s\n", currentWord);
            printf("\n");
            return TRUE;
        }


        printf("\n");
    }

    printf("Solution: %s\n\n", word);

    return FALSE;
}


guessedCorrectly askCharacter(char *finalWord, char *currentWord, char *triedCharacters)
{
    char guess = '\0';

    printf("Next guess: ");

    do
    {
        guess = getchar();
    }while(guess == '\0' || guess == '\n');

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
    if(!strncmp(finalWord, currentWord, MAX_WORD_LENGTH))
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


char *appendWordList(wordList *wl, char *newEntry)
{
    wordListEntry *listIterator = wl->first;

    if(newEntry == NULL)
        return;

    while(listIterator->next != NULL)
        listIterator = listIterator->next;

    listIterator->next = (wordListEntry *)malloc(sizeof(wordListEntry));
    listIterator = listIterator->next;
    listIterator->next = NULL;
    listIterator->word = newEntry;
    wl->numOfWords++;

    return newEntry;
}


void deleteAllElements(wordList *wl)
{
    while(wl->numOfWords > 0)
        deleteLastElement(wl);
}


void deleteLastElement(wordList *wl)
{
    wordListEntry *listIterator = wl->first;

    while(listIterator->next->next != NULL)
        listIterator = listIterator->next;

    free(listIterator->next);
    listIterator->next = NULL;
    wl->numOfWords--;
}


wordList *createWordList()
{
    wordList *newList = (wordList *)malloc(sizeof(wordList));
    newList->first = (wordListEntry *)malloc(sizeof(wordListEntry));    // create a dummy
    newList->first->next = NULL;
    newList->first->word = NULL;
    newList->numOfWords = 0;

    return newList;
}


char *getElement(wordList *wl, int element)
{
    int i = 0;
    wordListEntry *listIterator = wl->first;

    for(i = 0; i < element; i++)
    {
        if(listIterator->next != NULL)
            listIterator = listIterator->next;
        else
        {
            printf("Unable to get element %d from list %p.\n", element, wl);
            return NULL;
        }
    }

    return listIterator->word;
}


char *readLine(FILE *stream)
{
    long lineBegin = ftell(stream);
    char *line = NULL;

    char currentChar = '\0';
    int charCounter = 0;

    if(feof(stream))
        return NULL;

    for(charCounter = 0; currentChar != '\n' && currentChar != '\0'; charCounter++)
        currentChar = getc(stream);

    line = (char *)malloc((sizeof(char) * (charCounter + 1)));

    fseek(stream, SEEK_SET, lineBegin);
    fgets(line, MAX_WORD_LENGTH, stream);
    line[charCounter-1] = '\0';

    for(charCounter = 0; line[charCounter] != '\0'; charCounter++)
        if(line[charCounter] == '\n')
            line[charCounter] = '\0';

    return line;
}


wordList *addWordFile(char *filename, wordList *wl)
{
    FILE *f = fopen(filename, "r");

    if(f == NULL)
    {
        printf("Unable to read file %s\n", filename);
        return NULL;
    }

    while(!feof(f))
        if(*appendWordList(wl, readLine(f)) == '\0')
            deleteLastElement(wl);

    deleteLastElement(wl);

    fclose(f);

    return wl;
}



wordList *loadWordFile(char *filename)
{
    wordList *newList = createWordList();
    return addWordFile(filename, newList);
}


wordList *loadWordFileExtended(char *filename, wordList *wordlist)
{
    if(wordlist == NULL)
        wordlist = loadWordFile(filename);
    else
        addWordFile(filename, wordlist);

    return wordlist;
}


void saveWordList(char *filename, saveMode mode, wordList *wl)
{
    char mode_str[2];
    FILE *f = NULL;
    wordListEntry *iterator = NULL;

    memset(mode_str, '\0', sizeof(mode_str));

    switch(mode)
    {
        case APPEND:
            mode_str[0] = 'a';
        break;

        case OVERWRITE:
            mode_str[0] = 'w';
        break;

        default:
            printf("Something went wrong (unable to save file).\n");
        break;
    }

    f = fopen(filename, mode_str);

    for(iterator = wl->first->next; iterator != NULL; iterator = iterator->next)
    {
        fprintf(f, "%s\n", iterator->word);
    }
}


void initRandomGenerator()
{
    srand(time(0));
}


char *getRandomWord(wordList *wl)
{
    int number = rand() % wl->numOfWords+1;

    return getElement(wl, number);
}


int printWordList(wordList *wl)
{
    wordListEntry *listIterator = wl->first->next;

    while(listIterator != NULL)
    {
        printf("%s\n", listIterator->word);
        listIterator = listIterator->next;
    }

    return wl->numOfWords;
}
