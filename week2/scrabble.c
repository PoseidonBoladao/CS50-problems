#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define PLAYERS 2
#define CONVERT_ASCII 65
int point_each_char(int size[PLAYERS], string words[PLAYERS], int i, const int points_per_char[]);
int main(void)
{
    // prompt user for input
    string words[PLAYERS];
    int size[PLAYERS];
    for (int i = 0; i < PLAYERS; i++)
    {
        words[i] = get_string("Player %d: ", i + 1);
        size[i] = strlen(words[i]);
    }

    // define array for points
    const int points_per_char[] = {1, 3, 3, 2,  1, 4, 2, 4, 1, 8, 5, 1, 3,
                                   1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};

    // iterate through every character
    int points[PLAYERS];
    for (int i = 0; i < PLAYERS; i++)
    {
        points[i] = point_each_char(size, words, i, points_per_char);
    }

    // print results
    int p = points[0];
    for (int i = 1; i < PLAYERS; i++)
    {
        if (points[i] > p)
        {
            printf("Player %i wins!\n", i + 1);
        }
        else if (points[i] == p)
        {
            printf("Tie!\n");
        }
        else
        {
            printf("Player 1 wins!\n");
        }
    }
}

int point_each_char(int size[PLAYERS], string words[PLAYERS], int i, const int points_per_char[])
{
    int pontos = 0;
    for (int j = 0; j < size[i]; j++)
    {
        char c = toupper(words[i][j]);
        int ASCII = c;
        int converted = ASCII - CONVERT_ASCII;
        int point_current_char = points_per_char[converted];
        if (isalpha(c) == 0)
        {
            point_current_char = 0;
        }
        pontos = pontos + point_current_char;
    }
    return pontos;
}
