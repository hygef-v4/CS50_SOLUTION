#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
    // Accept a single command-line argument
    if (argc != 2)
    {
        printf("Usage: ./recover FILE\n");
        return 1;
    }


    // Open the memory card
    FILE *input_file = fopen(argv[1], "r");

     // if file not found
    if(input_file == NULL){
      printf("Could not open file");
      return 2;
    }

    // Create a buffer for a block of data
    unsigned char buffer[512];

    // the counter for number of image
    int count_image = 0;

    // set file pointer to handle JPEGs found
    FILE *output_file = NULL;

    // allocate memory for filename
    char *filename = malloc(8* sizeof(char));

    // 0xff 0xd8 0xff 0xe? & 0xf0 = 1110 0000

    // While there's still data left to read from the memory card
    while (fread(buffer, sizeof(char), 512, input_file))
    {
        // Create JPEGs from the data
        if (buffer[0] == 0xff && buffer[1] == 0xd8 && buffer[2] == 0xff && (buffer[3] & 0xf0) == 0xe0)
        {
          // create file name with 3 digit decimal
          sprintf(filename, "%03i.jpg", count_image);

          // open output_file to write
          output_file = fopen(filename, "w");

          // count # image
          count_image ++;

        }
        // check file not been used, if not -> write new found jpeg
        if(output_file != NULL){
          fwrite(buffer, sizeof(char), 512, output_file);
        }
    }
      if (output_file != NULL)
    {
        fclose(output_file);
    }
    fclose(input_file);
    free(filename);


    return 0;

}
