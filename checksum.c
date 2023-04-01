/*=============================================================================
| Assignment: HW 02 – Calculating the 8, 16, or 32 bit checksum for a
|             given input file
|
| Author: Priyansh Patel
| Language: c
|
| To Compile: gcc checksum.c
| To Execute: ./a.out textfile.txt checksum_size
|             where the files in the command line are in the current directory.
|
|             The text file contains text is mixed case with spaces, punctuation
|             and is terminated by the hexadecimal ‘0A’, inclusive.
|             (The 0x'0A' is included in the checksum calculation.)
|
|             The checksum_size contains digit(s) expressing the checksum size
|             of either 8, 16, or 32 bits
|
|
| Class: CIS3360 - Security in Computing – Spring 2021
| Instructor: McAlpin
| Due Date: 18 April
|
+=============================================================================*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// function declrations
void checksum(char* string, int bits, int count);
void print(char* string, int bits, unsigned long result, int count);
int padding (int bits, int count);

int main(int argc, char const *argv[])
{
  // variables and pointers
  FILE* f = NULL;
  int valid_bits;             // holds the valid bits
  char* input_string = NULL;  // holds input string
  char* c;                    // holds each line from input
  unsigned long result;       // holds checksum result
  int char_count;             // holds number of chars in the input
  int count;                  // count each line (to check)

  // open input file
  f = fopen(argv[1], "r");

  // check if the file opened
  if (f == NULL)
  {
    fprintf(stderr, "File did not open, or file does not exist\n");
    return -1;
  }

  // convert const char* to int
  valid_bits = atoi(argv[2]);

  // check the correct number of bits
  if ((valid_bits != 8 ) && (valid_bits != 16) && (valid_bits != 32))
  {
    fprintf(stderr, "Valid checksum sizes are 8, 16, or 32\n");
    return -1;
  }

  // dynamically allocate memory for input string
  input_string = malloc(sizeof(char) * 2048);
  c = malloc(sizeof(char) * 2048);

  // copy input file data into input_string
  while (fgets(c, 2048, f) != NULL)
  {
    count++;

    if (count == 1)
    {
      strcpy(input_string, c);
    }
    else
    {
      strcat(input_string, c);
    }
  }

  // make adjustments
  char_count = strlen(input_string);
  input_string[char_count] = '\0';

  // close file
  fclose(f);

  // checksum function
  checksum(input_string, valid_bits, char_count);

  // free dynamically allocated memory
  free(input_string);
  free(c);

  return 0;
}

// compute checksum
void checksum(char* string, int bits, int count)
{
  unsigned long result = 0;       // holds the checksum
  int pad;                        // holds the number 'X' to pad

  // compute checksum for given bits
  if (bits == 8)
  {
    // compute checksum for 8 bits
    for (int i = 0; i < count; i++)
    {
      result = result + string[i];
    }

    // mask hex chars for 8 bits
    result = result & 0xff;

    // print result
    print(string, bits, result, count);

    return;
  }
  else if (bits == 16)
  {
    pad = padding(bits, count);

    // pad with 'X'
    if (pad == 1)
    {
      string[count] = 'X';
      count++;
      string[count] = '\0';
    }

    // compute checksum for 16 bits
    for (int i = 0; i < count; i = i + 2)
    {
      result = result + (string[i] << 8);
      result = result + string[i + 1];
    }

    // mask hex chars for 16 bits
    result = result & 0xffff;

    // print result
    print(string, bits, result, count);

    return;
  }
  else if (bits == 32)
  {
    pad = padding(bits, count);

    // pad with number of 'x'
    if (pad != 0)
    {
      if (pad == 1)
      {
        string[count] = 'X';
        count++;
        string[count] = '\0';
      }
      else if (pad == 2 || pad == 3)
      {
        for (int j = 0; j < pad; j++)
        {
          string[count] = 'X';
          count++;
          string[count] = '\0';
        }
      }
    }

    // compute checksum for 32 bits
    for (int i = 0; i < count; i = i + 4)
    {
      result = result + (string[i] << 24);
      result = result + (string[i + 1] << 16);
      result = result + (string[i + 2] << 8);
      result = result + string[i + 3];
    }

    // mask hex chars for 32 bits
    result = result & 0xffffffff;

    // print result
    print(string, bits, result, count);

    return;
  }
}

// print output
void print(char* string, int bits, unsigned long result, int count)
{
  // prints every 80 lines
  for (int i = 0; i < count + 1; i++)
  {
    if ((i % 80) == 0 || string[i] == '\0')
    {
      printf("\n");

      if (string[i] == '\0')
      {
        break;
      }
    }

    printf("%c", string[i]);
  }

  // print checksum with valid bits and character count
  printf("%2d bit checksum is %8lx for all %4d chars\n", bits, result, count);
  return;
}

int padding(int bits, int count)
{
  int pad;        // holds the number 'X' to pad

  // returns number of 'X' to pad
  if (bits == 16)
  {
    return count % 2;       // only need to pad 1 'X'
  }
  else
  {
    pad = count % 4;        // will need to pad with 1, 2, or 3 'X'
    pad = 4 - pad;
    return pad;
  }
}

/*=============================================================================
| I Priyansh Patel (pr348838) affirm that this program is
| entirely my own work and that I have neither developed my code together with
| any another person, nor copied any code from any other person, nor permitted
| my code to be copied or otherwise used by any other person, nor have I
| copied, modified, or otherwise used programs created by others. I acknowledge
| that any violation of the above terms will be treated as academic dishonesty.
+=============================================================================*/
