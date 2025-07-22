#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define B 512
#define bufferSize 10
typedef uint8_t BYTE;
int main(int argc, char *argv[])
{
    // check for validity
    if (argc != 2)
    {
        printf("Usage: ./recover fileName \n");
        return 1;
    }

    // open file and check if file is correct
    FILE *memoryCard = fopen(argv[1], "r");
    if (memoryCard == NULL)
    {
        printf("Unable to open file.\n");
        return 1;
    }

    // create buffer
    BYTE buffer[B];
    char *fileName;
    fileName = malloc(bufferSize);
    if (fileName == NULL)
        return 1;
    int currentFile = 0;
    int newFileCheck = 0;
    sprintf(fileName, "%03i.jpg", currentFile);
    FILE *image = fopen(fileName, "w");
    if (image == NULL)
        return 1;

    // while loop to read memoryCard
    while (fread(&buffer, 1, B, memoryCard) == B)
    {
        // check for header
        if (buffer[0] == 0xff & buffer[1] == 0xd8 && buffer[2] == 0xff &&
            (buffer[3] & 0xf0) == 0xe0 && currentFile == newFileCheck)
        {
            fwrite(&buffer, 1, B, image);
            currentFile++;
        }
        else if (buffer[0] == 0xff & buffer[1] == 0xd8 && buffer[2] == 0xff &&
                 (buffer[3] & 0xf0) == 0xe0 && currentFile != newFileCheck)
        {
            fclose(image);
            sprintf(fileName, "%03i.jpg", currentFile);
            currentFile++;
            image = fopen(fileName, "w");
            if (image == NULL)
                return 1;
            fwrite(&buffer, 1, B, image);
        }
        else if (currentFile != 0)
            fwrite(&buffer, 1, B, image);
    }
    fclose(memoryCard);
    fclose(image);
    free(fileName);
}
