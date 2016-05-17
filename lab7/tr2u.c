// tr2u.c

#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>

// use system calls: read, write to implement the tr command

void translate(char* from, char* to)
{
    char in_char; 
    int  read_status = read(0, &in_char, 1);

    int  match = 0; // 1 if there is a match between input chars and 'from'
    int  i = 0;
    while (read_status > 0)
    {
        for (i = 0; i < strlen(from); i++)
        {
            if (in_char == from[i])
            {
                match = 1;
                in_char = to[i]; // set to translated char
                write(1, &in_char, 1);
                break;
            }
        }
        if (match == 0)
        {
            write(1, &in_char, 1);
        }
        
        read_status = read(0, &in_char, 1);
        match = 0; // reset match to 0
    }

    match = 0;              // check for matching byte in 'from'
}


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
    translate(from, to);

    return 0;
}



