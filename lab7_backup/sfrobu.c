/* 
    sfrobu.c 
 
    rewritten version of sfrob program that uses system calls
    to read standard input and write standard output
*/ 



#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/stat.h>


// keep track of number of times frobcmp is invoked
int frobcmp_count = 0;


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
    frobcmp_count++;
    return frobcmp( *((char**)x), *((char**)y) );
}

int main()
{
    struct stat fileStat;

    if (fstat(0, &fileStat) < 0)
    {
        fprintf(stderr, "Error -- problem when reading from stdin.");
        exit(1);
    }

    char**   words;        // pointers pointing to words read in
    char*    arr_input;    // store the inputted characters
    char     input;        // store the chars being read in
    ssize_t  file_size;

    ssize_t init_alloc = 10;
    if (S_ISREG(fileStat.st_mode))
    {
        if (fileStat.st_size == 0)
        {
            file_size = init_alloc;
            arr_input = (char *)malloc(file_size * sizeof(char));
        }
        else
        {
            file_size = fileStat.st_size;
            arr_input = (char *)malloc(file_size * sizeof(char));
        }
    }
    else
    {
        file_size = init_alloc;
        arr_input = (char *)malloc(file_size * sizeof(char));
    }
    
    long i = 0;
    long word_count = 0;
    ssize_t read_status;
    read_status =  read(0, &input, 1);
    if (read_status < 0)
    {
        fprintf(stderr, "Error while reading file.");
        exit(1);
    }
    while (read_status > 0) // read byte-by-byte
    {
        if (input == ' ')
        {
            if (i != 0)
            {
                if (arr_input[i - 1] != ' ')
                {
                    word_count++;
                    arr_input[i] = input;
                    i++;
                }
            }
        }
        else
        {
            arr_input[i] = input;
            i++;
        }
        // check for sufficient memory allocation
        if (i == file_size)
        {
            file_size *= 2;
            arr_input = (char *)realloc(arr_input, 
                                        file_size * sizeof(char));
            if (arr_input == NULL)
            {
                fprintf(stderr, "Memory re-allocation failed.");
                exit(1);
            }
        }

        read_status = read(0, &input, 1);
    }

    // append space char at the end of file
    if (arr_input[i-1] != 32)
    {
        arr_input[i] = ' ';
        word_count++;
    }

    words = (char**)malloc(sizeof(char*) * word_count);
    // check for memory allocation failure
    if (words == NULL)
    {
        fprintf(stderr, "Words pointer -- Memory allocation failure");
        exit(1);
    }

    long j;
    char* arr_input_initial = arr_input;
    for (j = 0; j < word_count; j++)
    {
        words[j] = arr_input;
        if (j+1 == word_count) // if last word then don't increment
        {
            break;
        }
        while ((*arr_input) != 32)
        {
            arr_input++; // increment until we find space
        }
        arr_input++; // increment again to bring arr_input to next word
    }

    qsort(words, word_count, sizeof(char*), compare);

    char* output_words;
    for (j = 0; j < word_count; j++)
    {
        output_words = words[j];
        //putchar(*output_words);
        if (write(1, output_words, 1) == 0)
        {
            fprintf(stderr, "Error -- Problem when writing output.");
            exit(1);
        }
        // iterate throgh output_words until end of word
        while (*output_words != 32)
        {
            output_words++; // next letter
            //putchar(*output_words);
            if (write(1, output_words, 1) == 0)
            {
                fprintf(stderr, "Error -- Problem when writing output.");
                exit(1);
            }
        }

    }

    // added implementation of number of comparisons:
    fprintf(stderr, "Comparisons: %i\n", frobcmp_count);
    free(words);
    free(arr_input_initial);

    return 0;

}









