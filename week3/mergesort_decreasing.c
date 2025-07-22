#include <stdio.h>
#include <string.h>

void merge_sort(int array[], int n);
int main(void)
{
    // get array size
    printf("Size of the array: ");
    int array_size;
    scanf("%i", &array_size);

    // get array
    int array[array_size];
    int merged_array[array_size];
    for (int i = 0; i < array_size; i++)
    {
        printf("Number: ");
        scanf("%i", &array[i]);
    }

     //sort
    merge_sort(array, array_size);
    for (int i = 0; i < array_size; i++)
    {
        printf("%i ", array[i]);
    }
    printf("\n");

}


// Merge sort
void merge_sort(int array[], int n)
{
    if (n == 0 || n == 1)
    {
        return;
    }

    int sub_array_left[n / 2];
    int sub_array_right[(n + 1)/ 2];

    for (int i = 0; i < n / 2; i++)
    {
        sub_array_left[i] = array[i];
    }

    for (int i = 0; i < (n + 1)/ 2; i++)
    {
        sub_array_right[i] = array[(n / 2) + i];
    }

    merge_sort(sub_array_left, n / 2);


    merge_sort(sub_array_right, (n + 1) / 2);


    int j = 0;
    int k = 0;
    int l = 0;
    for (int i = 0; i < n; i++)
    {
        if (k == (n + 1) / 2)
        {
            array[l] = sub_array_left[j];
            l++;
            j++;
        }

        else if (j == n / 2)
        {
            array[l] = sub_array_right[k];
            l++;
            k++;
        }

        else if (sub_array_left[j] < sub_array_right[k])
        {
            array[l] = sub_array_right[k];
            l++;
            k++;
        }

        else if (sub_array_left[j] > sub_array_right[k])
        {
            array[l] = sub_array_left[j];
            l++;
            j++;
        }
    }

}

