mm: main.o
	g++ -o mm main.o
distance.o: distance.cpp distance.hpp
	g++ -std=c++11 -o distance.cpp
Route.o: Route.cpp Route.hpp
	g++ -std=c++11 -o Route.cpp
trajectory_generated.o: trajectory_generated.cpp
	g++ -std=c++11 -o trajectory_generated.o trajectory_generated.cpp 
TR_SIM.o: TR_SIM.cpp TR_SIM.hpp
	g++ -std=c++11 -o TR_SIM.o TR_SIM.cpp
trajectory.o: trajectory.cpp trajectory.h distance.o Route.o trajectory_generated.o \
	TR_SIM.o
	g++ -std=c++11 -o trajectory.o trajectory.cpp trajectory.h distance.o Route.o trajectory_generated.o \
	TR_SIM.o
main.o: main.cpp
	g++ -std=c++11 -o main.o main.cpp 