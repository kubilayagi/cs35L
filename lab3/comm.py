#!/usr/bin/python

import sys, string, locale
from optparse import OptionParser

class comm:
    def __init__(self, args, options):
        if args[0] == "-":
            self.file1 = sys.stdin.read().split('\n')
        else:
            f1 = open(args[0], 'r')
            self.file1 = f1.read().split('\n')
            f1.close()
        if args[1] == "-":
            self.file2 = sys.stdin.read().split('\n')
        else:
            f2 = open(args[1], 'r')
            self.file2 = f2.read().split('\n')
            f2.close()
        del self.file1[len(self.file1)-1]
        del self.file2[len(self.file2)-1]
        '''
        newline = '\n'
        f1line = f2line = 0
        while f1line < len(self.file1):
            if newline not in self.file1[f1line]:
                self.file1[f1line].append(newline)
            f1line += 1

        while f2line < len(self.file2):
            if newline not in self.file2[f2line]:
                self.file2[f2line].append(newline)
            f2line += 1'''
        self.args = args
        self.options = options

    def compare_files(self):
        opt2tabs = 1
        opt3tabs = 2
        if self.options.one:
            opt2tabs = 0
            opt3tabs = 1
        if self.options.two:
            opt3tabs = 1
        if self.options.one and self.options.two:
            opt3tabs = 0
        line1 = line2 = 0

        if self.options.u:
            for line in self.file1:
                if line not in self.file2:
                    if not self.options.one:
                        print(line)
                else:
                    if not self.options.three:
                        print("\t" * opt3tabs + line)
                    self.file2.remove(line)
            if len(self.file2) != 0:
                if not self.options.two:
                    for line2 in self.file2:
                        print("\t" * opt2tabs + line2)
            return
                    
        elif sorted(self.file1) != self.file1 or sorted(self.file2) != self.file2:
            print("files are not sorted")
            return
            
        else:
            #use while loop here so that we can manipulate the counters how we need
            while line1 < len(self.file1) and line2 < len(self.file2):
                if self.file1[line1] != self.file2[line2]:
                    if self.file1[line1] < self.file2[line2]:
                        if not self.options.one:
                            print(self.file1[line1])
                        line1 += 1
                    else:
                        if not self.options.two:
                            print("\t" * opt2tabs + self.file2[line2])
                        line2 += 1
                else:
                    if not self.options.three:
                        print ("\t" * opt3tabs + self.file1[line1])
                    line1 += 1
                    line2 += 1
            if line1 == len(self.file1):
                while line2 < len(self.file2):
                    if not self.options.two:
                        print("\t" * opt2tabs + self.file2[line2])
                    line2 += 1
            if line2 == len(self.file2):
                while line1 < len(self.file1):
                    if not self.options.one:
                        print(self.file1[line1])
                    line1 += 1
            return


def main():
    version_msg = "%prog 2.0"
    usage_msg = """%prog [OPTION]... FILE1 FILE2
Compare the lines of two files.
Replace one (and only one) file with - to read from stdin.
Use the -u option for unsorted files; -1,-2,-3 to silence output from columns.
"""

    parser = OptionParser(version=version_msg, usage=usage_msg)
    parser.add_option("-1", action="store_true", dest="one", default=False, help="Suppress column with lines only in File1")
    parser.add_option("-2", action="store_true", dest="two", default=False, help="Suppress column with lines only in File2")
    parser.add_option("-3", action="store_true", dest="three", default=False, help="Suppress column with common output")
    parser.add_option("-u", action="store_true", dest="u", default=False, help="Compares unsorted files")
    options, args = parser.parse_args(sys.argv[1:])
    
    if len(args) != 2:
        parser.error("you need two arguments")
    if args[0] == "-" and args[1] == "-":
        parser.error("you can't have two files read from stdin")
    
    try:
        generator = comm(args, options)
    except:
        parser.error("error with init")
    try:
        generator.compare_files()
    except:
        parser.error("error with comparison")


if __name__ == "__main__":
    main()
