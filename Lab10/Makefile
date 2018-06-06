#######################################################################
#
# Copyright (C) 2018 David C. Harrison. All right reserved.
#
# You may not use, distribute, publish, or modify this code without 
# the express written permission of the copyright holder.
#
#######################################################################

all: grade

check: rpnc.js
	@echo "" | tee check.out
	@date    | tee -a check.out
	@echo "" | tee -a check.out
	@./check.sh "RpncJS: "  "nodejs rpnc.js"  | tee -a check.out
	@./check.sh "RpncAWK: " "awk -f rpnc.awk" | tee -a check.out

grade: check
	@./grade.sh 

submit:
	@tar czvf ~/CMPS109-Lab10.tar.gz rpnc.js rpnc.awk
