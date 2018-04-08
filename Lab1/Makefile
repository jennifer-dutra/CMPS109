#######################################################################
#
# Copyright (C) 2018 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

include Makefile.libs

CFLAGS = -Wall
CC = gcc

SRC=$(wildcard *.c)

rpnc: $(SRC)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS) 2>&1 | tee make.out

check: rpnc
	@echo "" | tee check.out
	@date    | tee -a check.out
	@echo "" | tee -a check.out
	@./check.sh "RpncSH: " "bash rpnc.sh"    | tee -a check.out
	@./check.sh "RpncPY: " "python3 rpnc.py" | tee -a check.out
	@./check.sh "RpncC: " "./rpnc"           | tee -a check.out

grade: clean check
	@./grade.sh 

clean:
	@rm -f rpnc check.out make.out

submit: clean
	@tar czvf ~/CMPS109-Lab1.tar.gz \
	--exclude=Makefile --exclude=grade.sh \
	--exclude=check.sh --exclude=check.dat *
