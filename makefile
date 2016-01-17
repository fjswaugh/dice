dice : main.cpp
	g++ -std=c++11 main.cpp -o dice

windows : main.cpp
	x86_64-w64-mingw32-g++ -std=c++11 main.cpp -o dicewin64.exe -static
