a.out: main.cpp lib/cos.hpp lib/data.hpp
	g++ main.cpp -g -o $@ -I galib -L galib/ga -lga -lm
