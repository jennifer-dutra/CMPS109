#
# Python3 Reverse Polish Notation (RPN) Calculator
#
# Accepts an arbitrary number of arguments in RPN, evaluates them and prints the answer to stdout.
#

import sys

def main():

    input = sys.argv[1]         # user input string
    tokens = input.split()      # split input by whitespace
    stack = []                  # initialize stack

    for i in tokens:
        # if token is a digit, add to stack
        if i.isdigit():
            stack.append(int(i))
        # if token is an operator, pop two values off stack and perform calculation
        elif i == '+' or i == '-' or i == '*' or i == '/' or i == '^':
            num1 = stack[-1]    # get val at top of stack before pop
            stack.pop()
            num2 = stack[-1]
            stack.pop()
            stack.append(operation(num1, num2, i))  # push updated result
    print(stack[-1])    # return final result

# performs calculation
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
