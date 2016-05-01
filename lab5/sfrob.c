#include <stdio.h>
#include <stdlib.h>


int frobcmp (const char *a, const char *b); 


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
int frobcmp (char const *a, char const *b)
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


// separate compare function that is compatible with qsort
int compare(void const *x, void const *y)
{
    return frobcmp( *((char**)x), *((char**)y) );
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
    int numwords = 0; // count the first word we've read in
    while (char_input != EOF)
    {   
        // check if we need to reallocate more memory
        if (i == init_allocate)
        {
            init_allocate = 2 * init_allocate; // double memory allocation
            arr_input = (char*)realloc(arr_input, init_allocate);

            // check for memory allocation failure again
            if (arr_input == NULL)
            {
                fprintf(stderr, "Memory re-allocation failed.");
                exit(1);
            }
            //init_allocate = 2 * init_allocate;
        }

        //arr_input[i] = char_input; // store the char input
        // check to see if we've completed a word
        if (char_input == ' ')
        {
            if (i != 0)
            {
                if (arr_input[i-1] != ' ')
                {
                    numwords++;
                    arr_input[i] = char_input;
                    i++;
                }
            }
        }
        else
        {
            arr_input[i] = char_input;
            i++;
        }
        char_input = getchar();
    }

    // Once we've hit the end of file, add a space to the end to match the 
    // format of a frobnicated text line
    // check for sufficient memory, if not allocate more for the space

    if (i == init_allocate)
    {
        init_allocate += 1; // only need one more for space
        arr_input =  (char*)realloc(arr_input, init_allocate);

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
        numwords++;
    }


    // input error
    if (ferror(stdin))
    {
        fprintf(stderr, "Input Error -- problem when reading from stdin.");
        exit(1);
    }

    // debugging purposes
    //printf("number of words: %d\n", numwords);

    // pointers pointing to words read in 
    // use number of words read in to determine memory allocation
    char** words = (char**)malloc(sizeof(char*)*numwords);

    // check for memory allocation failure
    if (words == NULL)
    {
        fprintf(stderr, "Words pointer -- Memory allocation failed");
        exit(1);
    }

    //printf("Successfully Allocated memory for words pointer. \n");


    // iterate through the arr_input and have words point to beginning
    // of each word
    // start at beginning of arr_input array
    // set first element of words to point to first letter of arr_input
    // increment arr_input until you hit a space, then increment one more
    // set next element of words to point to the start of the stored word

    int j;
    for (j = 0; j < numwords; j++)
    {
        words[j] = arr_input;
        //printf("reached. \n");
        while ((*arr_input) != 32)
        {
            arr_input++; // increment until we find space
        }
        arr_input++; // increment again to bring arr_input to next word
    }

    //printf("Checkpoint: Words pointer points to beginning of each word. \n");

    // sort words using qsort
    // qsort(words, numwords, sizeof(char*), frobcmp);
    // qsort with frobcmp does not work because of 'incompatible pointer type'
    qsort(words, numwords, sizeof(char*), compare);

    // output words
    char* output_words;
    for (j = 0; j < numwords; j++)
    {
        output_words = words[j];
        putchar(*output_words); // in case first letter is space
        // iterate throgh output_words until end of word
        while (*output_words != 32)
        {
            output_words++; // next letter
            putchar(*output_words);
        }

    }


    // output error message
    if (ferror(stdout))
    {
        fprintf(stderr, "Output Error -- putchar failed.");
        exit(1);
    }

    // free up allocated memory
    //free(arr_input); 
    free(words);

    return 0;

}

