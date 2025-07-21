#include <cs50.h>
#include <stdio.h>

void print_space(int height);
void print_brick(int height);
int main(void)
{
    int input;
    do
    {
        input = get_int("How high is the pyramid? (Between 1 and 8) ");
    }
    while (input < 1 || input > 8);

    int bricks = 1;
    do
    {
        print_space(input);
        print_brick(bricks);
        printf("  ");
        print_brick(bricks);
        input--;
        bricks++;
        printf("\n");
    }
    while (input > 0);
}

void print_space(int height)
{
    for (int i = 1; i < height; i++)
    {
        printf(" ");
    }
}

void print_brick(int height)
{
    for (int j = 0; j < height; j++)
    {
        printf("#");
    }
}
