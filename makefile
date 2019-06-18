a.out: main.cpp lib/cos.hpp lib/data.hpp
	g++ main.cpp -o $@ -I galib -L galib/ga -lga -lm
