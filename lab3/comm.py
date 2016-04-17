#!/usr/bin/python


"""
### comm.py #########

implementation of the `comm` command
    1. support all options and operands required by POSIX
    2. support all environment variables required by POSIX
    3. diagnostics?
    4. Report error if wrong number of operands
    5. Support extra option -u for unsorted input
    6. output lines should appear in same order of input lines
    7. output according to first input file's order
    8. lines that appear only in 2nd file come after other lines
    9. implementation should use locale, string modules of randline.py
    10. change messages to accurately describe modified behavior

"""

############# FUNCTIONALITY: compare sorted files line by line

import random, sys
from optparse import OptionParser


################ START CLASS ################################33
class comm:
    def __init__(self, arg1, arg2, options, parser):
        # check if either file is '-', check this individually
        if arg1 != '-': 
            # first argument is file1
            file1 = arg1
            f1 = open(file1, 'r')
        else:
            # read from stdin and store in f1
            f1 = sys.stdin

        if arg2 != '-':
            # second argument is file2
            file2 = arg2
            f2 = open(file2, 'r')
        else:
            # read from stdin and store in f2
            f2 = sys.stdin

        # read in the lines from both f1, f2
        # intilaize member vars
        self.f1_lines = f1.readlines()
        self.f2_lines = f2.readlines()
        f1.close()
        f2.close()
        self.options = options
        self.parser  = parser

        # accessors
    def getCol1(self):
        return self.f1_lines # lines of first file

    def getCol2(self):
        return self.f2_lines # lines of second file

    ########### member function         ######################
    ########### begin compare function  ###################
    def compare(self):
        # suppress_i is a boolean
        suppress_1 = self.options.no_c1      # true if we want to suppress 1
        suppress_2 = self.options.no_c2      # true if we want to suppress 2
        suppress_3 = self.options.no_c3      # true if we want to suppress 3
        unsorted   = self.options.unsorted   # true if files are unsorted

        f1_lines = self.f1_lines
        f2_lines = self.f2_lines
        # 3 column string each column separated by tabs




        if len(f1_lines) > len(f2_lines):
            f1_longer = True
        else: 
            f1_longer = False

        # iterate through both files until you reach end of shorter file
        # case 1: sorted input files
        # implementation:
        # i is index for f1, j is index for f2
        # while i less than length of f1, j less than length of f2
            # compare first word of both:
                # if f1[i] > f2[i],  add f2[i] to col2, increment j
                # if f1[i] < f2[i],  add f1[i] to col1, increment i
                # if f1[i] == f2[i], add f1[i] to col3, increment both i,j
        i, j = 0, 0
        output = ""
        if not unsorted: ## TO DO: ADD IN SUPPORT FOR OPTIONS
            while i < len(f1_lines) and j < len(f2_lines):
                if f1_lines[i] > f2_lines[j]: # word in f2 smaller, add to col2
                    if not suppress_2:
                        if suppress_1:
                            output += f2_lines[j]
                        else: 
                            output += '\t' + f2_lines[j]
                    j = j + 1
                elif f1_lines[i] < f2_lines[j]:
                    if not suppress_1:
                        output += f1_lines[i]
                    i = i + 1
                else: # word found in both
                    if not suppress_3:
                        if suppress_1 and suppress_2:
                            output += f1_lines[i]
                        elif suppress_1 or suppress_2:
                            output += '\t' + f1_lines[i]
                        else: # nothing suppressed
                            output += '\t\t' + f1_lines[i]
                    i = i + 1
                    j = j + 1

            # output the remainder of the longer file in its respective col
            if f1_longer and i < len(f1_lines) and not suppress_1:
                while i < len(f1_lines):
                    output += f1_lines[i]
                    i = i + 1
            if not f1_longer and j < len(f2_lines) and not suppress_2:
                while j < len(f2_lines):
                    output += '\t' + f2_lines[j]
                    j = j + 1
        else: # -u option passed in
              # unsorted case
              # for each element in f1_lines, look for it in f2_lines
              # if unique, put in column 1
              # if match found in f2_lines, add to col 3 + remove from col 2
              # append remaining lines from f2_lines
            for i in range(len(f1_lines)):
                search = f1_lines[i]
                match = False
                for j in range(len(f2_lines)):
                    if search == f2_lines[j]:
                        # add to col 3, remove from f2_lines
                        match = True
                        if not suppress_3:
                            if suppress_1 and suppress_2:
                                output += search   # add to 1st col
                            elif suppress_1 or suppress_2:
                                output += '\t' + search
                            else: # nothing suppressed, add to 3rd col
                                output += '\t\t' + search
                        # if suppress_3, then dont add anywhere, only remove    
                        f2_lines.remove(search) # remove from f2_lines
                        break # go to next word in file1
                # if no match, then add to column 1 -- only if no match
                if not match and not suppress_1:
                    output += search
            # append remaining lines from f2_lines to column 2
            # all elements of f1_lines have been exhausted
            if not suppress_2:
                for j in range(len(f2_lines)):
                   if not suppress_1:
                        output += '\t' + f2_lines[j]
                   else: # if column 1 is suppressed
                        output += f2_lines[j]
        
        # output result
        sys.stdout.write(output)
    #################   end compare function #############

####################### END CLASS            #######################



############## end compare function ################

#### start main ####

def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE1 FILE2
Compare sorted files FILE1 and FILE2 line by line.

With no options, produce three-column output.

Options:
-1 suppress column 1 (lines unique to FILE1)
-2 suppress column 2 (lines unique to FILE2)
-3 suppress column 3 (lines that appear in both files)
-u indication that FILE1 and FILE2 are unsorted
"""
################## OPTIONS ###########################
    
    parser = OptionParser(version=version_msg, 
                          usage=usage_msg)

    ### with options

    # -1  :  suppress col 1
    # -2  :  suppress col 2
    # -3  :  suppress col 3
    # -u  :  support unsorted file(s) as input
    parser.add_option("-1", 
                      action="store_true", dest="no_c1", default=False,
                      help="suppress column 1 (lines unique to FILE1)")
    parser.add_option("-2",
                       action="store_true", dest="no_c2", default=False,
                       help="suppress column 2 (lines unique to FILE2")
    parser.add_option("-3",
                      action="store_true", dest="no_c3", default=False,
                      help="suppress column 3 (lines that appear in both files")
    parser.add_option("-u",
                      action="store_true", dest="unsorted", default=False,
                      help="Indicate that FILE1 and/or FILE2 are unsorted")
        
################# END OPTIONS ######################

    # extract options and arguments, store into separate lists
    options, args = parser.parse_args(sys.argv[1:])



    ############## begin checks ###############################

    # check to make sure correct number of files given as input
    # write error message if two files are not provided
    if len(args) == 0:
        parser.error("missing operand")
    elif len(args) == 1:
        parser.error("missing operand after {0}".format(args[0]))
    elif len(args) > 2:
        parser.error("extra operand {0}".format(args[2]))

    # pass the following to compare (helper) function
    #  1. arguments (the two files)
    #  2. options, 
    #  3. parser
    try:
        generator = comm(args[0], args[1], options, parser)
    except IOError as err:
        parser.error("I/O error({0}): {1}".format(errno, strerror))

    ####################### end checks #######################

    # all comparison done within member function compare()
    # this generates output
    generator.compare()

################# end main ################


if __name__ == "__main__":
    main()
