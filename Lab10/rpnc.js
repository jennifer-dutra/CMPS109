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
  if(!isNaN(token)){
    // console.log('push: ' + token);
    stack.push(Number(token));
  }
  else if (token == '+' || token == '-' || token == '*' || token == '/' || token == '^') {
    var num1 = stack.pop();
    var num2 = stack.pop();
    // console.log('pop: ' + num1);
    // console.log('pop: ' + num2);
    var calc = compute(num1, num2, token);
    stack.push(calc);
    // console.log('push: ' + calc);
  }
 }

 console.log(stack.pop());

 // console.log('Input: ' + input);


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
    return 0;
}
