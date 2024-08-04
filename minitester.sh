#!/bin/bash

export OUI='echo salut $HOME $0'

valgrind_output=$(valgrind --leak-check=full --show-leak-kinds=all --trace-children=yes --track-fds=yes --suppressions=test.supp ./minishell 'echo < salut | cat $HOME/monitoring.sh$SALUT | << bonjour | > oui < non >> salut cat | $OUI|dsfdsfdsfdsfs|"e"ch"o" salut < oui >non >>salut| $?$? | echo "" "" salut' 2>&1 | sed 's/^==[0-9]*== //')

echo "$valgrind_output" > valgrind_output.tmp

if diff valgrind_output.tmp exec_test > /dev/null; then
  echo -e "\033[0;32mtest passed\033[0m"
else
  echo -e "\033[0;31mtest not passed\033[0m"
fi

rm valgrind_output.tmp
