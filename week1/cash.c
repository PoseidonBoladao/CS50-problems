#include <cs50.h>
#include <stdio.h>

int calculate_coins(int change);

int main(void)
{
    int change;
    do
    {
        change = get_int("Change: ");
    }
    while (change < 0);

    int output = calculate_coins(change);
    printf("%i\n", output);
}

int calculate_coins(int change)
{
    int coins = 0;
    int coin_values[] = {25, 10, 5, 1};
    const int size = 4;
    for (int i = 0; i < size; i++)
    {
        while (change >= coin_values[i])
        {
            change = change - coin_values[i];
            coins++;
        }
    }
    return coins;
}
