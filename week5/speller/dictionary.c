// Implements a dictionary's functionality

#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
} node;

// TODO: Choose number of buckets in hash table
const unsigned int N = 17579;

// number of words
int words = 0;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // hashes word
    unsigned int hashed = hash(word);
    int length = strlen(word);
    char temp_word[length];
    int i;

    // convert word to lower case
    for (i = 0; i < length; i++)
    {
        temp_word[i] = tolower(word[i]);
    }

    // terminates word
    temp_word[i] = '\0';

    // start with the header
    node *n = table[hashed];

    // iterate through the linked list
    while (n != NULL)
    {
        // compare input word with current node
        if (strcmp(temp_word, n->word) == 0)
        {
            return true;
        }

        // moves to next node
        n = n->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    int temp = 0;
    // hashes first two characters
    if (strlen(word) == 1 || word[1] == '\'' || word[2] == '\'')
    {
        temp = ((toupper(word[0]) - 'A') * 26 * 26);
    }
    else if (strlen(word) == 2)
    {
        temp = ((toupper(word[0]) - 'A') * 26 * 26) + ((toupper(word[1]) - 'A') * 26);
    }
    else
    {
        temp = ((toupper(word[0]) - 'A') * 26 * 26) + ((toupper(word[1]) - 'A') * 26) +
               (toupper(word[2]) - 'A');
    }
    return temp;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // TODO
    // populate hashtable with NULL values
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }

    // open dicrionary
    FILE *file = fopen(dictionary, "r");
    if (file == NULL)
    {
        return false;
    }

    // get ready to load dictionary to memory
    char c;
    char temp_word[LENGTH + 1];
    int index = 0;
    unsigned int hashed = 0;

    // load dictionary to memory
    while (fread(&c, sizeof(char), 1, file))
    {
        // check for end of word
        if (c == '\n')
        {
            node *n = malloc(sizeof(node));
            if (n == NULL)
            {
                unload();
                return false;
            }

            // pass the temporary word to the node
            for (int i = 0; i < index; i++)
            {
                n->word[i] = temp_word[i];
            }

            // terminate word
            n->word[index] = '\0';
            // position word in first position of linked list of hash table
            hashed = hash(n->word);
            n->next = table[hashed];
            table[hashed] = n;
            index = 0;
            words++;
        }
        else
        {
            temp_word[index] = c;
            index++;
        }
    }
    fclose(file);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    if (words > 0)
    {
        return words;
    }

    else
    {
        return 0;
    }
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // set temporary pointer
    node *ptr;

    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            ptr = table[i]->next;
            free(table[i]);
            table[i] = ptr;
        }
    }
    return true;
}
