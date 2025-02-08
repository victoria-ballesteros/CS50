#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define SIZE 512

typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        printf("Usage: recover <card>.raw\n");
        return 1;
    }

    FILE *input = fopen(argv[1], "rb");

    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    BYTE block[SIZE]; // No necesitamos calloc
    char name[8];     // Para nombres como "000.jpg", "001.jpg", etc.
    int cont = 0;
    FILE *output = NULL; // Inicializar a NULL

    while (fread(block, sizeof(BYTE), SIZE, input) == SIZE)
    {
        // Verificar si encontramos una firma de inicio de imagen
        if (block[0] == 0xFF && block[1] == 0xD8 && block[2] == 0xFF && (block[3] & 0xF0) == 0xE0)
        {
            printf("Entra");

            // Cerrar el archivo anterior si ya había uno abierto
            if (output != NULL)
            {
                fclose(output);
            }

            // Crear un nuevo nombre de archivo
            sprintf(name, "%03d.jpg", cont);
            output = fopen(name, "wb");

            if (output == NULL)
            {
                printf("Could not open file for writing.\n");
                fclose(input);
                return 1;
            }

            cont++;
        }

        // Si hay un archivo abierto, escribir el bloque
        if (output != NULL)
        {
            fwrite(block, sizeof(BYTE), SIZE, output);
        }
    }

    // Cerrar cualquier archivo restante
    if (output != NULL)
    {
        fclose(output);
    }

    fclose(input);

    return 0;
}