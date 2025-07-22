#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool check_cycle(int i, int j, int k);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // TODO
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (j > i && ranks[i] != ranks[j])
                preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // TODO
    pair_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // strenght of victory
    int strength_vic[pair_count];
    for (int i = 0; i < pair_count; i++)
    {
        strength_vic[i] = preferences[pairs[i].winner][pairs[i].loser];
    }

    // selection sort
    int current_largest;
    int current_largest_j;
    pair n1;
    pair n2;
    int n1_str;
    int n2_str;
    int j;
    for (int i = 0; i < pair_count; i++)
    {
        current_largest = 0;
        for (j = i; j < pair_count; j++)
        {
            if (strength_vic[j] > current_largest)
            {
                current_largest = strength_vic[j];
                n1 = pairs[j];
                n1_str = strength_vic[j];
                current_largest_j = j;
            }
        }
        n2 = pairs[i];
        n2_str = strength_vic[i];
        strength_vic[current_largest_j] = n2_str;
        strength_vic[i] = n1_str;
        pairs[i] = n1;
        pairs[current_largest_j] = n2;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    for (int i = 0; i < pair_count; i++)
    {
        if (!check_cycle(pairs[i].winner, pairs[i].winner, pairs[i].loser))
            locked[pairs[i].winner][pairs[i].loser] = true;
    }
    return;
}

bool check_cycle(int i, int j, int k)
{
    // i is the constant that will be checked if a cycle completes
    // k and j will work as candidates that can potentially close the loop
    if (i == k || locked[k][i])
        return true;

    else
    {
        // loop iterates through every candidate to check for locked interations later in the chain
        for (int l = 0; l < candidate_count; l++)
        {
            if (locked[k][l])
                if (check_cycle(i, k, l))
                    return true;
        }
    }
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // TODO
    for (int i = 0, j = 0; i < candidate_count; i++)
    {
        for (j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
                break;
        }
        if (!locked[j][i])
        {
            printf("%s\n", candidates[i]);
            break;
        }
    }
    return;
}
