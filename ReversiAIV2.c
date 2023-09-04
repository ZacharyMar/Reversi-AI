/**
 * @file ReversiAi.c
 * @author Zachary Mar
 * @brief This file is used for APS105 Lab 8. 2023W version
 * @date 2023-03-27
 *
 */

// DO NOT REMOVE THE FOLLOWING LINE
#if !defined(TESTER_P1) && !defined(TESTER_P2)
// DO NOT REMOVE THE ABOVE LINE
// DO NOT REMOVE THE FOLLOWING LINE
#endif
// DO NOT REMOVE THE ABOVE LINE

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Displays current board position
void printBoard(char board[][26], int n);

// Initializes the board to starting position
void initializeBoard(char board[][26], int n);

// Finds all possible moves in current board position
// Returns number of valid moves found
int findValidMoves(char validMoves[][3], char player, char board[][26], int n);

// Checks if row and col are valid positions
bool validRowCol(int row, int col, int n);

// Moves coordinate position
void updateRowCol(int* row, int* col, int dir, bool forward);

// Determines if same colour token is in line with current token connected by
// opposite coloured tokens
bool inLine(int row, int col, int direction, int n, char board[][26],
            char player, bool forward);

// Determines if an inputted move is valid
bool checkValidMove(char validMoves[][3], char row, char col, int length);

// Gets move from the user
bool getMove(char validUserMoves[][3], int numUserMoves, char board[][26],
             int n, char userColour);

// Updates the game state with new move
void updateGame(char board[][26], char row, char col, char player, int n);

// Calculates final score of the game
void calculateFinalScore(char board[][26], char userColour, char compColour,
                         int n);

// Function calculates current score of the specified player
int calculateScore(char board[][26], char colour, int n);

// Function returns whether game is over in current board position
bool gameOver(char board[][26], int n);

// Function determines the computer's move --> uses greedy algorithm
void computerMove(char validCompMoves[][3], int numCompMoves, char board[][26],
                  int n, char compColour);

// Generates random move to play - used for testing
void randomMoveGenerator(char validMoves[][3], int numMoves, char board[][26],
                         int n, char turn);

// Function determines better computer move using alpha beta pruning
int makeMove(char board[][26], int n, char turn, int* row, int* col);

// Sorts the possible move to prioritize checking better moves first
void prioritizeMoves(char possibleMoves[][3], int numMoves);

// Function determines the score for a given move -- called to determine best
// possible move Uses minimax function with alpha beta pruning
int minimaxValue(char board[][26], int depth, char originalTurn,
                 char currentTurn, int n, int alpha, int beta, clock_t start);

// Determines static evaluation of board position
int heuristic(char board[][26], char turn, int n);

// Determines turn number
int getTurnNumber(char board[][26], int n);

// Determines mobility score
int getMobilityScore(char board[][26], char turn, int n);

// Determines number of open tiles next to the current tile
int getOpenTiles(char board[][26], int n, int row, int col);

// Determines corner score
int getCornerScore(char board[][26], char turn, int n);

// Determines edge score
int getEdgeScore(char board[][26], char turn, int n);

// Determines if disk is stable
bool isStable(char board[][26], char turn, int n, int row, int col);

// Determines parity score
int getParityScore(char board[][26], char turn, int n);

// Copies the content of array 1 to array 2
void copyArray(char arr1[][26], char arr2[][26], int n);

//*******************************************************
// Note: Please only put your main function below
// DO NOT REMOVE THE FOLLOWING LINE
#ifndef TESTER_P2
// DO NOT REMOVE THE ABOVE LINE
int main(void) {
  // Stores the dimension of the board
  int n;
  // Game board
  char board[26][26] = {'\0'};
  // Stores which colour the computer plays
  char compColour;
  // Stores which colour user plays
  char userColour;
  // Keep track of who's turn it is -> even = computer, odd = user
  int turnNumber = 0;
  // Stores if invalid move was played
  bool invalid = false;
  // Stores the possibe moves for white
  char validUserMoves[672][3] = {'\0'};
  // Number of moves user can make
  int numUserMoves;
  // Stores the possible moves for black
  char validComputerMoves[672][3] = {'\0'};
  // Number of moves Computer can make
  int numComputerMoves;

  // Keep track of times went over 1 sec calculation
  int overTimeCount = 0;

  // User enters board dimensions
  printf("Enter the board dimension: ");
  scanf("%d%*c", &n);

  // Assign colours
  printf("Computer plays (B/W): ");
  scanf("%c%*c", &compColour);

  if (compColour == 'B') {
    userColour = 'W';
  } else if (compColour == 'W') {
    userColour = 'B';
    turnNumber = 1;
  }

  // Initialize and print board
  initializeBoard(board, n);
  printBoard(board, n);

  // Find valid moves for user and computer
  numUserMoves = findValidMoves(validUserMoves, userColour, board, n);
  numComputerMoves = findValidMoves(validComputerMoves, compColour, board, n);

  // Main game loop
  do {
    // Player's move
    if (turnNumber % 2 == 1) {
      // Check if player has any valid moves
      if (numUserMoves != 0) {
        // Get player's move and prints new board position
        invalid = getMove(validUserMoves, numUserMoves, board, n, userColour);

        // Use computer for testing
        // computerMove(validUserMoves, numUserMoves, board, n, userColour);

        // Random Move generator
        // printf("\n");
        // randomMoveGenerator(validUserMoves, numUserMoves, board, n,
        // userColour);
      }
      // No valid moves for player
      else {
        printf("%c has no valid move.\n", userColour);
      }
    }
    // Computer's move
    else {
      if (numComputerMoves != 0) {
        int row;
        int col;
        // Use clock to determine runtime
        clock_t tic = clock();

        makeMove(board, n, compColour, &row, &col);

        clock_t toc = clock();
        printf("\nElapsed: %f seconds\n\n",
               (double)(toc - tic) / CLOCKS_PER_SEC);

        if ((double)(toc - tic) / CLOCKS_PER_SEC > 1) {
          overTimeCount++;
        }

        printf("%c%c\n", row + 'a', col + 'a');
        updateGame(board, row + 'a', col + 'a', compColour, n);
        printBoard(board, n);
      }
      // No valid moves for computer
      else {
        printf("%c has no valid move.\n", compColour);
      }
    }

    // Increment turn
    turnNumber++;
    // Update valid moves for user and computer for next turn
    numUserMoves = findValidMoves(validUserMoves, userColour, board, n);
    numComputerMoves = findValidMoves(validComputerMoves, compColour, board, n);

    /* Tell player there are no valid moves left
    if (numUserMoves == 0 && numComputerMoves == 0) {
      printf("There are no valid moves left.\n");
    }
    */

  } while (
      !gameOver(board, n) &&
      !invalid);  // Game ends if no possible moves or player makes invalid move

  // Player made invalid move so computer wins
  if (invalid) {
    printf("Player %c wins.\n", compColour);
  }
  // Calculate score to determine winner
  else {
    calculateFinalScore(board, userColour, compColour, n);
  }

  printf("Calculation went over the time limit %d times.\n", overTimeCount);

  /* Print possible moves from current board position
  printf("Available moves for User:\n");
  for (int i = 0; i < numUserMoves; i++) {
    printf("%s\n", validUserMoves[i]);
  }
  printf("Available moves for Computer:\n");
  for (int j = 0; j < numComputerMoves; j++) {
    printf("%s\n", validComputerMoves[j]);
  }
  */

  return 0;
}
// DO NOT REMOVE THE FOLLOWING LINE
#endif
// DO NOT REMOVE THE ABOVE LINE
//*******************************************************

// Displays current board position
void printBoard(char board[][26], int n) {
  // Rows
  for (int row = 0; row < n + 1; row++) {
    // Cols
    for (int col = 0; col < n + 1; col++) {
      // Print grid header
      if (row == 0 && col == 0) {
        printf("  ");
      } else if (row == 0 && col > 0) {
        printf("%c", 'a' + col - 1);
      }
      // Print grid header
      else if (col == 0 && row > 0) {
        printf("%c ", 'a' + row - 1);
      } else if (col > 0 && row > 0) {
        // Print current spot on board
        printf("%c", board[row - 1][col - 1]);
      }
    }
    printf("\n");
  }
}

// Setups the board to starting to position
void initializeBoard(char board[][26], int n) {
  // Fill board with unoccupied spaces
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      board[row][col] = 'U';
    }
  }

  // Center tokens
  board[n / 2][n / 2] = 'W';
  board[(n / 2) - 1][(n / 2) - 1] = 'W';
  board[n / 2][(n / 2) - 1] = 'B';
  board[(n / 2) - 1][n / 2] = 'B';
}

// Checks that row and col are valid locations in grid
bool validRowCol(int row, int col, int n) {
  // Check for valid size of row and col
  if ((row >= 0 && row < n) && (col >= 0 && col < n)) {
    return true;
  }

  return false;
}

// Moves coordinate position
void updateRowCol(int* row, int* col, int dir, bool forward) {
  // Moving in forward direction
  if (forward) {
    // Move up
    if (dir == 0) {
      *row -= 1;
    }
    // Move diag up and right
    else if (dir == 1) {
      *row -= 1;
      *col += 1;
    }
    // Move right
    else if (dir == 2) {
      *col += 1;
    }
    // Move diag up and left
    else if (dir == 3) {
      *row -= 1;
      *col -= 1;
    }
  } else {
    // Move down
    if (dir == 0) {
      *row += 1;
    }
    // Move diag down and left
    else if (dir == 1) {
      *row += 1;
      *col -= 1;
    }
    // Move left
    else if (dir == 2) {
      *col -= 1;
    }
    // Move diag down and right
    else if (dir == 3) {
      *row += 1;
      *col += 1;
    }
  }
}

// Determines if the player's token is found in a line connected by opponent's
// tokens from a specfied point
bool inLine(int row, int col, int direction, int n, char board[][26],
            char player, bool forward) {
  // Token opposite token colour to player
  char oppPlayer = 'B';
  if (player == 'B') {
    oppPlayer = 'W';
  }

  // Move in direction specified
  updateRowCol(&row, &col, direction, forward);

  // No line if adjacent token is not opponent's
  if ((!validRowCol(row, col, n) ||
       (validRowCol(row, col, n) && board[row][col] != oppPlayer))) {
    return false;
  }

  // Check line direction
  while (validRowCol(row, col, n) && board[row][col] == oppPlayer) {
    // Increment location to look at
    updateRowCol(&row, &col, direction, forward);
  }
  // Same colour token connected by opposite colour found
  if (validRowCol(row, col, n) && board[row][col] == player) {
    return true;
  }

  // Did not find same colour token connected by opposite colour
  return false;
}

// Find valid moves - returns number of moves found
int findValidMoves(char validMoves[][3], char player, char board[][26], int n) {
  // Used to increment number of valid moves found
  int count = 0;

  // Check every possible grid position
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      // Tile is unoccupied
      if (board[row][col] == 'U') {
        // Check each direction to see if the another of the player's tokens is
        // in line with the current position connected by opponent's tokens
        for (int dir = 0; dir < 4; dir++) {
          // Valid move found
          if (inLine(row, col, dir, n, board, player, true) ||
              inLine(row, col, dir, n, board, player, false)) {
            // Create string to store move
            char move[] = {'a' + row, 'a' + col, '\0'};
            strcpy(validMoves[count], move);
            count++;
            break;
          }
        }
      }
    }
  }

  return count;
}

// Checks if the inputted move is valid
bool checkValidMove(char validMoves[][3], char row, char col, int length) {
  // Compare each valid move to inputted move
  for (int i = 0; i < length; i++) {
    if (validMoves[i][0] == row && validMoves[i][1] == col) {
      return true;
    }
  }
  // Inputted move not in the valid move list
  return false;
}

// Get next move from the player
bool getMove(char validUserMoves[][3], int numUserMoves, char board[][26],
             int n, char userColour) {
  // Characters used to store board position
  char row;
  char col;

  // Get move from the user
  printf("Enter move for colour %c (RowCol): ", userColour);
  scanf("%c%c%*c", &row, &col);

  if (checkValidMove(validUserMoves, row, col, numUserMoves)) {
    // Updates game board
    updateGame(board, row, col, userColour, n);
    printBoard(board, n);
    // Valid move
    return false;
  }
  // invalid move
  else {
    printf("Invalid move.\n");
    // Invalid move
    return true;
  }
}

// Function determines the computer's move
// Uses basic algorithm of choosing the tile that generates the largest score
void computerMove(char validCompMoves[][3], int numCompMoves, char board[][26],
                  int n, char compColour) {
  // Store the highest possible score as a result of a move
  int highestScore = 0;
  // Store the current score
  int currentScore;
  // Stores move that generates highest score
  char move[3] = {'\0'};
  // Stores temporary copy of the game board so the original position is not
  // lost
  char cpyboard[26][26] = {'\0'};

  // Iterate through all valid moves and determine which is best
  for (int i = 0; i < numCompMoves; i++) {
    // Copy original board to cpyboard
    copyArray(board, cpyboard, n);
    // update copy of the board with the current move
    updateGame(cpyboard, validCompMoves[i][0], validCompMoves[i][1], compColour,
               n);
    // Calculate projected score
    currentScore = calculateScore(cpyboard, compColour, n);

    // Updates highest score and move to get highest score
    if (currentScore > highestScore) {
      highestScore = currentScore;
      strcpy(move, validCompMoves[i]);
    }
  }

  // Make move
  printf("Computer places %c at %c%c\n", compColour, move[0], move[1]);
  updateGame(board, move[0], move[1], compColour, n);
  printBoard(board, n);
}

// Makes random moves
void randomMoveGenerator(char validMoves[][3], int numMoves, char board[][26],
                         int n, char turn) {
  // Create seed
  srand(time(NULL));

  // Choose move
  int randInt = rand() % numMoves;

  updateGame(board, validMoves[randInt][0], validMoves[randInt][1], turn, n);
  printBoard(board, n);
}

// Improved ai to make moves
int makeMove(char board[][26], int n, char turn, int* row, int* col) {
  // Get start clock
  clock_t start = clock();
  // Stores possible moves
  char possibleMoves[60][3] = {'\0'};
  // Stores number of possible moves
  // Updates list of possible moves
  int numPossibleMoves = findValidMoves(possibleMoves, turn, board, n);
  // Sorts the move list to prioritize checking edge and corners
  prioritizeMoves(possibleMoves, numPossibleMoves);
  // Determine opponent's colour
  char opponent = 'B';
  if (turn == 'B') {
    opponent = 'W';
  }
  // Stores score for best move
  int bestScore =
      -99999;  // Best move will be stored within the row and col variables

  // Test each possible move
  for (int i = 0; i < numPossibleMoves; i++) {
    // Create copy of the board and apply move to it
    char tempBoard[26][26];
    copyArray(board, tempBoard, n);
    updateGame(tempBoard, possibleMoves[i][0], possibleMoves[i][1], turn, n);

    // Evaluate board position
    int val;
    // Stores turn number
    int turnNum = getTurnNumber(board, n);

    // Look ahead further in endgame
    if (turnNum >= 49) {
      val =
          minimaxValue(tempBoard, 11, turn, opponent, n, -99999, 99999, start);
    }
    // Look ahead in opening to depth 6
    else if (turnNum <= 4) {
      val = minimaxValue(tempBoard, 6, turn, opponent, n, -99999, 99999, start);
    }
    // Main game can only go to depth 4 without exceeding 1 sec
    else {
      val = minimaxValue(tempBoard, 4, turn, opponent, n, -99999, 99999, start);
    }

    // Update best move
    if (val > bestScore) {
      bestScore = val;
      *row = possibleMoves[i][0] - 'a';
      *col = possibleMoves[i][1] - 'a';
    }

    if ((clock() - start) * 1000 / CLOCKS_PER_SEC > 985) {
      break;
    }
  }

  return 0;
}

// Orders the inputted moves to prioritize statistically better moves
void prioritizeMoves(char possibleMoves[][3], int numMoves) {
  // Prioritize moves that take edges and corners
  int frontOfListInd = 0;

  // Iterate through move list
  for (int i = 0; i < numMoves; i++) {
    // Check if move is and edge or corner
    if (possibleMoves[i][0] == 'a' || possibleMoves[i][0] == 'h' ||
        possibleMoves[i][1] == 'a' || possibleMoves[i][1] == 'h') {
      // temp to store string to be moved to end of list
      char* temp = possibleMoves[frontOfListInd];
      // Swap positions of strings
      strcpy(possibleMoves[frontOfListInd], possibleMoves[i]);
      strcpy(possibleMoves[i], temp);
      // Increment front position
      frontOfListInd++;
    }
  }
}

// Minimax function to determine best possible move
int minimaxValue(char board[][26], int depth, char originalTurn,
                 char currentTurn, int n, int alpha, int beta, clock_t start) {
  // Return static evaluation of board one we reached end of the game or
  // requried depth or time limit reached
  if (depth == 0 || gameOver(board, n) ||
      (clock() - start) * 1000 / CLOCKS_PER_SEC > 985) {
    return heuristic(board, originalTurn, n);
  }

  // Stores the possible moves for the current turn
  char possibleMoves[60][3] = {'\0'};
  // Stores number of possible moves
  int numMoves = findValidMoves(possibleMoves, currentTurn, board, n);
  // Sorts the move list to prioritize checking edge and corners
  prioritizeMoves(possibleMoves, numMoves);
  // Assign opponent's colour
  char opponent = 'B';
  if (currentTurn == 'B') {
    opponent = 'W';
  }

  // Skip turn if no possible moves
  if (numMoves == 0) {
    return minimaxValue(board, depth - 1, originalTurn, opponent, n, alpha,
                        beta, start);
  }

  // Store best possible move
  int bestScore = -9999;  // For finding max
  if (currentTurn != originalTurn) {
    bestScore *= -1;  // Finding min for opponent
  }

  // Consider each possible move
  for (int i = 0; i < numMoves; i++) {
    // Apply the move to a copy of the board
    char tempBoard[26][26];
    copyArray(board, tempBoard, n);
    updateGame(tempBoard, possibleMoves[i][0], possibleMoves[i][1], currentTurn,
               n);

    // Recursive call
    int val = minimaxValue(tempBoard, depth - 1, originalTurn, opponent, n,
                           alpha, beta, start);

    // Remember best move
    if (currentTurn == originalTurn) {
      // Remember max score if it is the original player's turn
      if (val > bestScore) {
        bestScore = val;
      }
      // Update alpha value
      if (val > alpha) {
        alpha = val;
      }
      // Prune the rest of the child nodes
      if (beta <= alpha) {
        break;
      }
    } else {
      // Remember min score if its the opponent's turn
      if (val < bestScore) {
        bestScore = val;
      }
      // Update beta value
      if (val < beta) {
        beta = val;
      }
      // Prune the rest of the child nodes
      if (beta <= alpha) {
        break;
      }
    }
  }

  return bestScore;
}

// Determines evaluation for board position
int heuristic(char board[][26], char turn, int n) {
  // Use parity score to determine best move when final board position is known
  if (gameOver(board, n)) {
    return getParityScore(board, turn, n);
  }

  // Calculate heuristic score based on corner score, edge score, and mobility
  // score Change weights dynamically
  double stabilityWeight;
  double mobilityWeight;
  int turnNumber = getTurnNumber(board, n);

  //  Early game weighting --> mobility more important
  if (turnNumber <= 20) {
    mobilityWeight = 1.5;
    stabilityWeight = 0.5;
  }
  // Mid game weighting
  else if (turnNumber <= 40) {
    mobilityWeight = 0.8;
    stabilityWeight = 1.25;
  }
  // End game weighting
  else {
    mobilityWeight = 0.25;
    stabilityWeight = 1.75;
  }

  // Return determined score
  return getCornerScore(board, turn, n) +
         mobilityWeight * getEdgeScore(board, turn, n) +
         stabilityWeight * getMobilityScore(board, turn, n);
}

// Determines the current turn number
int getTurnNumber(char board[][26], int n) {
  // Stores number of empty tiles found
  int count = 0;

  // Finds empty tiles
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == 'U') {
        count++;
      }
    }
  }

  // n*n - 4 is total number of turns
  return (n * n - 4) - count;
}

// Gets mobility score
int getMobilityScore(char board[][26], char turn, int n) {
  // Stores mobility score
  int score = 0;
  char opp = 'W';
  if (turn == 'W') {
    opp = 'B';
  }

  // Give score based on number of availiable moves for current player
  char moves[60][3];
  score += findValidMoves(moves, turn, board, n) * 5;

  // Gives score based on potential moves available
  // Decreases score for empty slots next to own disk
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == turn) {
        // Opponent's pot moves
        score -= getOpenTiles(board, n, row, col) * 2;
      } else if (board[row][col] == opp) {
        // Player's pot moves
        score += getOpenTiles(board, n, row, col) * 2;
      }
    }
  }

  return score;
}

// Returns number of surround open tiles
int getOpenTiles(char board[][26], int n, int row, int col) {
  // Keeps track of number of open tiles
  int count = 0;

  // Check forward directions
  for (int i = 0; i < 4; i++) {
    int cpyrow = row;
    int cpycol = col;

    updateRowCol(&cpyrow, &cpycol, i, true);
    if (validRowCol(cpyrow, cpycol, n) && board[cpyrow][cpycol] == 'U') {
      count++;
    }
  }

  // Check backwards directions
  for (int j = 0; j < 4; j++) {
    int cpyrow = row;
    int cpycol = col;

    updateRowCol(&cpyrow, &cpycol, j, false);
    if (validRowCol(cpyrow, cpycol, n) && board[cpyrow][cpycol] == 'U') {
      count++;
    }
  }

  return count;
}

// Gets corner score
int getCornerScore(char board[][26], char turn, int n) {
  /*
  Corner Positions:
  {0, 0}, {n-1, n-1}, {0, n-1}, {n-1, 0}
  */
  // Stores corner score
  int score = 0;
  char opp = 'W';
  if (turn == 'W') {
    opp = 'B';
  }

  // Check for captured corners and tiles next to captured corners
  if (board[0][0] == turn) {
    score += 99;
    // Additional spaces captured next to owned corner
    if (board[1][1] == turn) {
      score += 20;
    }
    if (board[0][1] == turn) {
      score += 30;
    }
    if (board[1][0] == turn) {
      score += 30;
    }
  } else if (board[0][0] == opp) {
    score -= 99;
    // Spaces captured next to opposing corner
    if (board[1][1] == turn) {
      score -= 10;
    }
    if (board[0][1] == turn) {
      score -= 15;
    }
    if (board[1][0] == turn) {
      score -= 15;
    }
  } else {
    // Corner is uncaptured
    if (board[1][1] == turn) {
      score -= 50;
    }
    if (board[0][1] == turn) {
      score -= 20;
    }
    if (board[1][0] == turn) {
      score -= 20;
    }
  }

  if (board[n - 1][n - 1] == turn) {
    score += 99;
    // Additional spaces captured next to owned corner
    if (board[n - 2][n - 2] == turn) {
      score += 20;
    }
    if (board[n - 1][n - 2] == turn) {
      score += 30;
    }
    if (board[n - 2][n - 1] == turn) {
      score += 30;
    }
  } else if (board[n - 1][n - 1] == opp) {
    score -= 99;
    // Spaces captured next to opposing corner
    if (board[n - 2][n - 2] == turn) {
      score -= 10;
    }
    if (board[n - 1][n - 2] == turn) {
      score -= 15;
    }
    if (board[n - 2][n - 1] == turn) {
      score -= 15;
    }
  } else {
    // Corner is uncaptured
    if (board[n - 2][n - 2] == turn) {
      score -= 50;
    }
    if (board[n - 1][n - 2] == turn) {
      score -= 20;
    }
    if (board[n - 2][n - 1] == turn) {
      score -= 20;
    }
  }

  if (board[0][n - 1] == turn) {
    score += 99;
    // Additional spaces captured next to owned corner
    if (board[1][n - 2] == turn) {
      score += 20;
    }
    if (board[0][n - 2] == turn) {
      score += 30;
    }
    if (board[1][n - 1] == turn) {
      score += 30;
    }
  } else if (board[0][n - 1] == opp) {
    score -= 99;
    // Additional spaces captured next to opposing corner
    if (board[1][n - 2] == turn) {
      score -= 10;
    }
    if (board[0][n - 2] == turn) {
      score -= 15;
    }
    if (board[1][n - 1] == turn) {
      score -= 15;
    }
  } else {
    // Corner is uncaptured
    if (board[1][n - 2] == turn) {
      score -= 50;
    }
    if (board[0][n - 2] == turn) {
      score -= 20;
    }
    if (board[1][n - 1] == turn) {
      score -= 20;
    }
  }

  if (board[n - 1][0] == turn) {
    score += 99;
    // Additional spaces captured next to owned corner
    if (board[n - 2][1] == turn) {
      score += 20;
    }
    if (board[n - 2][0] == turn) {
      score += 30;
    }
    if (board[n - 1][1] == turn) {
      score += 30;
    }
  } else if (board[n - 1][0] == opp) {
    score -= 99;
    // Additional spaces captured next to opposing corner
    if (board[n - 2][1] == turn) {
      score -= 10;
    }
    if (board[n - 2][0] == turn) {
      score -= 15;
    }
    if (board[n - 1][1] == turn) {
      score -= 15;
    }
  } else {
    // Corner is uncaptured
    if (board[n - 2][1] == turn) {
      score -= 50;
    }
    if (board[n - 2][0] == turn) {
      score -= 20;
    }
    if (board[n - 1][1] == turn) {
      score -= 20;
    }
  }

  return score;
}

// Gets edge score
int getEdgeScore(char board[][26], char turn, int n) {
  // Stores score
  int score = 0;

  // Increase score for each edge owned, decrease score for every edge owned by
  // opponent
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      // Tile on the edge that is not empty
      if ((row == 0 || row == n - 1 || col == 0 || col == n - 1) &&
          board[row][col] != 'U') {
        // Player owns edge
        if (board[row][col] == turn) {
          score += 3;
          // Check if stable disk
          if (isStable(board, board[row][col], n, row, col)) {
            score += 25;
          }
        }
        // Opponent owns edge
        else {
          score -= 3;
          if (isStable(board, board[row][col], n, row, col)) {
            score -= 25;
          }
        }
      }
    }
  }
  return score;
}

// Determines if the current disk is stable
bool isStable(char board[][26], char turn, int n, int row, int col) {
  // Only need to check row
  if (row == 0 || row == n - 1) {
    int cpycol = col;
    // Check to the right
    while (validRowCol(row, cpycol, n) && board[row][cpycol] == turn) {
      updateRowCol(&row, &cpycol, 2, true);
    }
    // Went off board before encountering blank or opponent
    if (!validRowCol(row, cpycol, n)) {
      return true;
    }

    // Check to the left
    while (validRowCol(row, col, n) && board[row][col] == turn) {
      updateRowCol(&row, &col, 2, false);
    }
    // Went off board before encountering blank or opponent
    if (!validRowCol(row, col, n)) {
      return true;
    }
  }
  // Only need to check column
  else {
    int cpyrow = row;
    // Check up
    while (validRowCol(cpyrow, col, n) && board[cpyrow][col] == turn) {
      updateRowCol(&cpyrow, &col, 0, true);
    }
    // Went off board before encountering blank or opponent
    if (!validRowCol(cpyrow, col, n)) {
      return true;
    }

    // Check down
    while (validRowCol(row, col, n) && board[row][col] == turn) {
      updateRowCol(&row, &col, 0, false);
    }
    // Went off board
    if (!validRowCol(row, col, n)) {
      return true;
    }
  }

  return false;
}

// Get parity score
int getParityScore(char board[][26], char turn, int n) {
  char opp = 'W';
  if (turn == 'W') {
    opp = 'B';
  }

  // Returns player's score minus opponent's score
  return calculateScore(board, turn, n) - calculateScore(board, opp, n);
}

// Updates board based on player's move
void updateGame(char board[][26], char row, char col, char player, int n) {
  // Create copy of row and col
  int cpyrow;
  int cpycol;
  // Token opposite token colour to player
  char oppPlayer = 'B';
  if (player == 'B') {
    oppPlayer = 'W';
  }

  // Update board with new move
  board[row - 'a'][col - 'a'] = player;

  // Flip over all tokens between the player's tokens
  // Check forward direction
  for (int dir = 0; dir < 4; dir++) {
    // Reset values
    cpyrow = row - 'a';
    cpycol = col - 'a';

    // Flip tokens in this direction
    if (inLine(cpyrow, cpycol, dir, n, board, player, true)) {
      // Reset values
      cpyrow = row - 'a';
      cpycol = col - 'a';
      updateRowCol(&cpyrow, &cpycol, dir, true);
      while (validRowCol(cpyrow, cpycol, n) &&
             board[cpyrow][cpycol] == oppPlayer) {
        board[cpyrow][cpycol] = player;
        // Increment location to look at
        updateRowCol(&cpyrow, &cpycol, dir, true);
      }
    }
  }

  // Flip in the backwards direction
  for (int dir = 0; dir < 4; dir++) {
    // Reset values
    cpyrow = row - 'a';
    cpycol = col - 'a';

    // Flip tokens in this direction
    if (inLine(cpyrow, cpycol, dir, n, board, player, false)) {
      // Reset values
      cpyrow = row - 'a';
      cpycol = col - 'a';
      updateRowCol(&cpyrow, &cpycol, dir, false);
      while (validRowCol(cpyrow, cpycol, n) &&
             board[cpyrow][cpycol] == oppPlayer) {
        board[cpyrow][cpycol] = player;
        // Increment location to look at
        updateRowCol(&cpyrow, &cpycol, dir, false);
      }
    }
  }
}

// Calculates the current score of the board
int calculateScore(char board[][26], char colour, int n) {
  int score = 0;

  // Check each tile and add to score if it finds the corresponding colour
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == colour) {
        score++;
      }
    }
  }

  return score;
}

// Determines is the game is over
bool gameOver(char board[][26], int n) {
  // Find available moves
  char moves[60][3];
  int movesB = findValidMoves(moves, 'B', board, n);
  int movesW = findValidMoves(moves, 'W', board, n);

  // If no possible moves game is over
  if (movesB == 0 && movesW == 0) {
    return true;
  }

  return false;
}

// Calculates the final score of the board
void calculateFinalScore(char board[][26], char userColour, char compColour,
                         int n) {
  // Stores the score of the user
  int userScore = 0;
  // Stores the score of the computer
  int compScore = 0;

  // Check each tile on board and add score if it finds corresponding colour
  for (int row = 0; row < n; row++) {
    for (int col = 0; col < n; col++) {
      if (board[row][col] == userColour) {
        userScore++;
      } else if (board[row][col] == compColour) {
        compScore++;
      }
    }
  }

  // Player wins
  if (userScore > compScore) {
    printf("%c player wins.\n", userColour);
  }
  // Computer wins
  else if (compScore > userScore) {
    printf("%c player wins.\n", compColour);
  }
  // Player and computer draws
  else {
    printf("Draw!\n");
  }
}

// Copies content of one array to a new array
void copyArray(char board[][26], char copy[][26], int n) {
  // Copies each row to the new array
  for (int i = 0; i < n; i++) {
    memcpy(copy[i], board[i], ((int)sizeof(char)) * n);
  }
}
