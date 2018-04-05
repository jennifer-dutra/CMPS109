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

/*
 * STACK INTERFACE - temporary
 * Author: Robert I. Pitts <rip@cs.bu.edu>
 *
 */

# ifndef _STACK_H
# define _STACK_H

typedef int stackElementT;

typedef struct {
 stackElementT *contents;
 int maxSize;
 int top;
} stackT;

void StackInit(stackT *stackP, int maxSize);

void StackDestroy(stackT *stackP);

void StackPush(stackT *stackP, stackElementT element);

stackElementT StackPop(stackT *stackP);

int StackIsEmpty(stackT *stackP);

int StackIsFull(stackT *stackP);

#endif  /* not defined _STACK_H */

int operate(int num1, int num2, char operation); // completes operation

/*
 * MAIN - RPN calculator
 *
 */

int main(int argc, char *argv[]) {

  char *rpn = argv[1];            // pointer to RPN argument
  stackT stack;                   // declare stack
  StackInit(&stack, strlen(rpn)); // create stack, allocate space
  int sum = 0;                    // sum

  // loop through RPN argument
  for(int i = 0; rpn[i] != '\0'; i++) {
    int currNum = 0; // current number

    // if character is a digit
    while(isdigit(rpn[i])) {
      // convert char to int and add digit to current number
      // must shift the current number by power of 10 (ex. '55' = '5' + '5' = 50 + 5)
      currNum = (currNum * 10) + (rpn[i] - '0');
      // printf("Digit is: %d, ", currNum); // TESTING

      // if the next digit is not a part of the same number, push number to the stack
      if(!isdigit(rpn[i + 1])) {
        // printf("push %d to stack, ", currNum);
        StackPush(&stack, currNum);
        // printf("pushed: %d, ", currNum); // TESTING
      }
      i++;  // iterate to the next character in the string
    }

    // if character is an operation
    if(rpn[i] == '+' || rpn[i] == '-' || rpn[i] == '*' || rpn[i] == '/' || rpn[i] == '^') {
      int num1 = StackPop(&stack);        // pop first number from stack
      int num2 = StackPop(&stack);        // pop second number from stack
      // printf("Popped: %d, ", num1);    // TESTING
      // printf("Popped: %d, ", num2);    // TESTING
      sum = operate(num1, num2, rpn[i]);  // perform desired operation on two integers
      // printf("push %d to stack, ", sum);
      StackPush(&stack, sum);             // update current sum by pushing to the stack
    }


  }

  return sum; // return final sum for the RPN argument given

  return -1; // return -1 if there is there is an error
}


/*
 * OPERATE - when program reaches operator in string, this
 * function is called to peform the correct arithmetic.
 */
int operate(int num1, int num2, char operation) {

  if(operation == '+') {
    return (num2 + num1);
  }
  else if (operation == '-') {
    return (num2 - num1);
  }
  else if (operation == '*') {
    return (num2 * num1);
  }
  else if (operation == '/') {
    return (num2 / num1);
  }
  else if (operation == '^') {
    return (pow(num2, num1));
  }
  else { // CHANGE THIS
    return 0;
  }
}


/*
 * STACK IMPLEMENTATION - temporary
 * Author: Robert I. Pitts <rip@cs.bu.edu>
 *
*/

void StackInit(stackT *stackP, int maxSize)
{
  stackElementT *newContents;

  /* Allocate a new array to hold the contents. */

  newContents = (stackElementT *)malloc(sizeof(stackElementT) * maxSize);

  if (newContents == NULL) {
    fprintf(stderr, "Insufficient memory to initialize stack.\n");
    exit(1);  /* Exit, returning error code. */
  }

  stackP->contents = newContents;
  stackP->maxSize = maxSize;
  stackP->top = -1;  /* I.e., empty */
}

void StackDestroy(stackT *stackP)
{
  /* Get rid of array. */
  free(stackP->contents);

  stackP->contents = NULL;
  stackP->maxSize = 0;
  stackP->top = -1;  /* I.e., empty */
}

void StackPush(stackT *stackP, stackElementT element)
{
  if (StackIsFull(stackP)) {
    fprintf(stderr, "Can't push element on stack: stack is full.\n");
    exit(1);  /* Exit, returning error code. */
  }

  /* Put information in array; update top. */

  stackP->contents[++stackP->top] = element;
}

stackElementT StackPop(stackT *stackP)
{
  if (StackIsEmpty(stackP)) {
    fprintf(stderr, "Can't pop element from stack: stack is empty.\n");
    exit(1);  /* Exit, returning error code. */
  }

  return stackP->contents[stackP->top--];
}

int StackIsEmpty(stackT *stackP)
{
  return stackP->top < 0;
}

int StackIsFull(stackT *stackP)
{
  return stackP->top >= stackP->maxSize - 1;
}
