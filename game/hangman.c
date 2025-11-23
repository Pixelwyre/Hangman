#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "hangman.h"
#include "../utility/utilities.h"

/**
 * Intializes the GameState struct using the default and provided values
 *
 * @param wordFile pointer to the string containing name of the file
 * @param word pointer to the string containing the word from the file
 * @param lives amount of the initial lives of user when the game start
 * @return GameState initialised by the provided and default values
 */
GameState initHangman(const char *wordFile, const char *word, int lives) {
    GameState game;

    strncpy(game.wordFile, wordFile, MAX_WORD_LEN);

    strncpy(game.word, word, MAX_WORD_LEN);
    stringToLower(game.word);

    copyStringToUnderscores(game.revealed, game.word);

    for (int i = 0; game.word[i] != '\0'; i++) {
        if (game.word[i] == ' ')
            game.revealed[i] = ' ';
    }

    // makes random blank a super blank
    int len = strlen(game.word);
    int underscoreIndexes[MAX_WORD_LEN];
    int count = 0;
    for (int i = 0; i < len; i++) {
        if (game.revealed[i] == '_')
            underscoreIndexes[count++] = i;
    }

    if (count > 0)
        game.superBlankPos = underscoreIndexes[rand() % count];
    else
        game.superBlankPos = -1;

    if (game.superBlankPos != -1)
        game.revealed[game.superBlankPos] = '~';

    resetString(game.guessed);
    game.numGuessed = 0;
    game.lives = lives;
    game.shieldActive = 0;

    return game;
}

/**
 * Validates Guesses if they are eligible to be processed or not
 *
 * @param game GameState struct that contains the current state of the game
 * @param guess character the user has guessed
 * @return false if the character is present or is not alphabet, else true
 */
bool validateGuess(const GameState *game, char guess) {
    if (!isalpha(guess))
        return false;

    guess = tolower(guess);

    if (charInArray(game->guessed, guess, game->numGuessed))
        return false;

    return true;
}

/**
 * Checks if guess is correct or not, or if correct is it a super blank or not
 *
 * @param game GameState struct that contains the current state of the game
 * @param guess character the user has guessed
 * @return checks if guess is correct or wrong, adjusts lives accordingly, if super blank is guessed returns true, else false
 */
bool processGuess(GameState *game, char guess) {
    guess = tolower(guess);

    bool found = stringHasChar(game->word, guess);

    if (found) {
        revealGuessedLetter(game->word, game->revealed, guess);

        // checks if guess is super blank
        if (game->superBlankPos != -1 && game->word[game->superBlankPos] == guess) {
            game->revealed[game->superBlankPos] = guess;
            game->superBlankPos = -1; // super blank is used up
            return true;
        }

        return false;
    }
    if (game->shieldActive) {
        game->shieldActive = 0;
    } else {
        game->lives--;
    }
    return false;
}

/**
 * Checks if the user won the game
 *
 * @param game GameState struct that contains the current state of the game
 * @return if word is completely revealed (hence won)
 */
bool isGameWon(const GameState *game) {
    return isWordFullyRevealed(game->revealed);
}

/**
 * Checks if the game is over, by determining if user won or if lives are depleted
 *
 * @param game GameState struct that contains the current state of the game
 * @return 1 if game is won, (hence over, and won) or 0 if game is not won but lives is 0 (hence over, and lost)
 */
bool isGameOver(const GameState *game) {
    return (isGameWon(game) || game->lives <= 0);
}

/**
 * Returns the current word with '_' as blanks from the GameState struct
 *
 * @param game GameState struct that contains the current state of the game
 * @return revealed field from the GameState stuct
 */
const char *getRevealedWord(const GameState *game) {
    return game->revealed;
}

/**
 * Returns the current player lives from the GameState struct
 *
 * @param game GameState struct that contains the current state of the game
 * @return lives field from the GameState struct
 */
int getRemainingLives(const GameState *game) {
    return game->lives;
}

/**
 * Returns the array containing letters that player guessed thus far
 *
 * @param game GameState struct that contains the current state of the game
 * @return the guessed array from GameState struct which contains the characters user has guessed
 */
const char *getGuessedLetters(const GameState *game) {
    return game->guessed;
}

/**
 * Resets the game using initHangman(). We call this at the end of the game loop if playing again
 *
 * @param wordFile pointer to the string containing name of the file
 * @param word pointer to the string containing the word from the file
 * @param lives amount of the initial lives of user when the game start
 * @return GameState initialised by the provided and default values using the initHangman() function
 */
GameState resetGame(const char *wordFile, const char *word, int lives) {
    return initHangman(wordFile, word, lives);
}

/**
 * This process the powers the player received from randomly choosing a box from power menu
 *
 * @param game GameState struct that contains the current state of the game
 * @param power_id id of the power up obtained
 */
void activatePowerUp(GameState *game, int power_id) {
    switch (power_id) {
        case 1: {
            // reveals random blank
            int len = strlen(game->word);
            int validIndexes[MAX_WORD_LEN];
            int count = 0;

            for (int i = 0; i < len; i++) {
                if (game->revealed[i] == '_')
                    validIndexes[count++] = i;
            }

            if (count > 0) {
                int idx = validIndexes[rand() % count];
                game->revealed[idx] = game->word[idx];
                printf("Power-Up: A random letter was revealed!\n");
            }
        }
        break;

        case 2: // gives extra life
            game->lives++;
            printf("Power-Up: +1 Life!\n");
            break;

        case 3: // shows all vowels
            for (int i = 0; game->word[i] != '\0'; i++) {
                char c = game->word[i];
                if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
                    game->revealed[i] = c;
            }
            printf("Power-Up: All vowels revealed!\n");
            break;

        case 4: // activates shield
            game->shieldActive = 1;
            printf("Power-Up: Shield activated!\n");
            break;

        case 5: {
            // funny chance power
            int roll = rand() % 100 + 1;

            if (roll <= 5) {
                printf("Bonus random power up!\n");
                activatePowerUp(game, rand() % 4 + 1);
            } else if (roll <= 10) {
                game->lives--;
                printf("Lost 1 life.\n");
            } else {
                printf("Nothing happened.\n");
            }
        }
        break;
    }
}

/**
 * Shows user the power up menu to pick a random box, calls activatePowerUp() to activate that power up
 * Fills 3 random boxes out of 9 with random power ups
 *
 * @param game GameState struct that contains the current state of the game
 */
void powerUpMenu(GameState *game) {
    printf("\nSuper blank found! Choose a box (1–9)\n");

    int boxPower[9];
    for (int i = 0; i < 9; i++) boxPower[i] = 0;

    // choose 3 positions
    int pos1 = rand() % 9, pos2 = rand() % 9, pos3 = rand() % 9;
    while (pos2 == pos1) pos2 = rand() % 9;
    while (pos3 == pos1 || pos3 == pos2) pos3 = rand() % 9;

    // assigns random power ids
    boxPower[pos1] = rand() % 5 + 1;
    boxPower[pos2] = rand() % 5 + 1;
    boxPower[pos3] = rand() % 5 + 1;

    // displays boxes
    for (int i = 1; i <= 9; i++) {
        printf("[%d] ", i);
        if (i % 3 == 0) {
            printf("\n");
        }
    }

    printf("Choose a box: ");
    int choice;
    scanf("%d", &choice);

    if (choice < 1 || choice > 9) {
        printf("Invalid box.\n");
        return;
    }

    if (boxPower[choice - 1] != 0) {
        int pid = boxPower[choice - 1];
        printf("You got power up #%d!\n", pid);
        activatePowerUp(game, pid);
    } else {
        printf("Empty box, no power up.\n");
    }
}
