#
# Bash Reverse Polish Notation (RPN) Calculator
#
# Accepts an arbitrary number of arguments in RPN, evaluates them and prints the answer to stdout.
#
# Based Prof. Harrison's in class example:
# Source: https://opencast-player-1.lt.ucsc.edu:8443/engage/theodul/ui/core.html?id=764d4b0f-c245-4492-a317-4dded677a139
#

input=`echo "$1" | sed 's/\*/x/g'`  # replace all '*' with 'x'
sp=0                                # next empty spot in stack (stack pointer)
stack=()                            # array implementation of stack

# loop through all tokens in the rpn argument
for oper in $input
do
  # if the string contains only digits, push to stack
  if [[ $oper =~ ^[0-9]+$ ]]; then
    stack[$sp]=$oper
  # if the string is an operator
  else
    (( num1_idx = sp - 2 )) # store index of second to top element
    (( num2_idx = sp - 1 )) # store index of top element
    (( sp -= 2))
    num1=${stack[num1_idx]} # store top two numbers ("pop" off stack)
    num2=${stack[num2_idx]}
    if [ "$oper" == '+' ]; then
      (( result = $num1 + $num2 ))  # addition
    elif [ "$oper" == '-' ]; then
      (( result = $num1 - $num2 ))  # subtraction
    elif [ "$oper" == '/' ]; then
      (( result = $num1 / $num2 ))  # divison
    elif [ "$oper" == '^' ]; then
      (( result = $num1 ** $num2 )) # power
    elif [ "$oper" == 'x' ]; then
      (( result = $num1 * $num2 ))  # multiplication
    fi
    stack[$sp]=$result  # store result of calculation at top of stack
  fi
  (( sp += 1 )) # new element, adjust stack pointer
done

echo ${stack[$top]}  # return result at the top of the stack
