#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void fillBoard(char board[8][8]);
void setupBoard(char board[8][8]);
void placeBoat(char board[8][8], char boat[], int size);
bool isValidY(char y);
bool isValidX(int x);
void printBoard(char board[8][8]);
bool fire(int turn, char input[], char p1Board[8][8], char p2Board[8][8],
          char p1Shots[8][8], char p2Shots[8][8], int p1Lives[5], int p2Lives[5]);
bool isWinner(int pLives[5]);

int main(){
  char p1Board[8][8];
  char p2Board[8][8];
  char p1Shots[8][8];
  char p2Shots[8][8];
  int p1Lives[5] = {5, 4, 3, 2, 2};
  int p2Lives[5] = {5, 4, 3, 2, 2};
  fillBoard(p1Shots);
  fillBoard(p2Shots);
  int turn = 0; 

  printf("Player 1, please set up your ships (y, x, orientation)\n" );
  setupBoard(p1Board);
  printf("\n\nPlayer 2, please set up your ships (y, x, orientation)\n" );
  setupBoard(p2Board);
  printf("\nAll boats set up, Player 1 can now strike.\n\n" );

  char input[10];
  while(1){
    if(fgets(input, 10, stdin) == NULL){
      return 0;
    } else{
      if(strcmp(input, "exit\n") == 0){
        exit(0);
      } else if(strcmp(input, "shots\n") == 0){
        printf("\n");
        if(turn){
          printBoard(p2Shots);
        } else {
          printBoard(p1Shots);
        }
      } else if(strncmp(input, "fire", 4) == 0 && strlen(input) == 9){
        if(fire(turn, input, p1Board, p2Board, p1Shots, p2Shots, p1Lives, p2Lives)){
          if(turn){
            if(isWinner(p1Lives)){
              printf("Player 2 wins!\n" );
              exit(0);
            }
            turn = 0;
            printf("Player 1's turn\n\n" );
          } else{
            if(isWinner(p2Lives)){
              printf("Player 1 wins!\n" );
              exit(0);
            }
            turn = 1;
            printf("Player 2's turn\n\n" );
          }
        }
      } else if(input[0] == '\n'){
        continue;
      } else{
        printf("\nUnrecognised command\n\n");
      }
    }
  }

  //printBoard(p1Board);
  //printBoard(p2Board);
  return 0;
}


void fillBoard(char board[8][8]){
  for(int x = 0; x < 8; x ++){
    for(int y = 0; y < 8; y++){
      board[x][y] = ' ';
    }
  }
}

void setupBoard(char board[8][8]){
  fillBoard(board);
  placeBoat(board, "Carrier", 5);
  placeBoat(board, "Battleship", 4);
  placeBoat(board, "Destroyer", 3);
  placeBoat(board, "Submarine", 2);
  placeBoat(board, "Patrol Boat", 2);
}

void placeBoat(char board[8][8], char boat[], int size){
  char input[20];
  int y;
  int x;
  char dir;
  bool valid = false;
  while(!valid){
    valid = true;
    printf("%s: ", boat);
    fgets(input, 20, stdin);
    if(strlen(input) != 6 || !isValidY(input[0])        || input[1] != ' '
                          || !isValidX(atoi(&input[2])) || input[3] != ' '
                          || !(input[4] == 'H'          || input[4] == 'V')){
      valid = false;
    } else {
      y = input[0] - 64;
      x = atoi(&input[2]);
      dir = input[4];
      if(dir == 'H'){
        for(int i = x - 1; i < size + x - 1; i ++){
          if(i > 7 || board[y-1][i] != ' '){
            valid = false;
            break;
          }
        }
      } else {
        for(int i = y - 1; i < size + y - 1; i++){
          if(i > 7 || board[i][x-1] != ' '){
            valid = false;
            break;
          }
        }
      }
    }
    if(!valid){
      printf("Invalid ship configuration\n");
    }
  }
  if(dir == 'H'){
    for(int i = x - 1; i < size + x - 1; i ++){
      board[y-1][i] = boat[0];
    }
  } else {
    for(int i = y - 1; i < size + y - 1; i++){
      board[i][x-1] = boat[0];
    }
  }
}

bool isValidY(char y){
  char validY[] = {'A','B','C','D','E','F','G','H'};
  for(int i = 0; i < 8; i++){
    if(y == validY[i]){
      return true;
    }
  }
  return false;
}

bool isValidX(int x){
  int validX[] = {1,2,3,4,5,6,7,8};
  for(int i = 0; i < 8; i++){
    if(x == validX[i]){
      return true;
    }
  }
  return false;
}

void printBoard(char board[8][8]){
  printf("  1 2 3 4 5 6 7 8\n");
  for(int y = 0; y < 8; y++){
    printf("%c|", y + 65);
    for(int x = 0; x < 8; x++){
      printf("%c|", board[y][x]);
    }
    printf("\n");
  }
  printf("\n");
}

bool fire(int turn, char input[], char p1Board[8][8], char p2Board[8][8], char p1Shots[8][8], char p2Shots[8][8], int p1Lives[5], int p2Lives[5]){
  if(!(input[4] == ' ' && input[6] == ' ')){
    printf("\nUnrecognised command\n\n");
    return false;
  }
  if(!isValidY(input[5]) || !isValidX(atoi(&input[7]))){
    printf("Invalid coordinates\n");
    return false;
  }
  int y = input[5] - 65;
  int x = atoi(&input[7])- 1;
  char (*pBoard)[8];
  char (*pShots)[8];
  char (*eBoard)[8];
  int *eLives;
  if(turn){
    pBoard = p2Board;
    pShots = p2Shots;
    eBoard = p1Board;
    eLives = p1Lives;
  } else {
    pBoard = p1Board;
    pShots = p1Shots;
    eBoard = p2Board;
    eLives = p2Lives;
  }
  if(pShots[y][x] != ' '){
    printf("You have already fired at this location\n");
    return false;
  }
  if(eBoard[y][x] != ' '){
    int i = 0;
    char *boat;
    if(eBoard[y][x] == 'C'){
      i = 0;
      boat = "Carrier";
    } else if(eBoard[y][x] == 'B'){
      i = 1;
      boat = "Battleship";
    } else if(eBoard[y][x] == 'D'){
      i = 2;
      boat = "Destroyer";
    } else if(eBoard[y][x] == 'S'){
      i = 3;
      boat = "Submarine";
    } else if(eBoard[y][x] == 'P'){
      i = 4;
      boat = "Patrol Boat";
    }
    eLives[i]--;
    pShots[y][x] = 'X';
    if(eLives[i] == 0){
      printf("\nWe have sunk your %s!\n\n", boat);
    } else {
      printf("\nWe have hit the target!\n\n");
    }
  } else{
    pShots[y][x] = '0';
    printf("\nYou have missed!\n\n");
  }
  return true;
}

bool isWinner(int pLives[5]){
  for(int i = 0; i < 5; i++){
    if(pLives[i] != 0){
      return false;
    }
  }
  return true;
}
