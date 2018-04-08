# include <stdio.h>
# include <ctype.h>
# include <stdlib.h>
# include <string.h>
# include <math.h>

/*
 * Reverse Polish Notation (RPN) Calculator
 *
 * Accepts ARGC arguments in RPN, evaluates them and prints the answer to stdout.
 *
 * Returns -1 on error, 0 otherwise.
 */

int main(int argc, char *argv[]) {
  char *rpn = argv[1];                   // pointer to RPN argument
  int stack[strlen(rpn)];                // array based implementation of stack
  int top = 0;                           // stack pointer

  /* Parse RPN string character by character until end of string is reached. */
  for(int i = 0; rpn[i] != '\0'; i++) {
    int currNum = 0; // current number

    /* if character is a digit */
    while(isdigit(rpn[i])) {
      /* Convert char to int and add digit to current number.
         Must shift the current number by power of 10 (ex. '55' = '5' + '5' = 50 + 5) */
      currNum = (currNum * 10) + (rpn[i] - '0');

      /* If the next digit is not a part of the same number, push number to the stack */
      if(!isdigit(rpn[i + 1])) {
        stack[top] = currNum;
        top++;
      }
      i++;  // iterate to the next character in the string
    }

    /* if character is an operation */
    if(rpn[i] == '+' || rpn[i] == '-' || rpn[i] == '*' || rpn[i] == '/' || rpn[i] == '^') {
      int num1 = stack[top - 2];  // get second to top element of stack ("pop")
      int num2 = stack[top - 1];  // get top element of stack ("pop")
      top -= 2;

      /* Perform operaton on elements popped off stack. Push updated sum. */
      if(rpn[i] == '+') {
        stack[top] = num1 + num2;
      }
      else if (rpn[i] == '-') {
        stack[top] = num1 - num2;
      }
      else if (rpn[i] == '*') {
        stack[top] = num1 * num2;
      }
      else if (rpn[i] == '/') {
        stack[top] = num1 / num2;
      }
      else if (rpn[i] == '^') {
        stack[top] = pow(num1, num2);
      }
      top++;
    }
  }
  printf("%d", stack[0]); // print final result
  return 0;
}
