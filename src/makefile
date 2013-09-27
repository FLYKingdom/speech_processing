all:  train.o header.o validation.o mean.o silence.o silence-functions.o 
	g++ -Wall  -g train.o header.o validation.o mean.o silence.o silence-functions.o -o client -lm
#	rm *.o

train.o:train.cpp
	g++ -Wall  -g -c train.cpp

header.o:../lib/header.cpp
	g++ -Wall  -g -c ../lib/header.cpp

validation.o:../lib/validation.cpp
	g++ -Wall  -g -c ../lib/validation.cpp

mean.o:../lib/mean.cpp
	g++ -Wall  -g -c ../lib/mean.cpp

silence.o:../lib/silence.cpp
	g++ -Wall  -g -c ../lib/silence.cpp

silence-functions.o:../lib/silence-functions.cpp
	g++ -Wall  -g -c ../lib/silence-functions.cpp

feature_calculation.o:../lib/feature_calculation.cpp
	g++ -Wall  -g -c ../lib/feature_calculation.cpp

clean:
	rm *.o
	rm *.txt
