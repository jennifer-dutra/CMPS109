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
 * Array based implementation of stack
 * Referenced:
 * https://www.cs.bu.edu/teaching/c/stack/array/
 */

typedef int stackElement;

typedef struct {
 stackElement *array;   // pointer to contents of the array
 int capacity;          // max size of array
 int top;               // represents current size of stack
} stack;

void createStack(stack *sp, int capacity);        // make stack, allocate memory

void deleteStack(stack *sp);                      // delete stack, free memory

void push(stack *sp, stackElement num);           // push number to stack

stackElement pop(stack *sp);                      // pop top element off stack

int operate(int num1, int num2, char operation);  // completes operation

/*
 * MAIN - RPN calculator
 */

int main(int argc, char *argv[]) {
  char *rpn = argv[1];                  // pointer to RPN argument
  stack rpnStack;                       // declare stack
  createStack(&rpnStack, strlen(rpn));  // create stack, allocate space for all nums
  int sum = 0;                          // current sum to push to stack
  int result = 0;                       // final calculation to return

  /* Parse RPN string character by character until end of string is reached.
  Evaluates RPN during loop and returns final sum (at the top of the stack) */
  for(int i = 0; rpn[i] != '\0'; i++) {
    int currNum = 0; // current number

    /* if character is a digit */
    while(isdigit(rpn[i])) {
      /* Convert char to int and add digit to current number.
         Must shift the current number by power of 10 (ex. '55' = '5' + '5' = 50 + 5) */
      currNum = (currNum * 10) + (rpn[i] - '0');

      /* If the next digit is not a part of the same number, push number to the stack */
      if(!isdigit(rpn[i + 1])) {
        push(&rpnStack, currNum);
      }
      i++;  // iterate to the next character in the string
    }

    /* if character is an operation */
    if(rpn[i] == '+' || rpn[i] == '-' || rpn[i] == '*' || rpn[i] == '/' || rpn[i] == '^') {
      int num1 = pop(&rpnStack);             // pop first number from stack
      int num2 = pop(&rpnStack);             // pop second number from stack
      sum = operate(num1, num2, rpn[i]);     // perform desired operation on two integers
      push(&rpnStack, sum);                  // update current sum by pushing to the stack
    }
  }

  /* Final sum is at the top of stack */
  result = pop(&rpnStack);
  printf("%d", result);
  deleteStack(&rpnStack);
  return 0;

  return -1; // return -1 if there is there is an error
  }


/*
 * OPERATE - when program reaches operator in string, this
 * function is called to peform the correct arithmetic.
 * Handles addition, subtraction, multiplication, division, and powers.
 *
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
  else { // CHANGE THIS??
    return 0;
  }
}


/*
 * Stack Implementation
 * Referenced: https://www.cs.bu.edu/teaching/c/stack/array
 *
*/

/* Creates a stack and allocates memory accordingly, updates pointers */
void createStack(stack *sp, int capacity)
{
  /* Create stack pointer and allocate memory */
  stackElement *newarray;
  newarray = (stackElement *)malloc(sizeof(stackElement) * capacity);
  sp->array = newarray;     // set new stack pointer
  sp->capacity = capacity;  // set capacity
  sp->top = -1;             // the array is empty so top = -1
}

/* Delete stack and free associated memory */
void deleteStack(stack *sp)
{
  free(sp->array);
  sp->array = NULL;
  sp->capacity = 0;
  sp->top = -1;
}

/* Push element to the stack */
void push(stack *sp, stackElement element)
{
  sp->array[++sp->top] = element;
}

/* Pop element off the stack */
stackElement pop(stack *sp) {
  return sp->array[sp->top--];
}
