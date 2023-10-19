remove: execute
	rm cle
	rm increasingCle
	rm movingCle
execute: compile
	./cle
	./increasingCle
	./movingCle
compile:
	g++ -std=c++11 -I src/include -L src/lib Circle/main.cpp -o cle -lSDL2
	g++ -std=c++11 -I src/include -L src/lib IncreasingCircle/main.cpp -o increasingCle -lSDL2
	g++ -std=c++11 -I src/include -L src/lib MovingCircle/main.cpp -o movingCle -lSDL2