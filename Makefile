# Names: Kaylee Muckerman and Julian To
# REDID: 130526510 and 130405272

# Makefile for dine seating project

CXX = g++
CXXFLAGS = -std=c++11 -pthread -lrt -Wall -g3 -c 

OBJS = consumer.o log.o producer.o dineseating.o monitor.o

PROGRAM=dineseating

$(PROGRAM) : $(OBJS) 
	$(CXX) -pthread -o $(PROGRAM) $^

log.o: log.h seating.h
	$(CXX) $(CXXFLAGS) log.cpp

producer.o: producer.h
	$(CXX) $(CXXFLAGS) producer.cpp

monitor.o: monitor.h
	$(CXX) $(CXXFLAGS) monitor.cpp

consumer.o: consumer.h
	$(CXX) $(CXXFLAGS) consumer.cpp

dine_seating.o: producer.h consumer.h
	$(CXX) $(CXXFLAGS) dineseating.cpp
			
clean :
	rm -f *.o $(PROGRAM)

