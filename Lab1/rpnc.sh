#
# Bash Reverse Polish Notation (RPN) Calculator
#
# Accepts an arbitrary number of arguments in RPN, evaluates them and prints the answer to stdout.
#

# echo $1

stack=()
top=0     # next empty spot in stack

for oper in $1
do
  if [[ $oper =~ ^[0-9]+$ ]]; then
    # echo "push: $oper, "
    stack[$top]=$oper
    (( top += 1 ))
  elif [ "$oper" == '+' ]; then
    # echo "plus"
    (( num1_idx = top - 2 ))
    (( num2_idx = top - 1 ))
    (( top -= 2))
    num1=${stack[num1_idx]}
    num2=${stack[num2_idx]}
    (( result = num1 + num2 ))
    (( top += 1 ))
    # echo "pop: $num1, "
    # echo "pop: $num2, "

  fi
  # echo $oper
done

echo $result
