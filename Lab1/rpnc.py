#
# Python3 Reverse Polish Notation (RPN) Calculator
#
# Accepts an arbitrary number of arguments in RPN, evaluates them and prints the answer to stdout.
#

import sys

def main():

    input = sys.argv[1]
    currNum = 0
    stack = []

    for i in input:
        print(i)
        if i.isdigit():
            digit = ord(i) - 48
            # currNum = (currNum * 10) + digit
            stack.append(digit)
            print(stack)

        elif i == '+' or i == '-' or i == '*' or i == '/' or i == '^':
            print("operator %c, " % i)
            num1 = stack[-1]
            stack.pop()
            num2 = stack[-1]
            stack.pop()
            result = operation(num1, num2, i)
            stack.append(result)
            print(stack[-1])

def operation(num1, num2, operator):
    if operator == '+':
        return num2 + num1
    elif operator == '-':
        return num2 - num1
    elif operator == '*':
        return num2 * num1
    elif operator == '/':
        return num2 / num1
    elif operator == '^':
        return pow(num2, num1)

if __name__ == "__main__":
    main()
