/*
 * Node.js / JavaScript Reverse Polish Notation (RPN) Calculator
 *
 * Accepts an single argument in RPN, evaluates it and prints the answer to stdout.
 *
 */

 var input = process.argv[2];     // get rpn expression
 var tokens = input.split(" ");   // split on whitespace
 var stack = [];                  // initialize empty stack

 for(var i = 0; i < tokens.length - 1; i++) {
  var token = tokens[i];
  // if it's a number, push to the stack
  if(!isNaN(token)){
    stack.push(Number(token));
  }
  // if it's an operator, pop two numbers off the stack and compute
  else if (token == '+' || token == '-' || token == '*' || token == '/' || token == '^') {
    var num1 = stack.pop();
    var num2 = stack.pop();
    var calc = compute(num1, num2, token);  // push computed val back to stack
    stack.push(calc);
  }
 }
 console.log(stack.pop());

// all arithmetic done in compute function
function compute(num1, num2, operator) {
  if (operator == '+')
    return num2 + num1;
  else if (operator == '-')
    return num2 - num1;
  else if (operator == '*')
    return num2 * num1;
  else if (operator == '/')
    return num2 / num1;
  else if (operator == '^')
    return Math.pow(num2, num1);
}
