#ifndef HANGMAN_H_INCLUDED
#define HANGMAN_H_INCLUDED



#define MAX_WORDLENGTH 30
#define LETTERS_IN_ALPHABET 32


enum truth
{
    TRUE = 1, FALSE = 0
};

enum guessedCorrectly
{
    YES, NO, AGAIN
};


typedef enum truth truth;
typedef enum guessedCorrectly guessedCorrectly;


truth askWord(char *word, int maxGuesses);
guessedCorrectly askCharacter(char *finalWord, char *currentWord, char *triedCharacters);
truth checkSolved(char *finalWord, char *currentWord);
truth checkCharacter(char character, char *word);
void uncoverCharacters(char character, char *finalWord, char *currentWord);
void addCharacter(char character, char *charlist);



#endif // HANGMAN_H_INCLUDED
