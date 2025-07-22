#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

#define ASCII_SPACE 32
#define ASCII_PERIOD 46
#define ASCII_QUESTION 63
#define ASCII_EXCLAMATION 33
int main(void)
{
    // promp user
    string text = get_string("Text: ");

    // string lenght
    int string_size = strlen(text);

    // count spaces, letters, sentences
    float spaces = 0;
    float sentences = 0;
    float letters = 0;
    for (int i = 0; i < string_size; i++)
    {
        if (text[i] == ASCII_SPACE)
        {
            spaces++;
        }
        if (text[i] == ASCII_PERIOD || text[i] == ASCII_QUESTION || text[i] == ASCII_EXCLAMATION)
        {
            sentences++;
        }
        if (isalpha(text[i]) != 0)
        {
            letters++;
        }
    }
    float words = spaces + 1;

    // index = 0.0588 * L - 0.296 * S - 15.8
    const float constant[4] = {100, 0.0588, 0.296, 15.8};
    float L = (letters / words * constant[0]);
    float S = (sentences / words * constant[0]);
    float index = (constant[1] * L) - (constant[2] * S) - constant[3];
    int rounded = round(index);

    // print output
    if (rounded < 1)
    {
        printf("Before Grade 1\n");
    }
    else if (rounded > 16)
    {
        printf("Grade 16+\n");
    }
    else
    {
        printf("Grade %i\n", rounded);
    }
}
