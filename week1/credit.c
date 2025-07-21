#include <cs50.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int get_card_type(long card, int digits);
bool is_valid_checksum(long card);
int sum_of_double(int digit);
#define AMEX 1
#define MASTERCARD 2
#define VISA 3
#define INVALID 0
int main(void)
{
    // prompt user for credit card
    long card = get_long("Card number: ");
    char cardst[20];
    sprintf(cardst, "%li", card);
    int digits = strlen(cardst);

    // check for AMEX VISA MASTERCARD INVALID
    if (digits == 15 && is_valid_checksum(card) && get_card_type(card, digits) == AMEX)
    {
        printf("AMEX\n");
        return 0;
    }
    else if (digits == 16 && is_valid_checksum(card) && get_card_type(card, digits) == MASTERCARD)
    {
        printf("MASTERCARD\n");
        return 0;
    }
    else if ((digits == 13 || digits == 16) && is_valid_checksum(card) &&
             get_card_type(card, digits) == VISA)
    {
        printf("VISA\n");
        return 0;
    }
    else
    {
        printf("INVALID\n");
    }
}

bool is_valid_checksum(long card)
{
    int x = 0;
    int y = 0;
    int last_digit;
    int before_last_digit;
    while (card > 0)
    {
        last_digit = card % 10;
        x = x + last_digit;
        card = card / 10;
        before_last_digit = card % 10;
        y = sum_of_double(before_last_digit);
        x = x + y;
        y = 0;
        card = card / 10;
    }
    if (x % 10 != 0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int sum_of_double(int digit)
{
    int doubled = digit * 2;
    if (doubled >= 10)
    {
        return (doubled % 10) + (doubled / 10);
    }
    else
    {
        return doubled;
    }
}

int get_card_type(long card, int digits)
{
    int d = digits - 2;
    long power = pow(10, d);
    int first_2_numbers = card / power;
    if (first_2_numbers == 34 || first_2_numbers == 37)
    {
        return 1;
    }
    else if (first_2_numbers == 51 || first_2_numbers == 52 || first_2_numbers == 53 ||
             first_2_numbers == 54 || first_2_numbers == 55)
    {
        return 2;
    }
    else if (first_2_numbers / 10 == 4)
    {
        return 3;
    }
    else
    {
        return 0;
    }
}
