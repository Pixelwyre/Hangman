# Hangman

### Dependencies
- SDL2
- glad

### PAC
| ✅   | Input              | Processing                                               | Module / Reference    | Output                         |
|-----|--------------------|----------------------------------------------------------|-----------------------|--------------------------------|
| [ ] | word, lives        | Initialize game state and variables                      | `initHangman()`       | Ready game state               |
| [ ] | guessed letter     | Check if letter exists in word, update revealed or lives | `processGuess()`      | Updated state (revealed/lives) |
| [ ] | current game state | Check if all letters guessed                             | `isGameWon()`         | Boolean (win/lose)             |
| [ ] | current game state | Check if lives are 0 or word complete                    | `isGameOver()`        | Boolean (true/false)           |
| [ ] | current game state | Return current revealed word with underscores            | `getRevealedWord()`   | Revealed word string           |
| [ ] | current game state | Return remaining lives count                             | `getRemainingLives()` | Integer (lives left)           |
| [ ] | current game state | Return list of already guessed letters                   | `getGuessedLetters()` | Character array                |
| [ ] | guessed letter     | Validate that guess is valid and not repeated            | `validateGuess()`     | Boolean (valid/invalid)        |
| [ ] | new word, lives    | Reset all values for new round                           | `resetGame()`         | Refreshed game state           |


### PSEUDOCODE