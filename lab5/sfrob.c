#include <stdio.h>
#include <stdlib.h>


int frobcmp (const char* a, const char* b); 


/*
    a, b point to array of non-space bytes followed by space byte
    returns an int result:
        -1 : a  <   b
         0 : a  ==  b
         1 : a  >   b
   both arrays are frobnicated
   if one unfrobnicated array is a prefix of the other, then shorter is
   longer than the longer
*/ 
int frobcmp (char const* a, char const* b)
{
    // iterate through both arrays: a,b
    // if one is shorter than the other, then it is "<" and return number
    while ((*a != 32) && (*b != 32)) // check for space character
    {
        // XOR each letter in the arrays with 42 and then compare char by char
        if ( (*a^42) < (*b^42) )          // case: a < b
        {
            return -1;
        }
        else if ( (*a^42) > (*b^42))      // case: a > b
        {
            return 1;
        }

        a++;
        b++;
    }

    // if no return at this point, then a or b has hit a space (prefix)
    // and/or both have hit a space and a == b
    if ( (*a == 32) && (*b != 32))        // case: a is shorter than b (a < b)
        return -1;
    else if ( (*a != 32) && (*b == 32) ) // case: b is shorter than a (a > b)
        return 1;
    else                                 // case: same length (a == b)
        return 0;
}

int main()
{
    int init_allocate = 10; // initial allocation of memory
    char* arr_input = (char*)malloc(sizeof(char)*init_allocate);

    // report error and exit if fail to allocate memory
    if (arr_input == NULL)
    {
        fprintf(stderr, "Memory allocation failed.");
        exit(1);

    }

    // input is an encoded file
    // compute output by decoding input, sorting decoded data
    // encoding resulting output

    // get initial input character
    char char_input = getchar();
    int i = 0; // index for array
    if(char_input == EOF) // hit EOF right away
    {
        exit(1);
    }
    // iterate through input until you hit EOF
    while (char_input != EOF)
    {   
        // check if we need to reallocate more memory
        if (i == init_allocate)
        {
            init_allocate = 2 * init_allocate; // double memory allocation
            char* arr_input = (char*)realloc(arr_input, init_allocate);

            // check for memory allocation failure again
            if (arr_input == NULL)
            {
                fprintf(stderr, "Memory re-allocation failed.");
                exit(1);
            }
        }

        arr_input[i] = char_input; // store the char input
        i++;
    }

    // Once we've hit the end of file, add a space to the end to match the 
    // format of a frobnicated text line
    // check for sufficient memory, if not allocate more for the space

    if (i == init_allocate)
    {
        init_allocate += 1; // only need one more for space
        char* arr_input =  (char*)realloc(arr_input, init_allocate);

        // check for failure
        if (arr_input == NULL)
        {
            fprintf(stderr, "Memory re-allocation failed.");
            exit(1);
        }
    }
    if (arr_input[i-1] != 32)
    {
        arr_input[i] = ' '; // append space character at the end of file
    }

    printf("hello");



    return 0;

}

