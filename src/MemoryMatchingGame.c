#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX 6

void initializeBoard(char board[MAX][MAX], char hidden[MAX][MAX], int size);
void shuffleCards(char cards[], int total);
void displayBoard(char hidden[MAX][MAX], int size);
int allMatched(char hidden[MAX][MAX], int size);
void saveResult(char player1[], char player2[], int p1Score, int p2Score, int size, int moves);

int main() {
    char board[MAX][MAX], hidden[MAX][MAX];
    char player1[50], player2[50];
    int p1Score = 0, p2Score = 0;
    int x1, y1, x2, y2;
    int turn = 1, moves = 0;
    int size, choice;

    srand(time(0));

    printf("MEMORY MATCHING GAME (2 PLAYERS)\n");

    printf("Enter name of Player 1: ");
    scanf(" %[^\n]", player1);
    printf("Enter name of Player 2: ");
    scanf(" %[^\n]", player2);

    printf("\nSelect Difficulty Level:\n");
    printf("1. Easy (2x2)\n");
    printf("2. Medium (4x4)\n");
    printf("3. Hard (6x6)\n");
    printf("Enter your choice: ");
    scanf("%d", &choice);

    if (choice == 1) size = 2;
    else if (choice == 2) size = 4;
    else size = 6;

    initializeBoard(board, hidden, size);

    printf("\nGame starts now! %s will begin.\n", player1);

    while (!allMatched(hidden, size)) {
        displayBoard(hidden, size);
        printf("\nTurn: %s\n", (turn == 1) ? player1 : player2);
        printf("Enter first card (row col): ");
        scanf("%d %d", &x1, &y1);
        printf("Enter second card (row col): ");
        scanf("%d %d", &x2, &y2);

        if (x1 < 0 || x1 >= size || y1 < 0 || y1 >= size ||
            x2 < 0 || x2 >= size || y2 < 0 || y2 >= size) {
            printf("\nInvalid coordinates! Try again.\n");
            continue;
        }
        if (hidden[x1][y1] != '*' || hidden[x2][y2] != '*') {
            printf("\nCard already open! Try again.\n");
            continue;
        }

        hidden[x1][y1] = board[x1][y1];
        hidden[x2][y2] = board[x2][y2];
        displayBoard(hidden, size);
        moves++;

        if (board[x1][y1] == board[x2][y2]) {
            printf("Match Found!");
            if (turn == 1) {
                p1Score++;
                printf("%s scores a point!\n", player1);
            } else {
                p2Score++;
                printf("%s scores a point!\n", player2);
            }
        } else {
            printf("Not a match! Turn passes.\n");
            hidden[x1][y1] = '*';
            hidden[x2][y2] = '*';
            turn = (turn == 1) ? 2 : 1;
        }

        printf("\nCurrent Scoreboard:\n");
        printf("%s: %d points\n", player1, p1Score);
        printf("%s: %d points\n", player2, p2Score);
    }

    printf("\nAll pairs matched!\n");
    printf("\nFinal Scores:\n");
    printf("%s: %d points\n", player1, p1Score);
    printf("%s: %d points\n", player2, p2Score);

    if (p1Score > p2Score)
        printf("\nWinner: %s!\n", player1);
    else if (p2Score > p1Score)
        printf("\nWinner: %s!\n", player2);
    else
        printf("\nIt's a Draw!\n");

    printf("\nTotal Moves: %d\n", moves);

    saveResult(player1, player2, p1Score, p2Score, size, moves);

    printf("\nGame results saved successfully in 'scores.txt'!\n");
    return 0;
}

void initializeBoard(char board[MAX][MAX], char hidden[MAX][MAX], int size) {
    int total = size * size;
    char cards[total];
    char letter = 'A';
    for (int i = 0; i < total; i += 2) {
        cards[i] = letter;
        cards[i + 1] = letter;
        letter++;
    }
    shuffleCards(cards, total);

    int k = 0;
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++) {
            board[i][j] = cards[k++];
            hidden[i][j] = '*';
        }
}

void shuffleCards(char cards[], int total) {
    for (int i = 0; i < total; i++) {
        int r = rand() % total;
        char temp = cards[i];
        cards[i] = cards[r];
        cards[r] = temp;
    }
}

void displayBoard(char hidden[MAX][MAX], int size) {
    printf("\n   ");
    for (int i = 0; i < size; i++) printf("%2d ", i);
    printf("\n");
    for (int i = 0; i < size; i++) {
        printf("%2d ", i);
        for (int j = 0; j < size; j++) {
            printf(" %c ", hidden[i][j]);
        }
        printf("\n");
    }
}

int allMatched(char hidden[MAX][MAX], int size) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < size; j++)
            if (hidden[i][j] == '*')
                return 0;
    return 1;
}

void saveResult(char player1[], char player2[], int p1Score, int p2Score, int size, int moves) {
    FILE *fp = fopen("scores.txt", "a");
    if (fp == NULL) {
        printf("Error saving result!\n");
        return;
    }

    fprintf(fp, "Board Size: %dx%d | Moves: %d | %s: %d | %s: %d | Winner: ",
            size, size, moves, player1, p1Score, player2, p2Score);

    if (p1Score > p2Score)
        fprintf(fp, "%s\n", player1);
    else if (p2Score > p1Score)
        fprintf(fp, "%s\n", player2);
    else
        fprintf(fp, "Draw\n");

    fclose(fp);
}