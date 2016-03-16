#
# p1.cpp
#-------------------------------
# Author : Sai Vempali
# UFID   : 16141381
# email  : vishnu24@ufl.edu
#-------------------------------


all: p1

p1 : p1.cpp
	g++ -std=c++0x -o p1 p1.cpp 
	@echo Compilation Successful!

cl: 
	rm -f *.o p1
	@echo All programs removed. Compile again.
	
