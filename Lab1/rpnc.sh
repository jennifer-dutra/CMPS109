#
# Bash Reverse Polish Notation (RPN) Calculator
#
# Accepts an arbitrary number of arguments in RPN, evaluates them and prints the answer to stdout.
#

stack=()
top=0     # next empty spot in stack

for oper in $1
do
  if [[ $oper =~ ^[0-9]+$ ]]; then
    # echo "push: $oper, "
    stack[$top]=$oper
  else
    (( num1_idx = top - 2 ))
    (( num2_idx = top - 1 ))
    (( top -= 2))
    num1=${stack[num1_idx]}
    num2=${stack[num2_idx]}
    # echo "pop: $num2, "
    # echo "pop: $num1, "
    if [ "$oper" == '+' ]; then
      (( result = $num1 + $num2 ))
    elif [ "$oper" == '-' ]; then
      (( result = $num1 - $num2 ))
    elif [ "$oper" == '/' ]; then
      (( result = $num1 / $num2 ))
    elif [ "$oper" == '^' ]; then
      (( result = $num1 ** $num2 ))
    fi
    stack[$top]=$result
    # echo "push: $result, "
  fi
  (( top += 1 ))
  # echo $oper
done

echo ${stack[0]}
