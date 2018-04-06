#
# Python3 Reverse Polish Notation (RPN) Calculator
#
# Accepts an arbitrary number of arguments in RPN, evaluates them and prints the answer to stdout.
#

import sys

def main():

    input = sys.argv[1]


    for i in input:
        print(i)
        if i == '+' or i == '-' or i == '*' or i == '/' or i == '^':
            print("operator, ")
        elif i.isdigit():
            print("is digit, ")
        else:
            print("space, ")


if __name__ == "__main__":
    main()
