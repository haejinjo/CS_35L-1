// tr2b.c


#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// uses getchar and puchar to transliterate bytes
// implementation using library functions
/*
void translate(char* from, char* to)
{
    int in_char = getchar();    // reads input from stdin
    int match = 0;              // check for matching byte in 'from'
    int i;
    // iterate through the input and look for matches in 'from'
    while (in_char != EOF)
    {
        for (i = 0; i < strlen(from); i++)
        {
            if (in_char == (unsigned char)from[i])
            {
                match = 1;
                putchar(to[i]); // translate the matched byte to byte in 'to'
                break;
            }
        }
        if (match == 0)
        {
            putchar(in_char); // no match => output the original byte read in
        }
        // regardless of match/no-match, read in next byte for next comparison
        in_char = getchar();
        match = 0; // reset match to 0
    }
}
*/


int main(int argc, char *argv[])
{
    // check for the correct # of args
    if (argc != 3) // the command, followed by 
    {
        fprintf(stderr, "Incorrect number of arguments. 2 arguments needed.\n");
        exit(1);
    }

    char* from = argv[1]; // original string
    char* to   = argv[2]; // translated string

    // check that 'from' and 'to' are same length
    if (strlen(from) != strlen(to))
    {
        fprintf(stderr, "Arguments must be the same length. \n");
        exit(1);
    }

    // check that 'from' has no duplicate bytes
    int i, j;
    char check_byte;
    for (i = 0; i < strlen(from); i++)
    {
        check_byte = from[i];
        for (j = i + 1; j < strlen(from); j++)
        {
            if (check_byte == from[j]) // duplicate byte
            {
                fprintf(stderr,
                        "The 'from' argument cannot have duplicate bytes.\n");
                exit(1);
            }
        }
    }

    // translate bytes
    
    int in_char = getchar();    // reads input from stdin
    int match = 0;              // check for matching byte in 'from'
    // iterate through the input and look for matches in 'from'
    while (in_char != EOF)
    {
        for (i = 0; i < strlen(from); i++)
        {
            if (in_char == (unsigned char)from[i])
            {
                match = 1;
                putchar(to[i]); // translate the matched byte to byte in 'to'
                break;
            }
        }
        if (match == 0)
    	  {
            putchar(in_char); // no match => output the original byte read in
        }
        // regardless of match/no-match, read in next byte for next comparison
        in_char = getchar();
        match = 0; // reset match to 0
    }
    

    return 0;
}





