#ifndef HANGMAN_H_INCLUDED
#define HANGMAN_H_INCLUDED



#define MAX_WORDLENGTH 30
#define LETTERS_IN_ALPHABET 62

#define STANDART_NUMBER_OF_GUESSES 10


enum truth
{
    TRUE = 1, FALSE = 0
};

enum guessedCorrectly
{
    YES, NO, AGAIN
};


struct wordListEntry
{
    char *word;
    struct wordListEntry *next;
};

struct wordList
{
    int numOfWords;
    struct wordListEntry *first;
};


typedef enum truth truth;
typedef enum guessedCorrectly guessedCorrectly;
typedef struct wordListEntry wordListEntry;
typedef struct wordList wordList;


void mainMenue();
int printMenue();
void mainMenueOption1();
void mainMenueOption2();
void mainMenueOption3();
void mainMenueOption4();
void mainMenueOption5();
void mainMenueOption6();
void mainMenueOption7();
void mainMenueOptionDefault();

truth askWord(char *word, int maxGuesses);
guessedCorrectly askCharacter(char *finalWord, char *currentWord, char *triedCharacters);
truth checkSolved(char *finalWord, char *currentWord);
truth checkCharacter(char character, char *word);
void uncoverCharacters(char character, char *finalWord, char *currentWord);
void addCharacter(char character, char *charlist);

char *appendWordList(wordList *wl, char *newEntry);
void deleteAllElements(wordList *wl);
void deleteLastElement(wordList *wl);
wordList *createWordList();
char *getElement(wordList *wl, int element);
char *readLine(FILE *stream);
wordList *loadWordFile(char *filename);
wordList *addWordFile(char *filename, wordList *wl);

void initRandomGenerator();
char *getRandomWord(wordList *wl);

int printWordList(wordList *wl);        // debug



#endif // HANGMAN_H_INCLUDED
