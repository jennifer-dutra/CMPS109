#
# Awk Reverse Polish Notation (RPN) Calculator
#
# Accepts an single argument in RPN, evaluates it and prints the answer to stdout.
#
BEGIN {
    input = ARGV[1]
    # print input
    tokens = split(input, array, " ")
    top = 0;

    for (i = 1; i <= tokens; i++) {

      if ( array[i]~/^[0-9]*$/ ) {
       stack[top] = array[i]
      }
      else {
        num1 = stack[top - 2]
        num2 = stack[top - 1]
        top -= 2

        if (array[i] == "+")
          result = num1 + num2
        else if (array[i] == "-")
          result = num1 - num2
        else if (array[i] == "*")
          result = num1 * num2
        else if (array[i] == "/")
          result = num1 / num2
        else if (array[i] == "^")
          result = num1 ^ num2

        stack[top] = result
      }
      top++
    }
    print stack[top - 1]
}
