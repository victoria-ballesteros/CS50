// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

// Definition of a byte (8 bits)
typedef uint8_t BYTE;

// Definition of sample (16 bits)
typedef uint16_t SAMPLE;

// Aux function
int16_t scale_factor(int16_t sample, float factor);

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // TODO: Copy header from input file to output file

    BYTE byte;

    for (int i = 0; i < HEADER_SIZE; i++)
    {
        if (fread(&byte, sizeof(byte), 1, input) == 0)
        {
            return 1;
        }

        fwrite(&byte, sizeof(byte), 1, output);
    }

    // TODO: Read samples from input file and write updated data to output file

    SAMPLE sample;

    while (fread(&sample, sizeof(sample), 1, input) != 0)
    {
        sample = scale_factor(sample, factor);
        fwrite(&sample, sizeof(sample), 1, output);
    }

    // Close files
    fclose(input);
    fclose(output);
}

int16_t scale_factor(int16_t sample, float factor)
{
    int32_t temp = (int32_t) sample * factor;

    if (temp > 32767)
        return 32767;
    if (temp < -32768)
        return -32768;

    return (int16_t) temp;
}
