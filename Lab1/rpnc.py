#
# Python3 Reverse Polish Notation (RPN) Calculator
#
# Accepts an arbitrary number of arguments in RPN, evaluates them and prints the answer to stdout.
#

import sys

def main():

    input = sys.argv[1]
    currNum = 0

    for i in input:
        print(i)
        if i.isdigit():
            digit = ord(i) - 48
            currNum = (currNum * 10) + digit
            print("push digit: %d" % digit)


        if i == '+' or i == '-' or i == '*' or i == '/' or i == '^':
            print("operator, ")
            # pop, pop
            # call operation
            # push back updated val
        else:
            print("space, ")

# def operation(num1, num2, operator):
#     if operator == '+':
#         return num2 + num1
#     elif operator == '-':
#         return num2 - num1
#     elif operator == '*':
#         return num2 * num1
#     elif operator == '/':
#         return num2 / num1
#     elif operator == '^':
        # return num2 ^ num1

if __name__ == "__main__":
    main()
