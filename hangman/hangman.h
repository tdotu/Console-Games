#ifndef HANGMAN_H_INCLUDED
#define HANGMAN_H_INCLUDED



#define MAX_WORD_LENGTH 30
#define MAX_FILENAME_LENGTH 50
#define LETTERS_IN_ALPHABET 62

#define STANDART_NUMBER_OF_GUESSES 11


enum truth
{
    TRUE = 1, FALSE = 0
};

enum guessedCorrectly
{
    YES, NO, AGAIN
};

enum saveMode
{
    UNKNOWN = 0, APPEND = 1, OVERWRITE = 2
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
typedef enum saveMode saveMode;
typedef struct wordListEntry wordListEntry;
typedef struct wordList wordList;


int main(int argc, char **argv);

void mainMenue();
int printMenue();
void mainMenueOption1();
void mainMenueOption2();
void mainMenueOption3();
void mainMenueOption4();
void mainMenueOption5();
void mainMenueOption6();
void mainMenueOption7();
void mainMenueOption8();
void mainMenueOptionDefault();

void printCredits();
char *enterFilename(char *buffer);
saveMode enterSaveMode();
char *enterWord(char *buffer);
int waitKey();

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
wordList *addWordFile(char *filename, wordList *wl);
wordList *loadWordFile(char *filename);
wordList *loadWordFileExtended(char *filename, wordList *wordlist);
void saveWordList(char *filename, saveMode mode, wordList *wl);

void initRandomGenerator();
char *getRandomWord(wordList *wl);

int printWordList(wordList *wl);        // debug



#endif // HANGMAN_H_INCLUDED
