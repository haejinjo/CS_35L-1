#include <stdio.h>
#include <string.h>
#include <stdlib.h>
// stdio.h is for the getchar and putchar, while string is for functions involving strings

//----- COMPARISON FUNCTION ------//
// I want to have a function that compares two characters and if they are the same
// I want to modify that character
// EDIT - needed it to break after modifying or else we in trouble
int translate(int* current, const char* from, const char* to)
{
  int i = 0;
  while (i < strlen(from))
    {
      if (*current == (unsigned char)from[i])
	{
	  return (unsigned char)to[i];
	}
      i++;
    }
  return *current;
}

int main(int argc, char* argv[])
{ // Our main function needs to get the arguments of the two sets so we get them using 
  // number of arguments and the arguments themselves
  // For now, I'm just going to loop through our first argument and check whether or not
  // there are duplicates
  // EDIT - Need to exit when error occurs
  if (argc != 3)
    {
      fprintf(stderr, "Requires two sets to translate!\n");
      exit(1);
    }
  if (strlen(argv[1]) != strlen(argv[2]))
    {
      fprintf(stderr, "Sets need to be of the same size!\n");
      exit(1);
    }
  char *from = argv[1];
  char *to = argv[2];
  int it1 = 0;
  int it2 = 0;
  while (it1 < strlen(from))
    {
      it2 = it1 + 1;
      while(it2 < strlen(from))
	{
	  if (from[it2] == from[it1])
	    {
	      fprintf(stderr, "Duplicate bytes in from set!\n");
	      exit(1);
	    }
	  it2 += 1;
	}
      it1 += 1;
    }
  
  // We now want to check stdin for characters we want. 
  // I loop through the entire file with a holder character
  // With that, I call my compare function implemented above and putchar the
  // resulting character
  // Currently calling a lot of system calls!
  int input = 0;
  int lastChar = 0;
  while (lastChar != 1)
    {  
      input = getchar();
      if (input == EOF)
	{
	  lastChar = 1;
	  break;
	}
      int* current = &input;
      // translate(current, from, to);
      putchar(translate(current, from, to));
    }
}

