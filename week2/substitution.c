#include <cs50.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>

#define ALPHABET 26
#define ASCII_upper 65
#define ASCII_lower 97
int check_validity(int argc, string argv[]);
int main(int argc, string argv[])
{
    // check for input validity
    int valid = check_validity(argc, argv);
    if (valid != 0)
    {
        return 1;
    }

    // prompt user for plain text
    string plaintext = get_string("plaintext: ");

    // transform to ciphertext
    int size = strlen(plaintext);
    char ciphertext;
    int index_in_key;
    printf("ciphertext: ");
    for (int i = 0; i < size; i++)
    {
        char c = plaintext[i];
        if (isalpha(c) == 0)
        {
            printf("%c", c);
        }
        else if (islower(c) != 0)
        {
            index_in_key = c - ASCII_lower;
            c = tolower(argv[1][index_in_key]);
            printf("%c", c);
        }
        else if (isupper(c) != 0)
        {
            index_in_key = c - ASCII_upper;
            c = toupper(argv[1][index_in_key]);
            printf("%c", c);
        }
    }
    printf("\n");
    return 0;
}

int check_validity(int argc, string argv[])
{
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    int size = strlen(argv[1]);
    int sum = 0;
    if (size != ALPHABET)
    {
        printf("Key is not 26 character\n");
        return 2;
    }
    char upper[ALPHABET];
    int multiples[ALPHABET] = {0};
    for (int i = 0; i < size; i++)
    {
        if (isalpha(argv[1][i]) == 0)
        {
            printf("Key is not alphabetical characters.\n");
            return 1;
        }
        char c = argv[1][i];
        upper[i] = toupper(c);
        int x = upper[i] - ASCII_upper;
        multiples[x]++;
        if (multiples[x] > 1)
        {
            printf("Key must contain 26 different characters.\n");
            return 3;
        }
    }
    return 0;
}
