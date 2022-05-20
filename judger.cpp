#include <cstdio>
#include <cstdlib>
#include <chrono>

int main(int argc, char** argv) {
  std::system("g++ std.cpp -o std.exe -std=c++14 -Wall -O2 -fno-ms-extensions -Wl,-stack=512000000");
  std::system("g++ gen.cpp -o gen.exe -std=c++14 -Wall -O2 -fno-ms-extensions -Wl,-stack=512000000");
  std::system("g++ myans.cpp -o myans.exe -std=c++14 -Wall -O2 -fno-ms-extensions -Wl,-stack=512000000");
	int i = 1;
	bool tmp = argc > 1;
	while (1) {
		if (tmp) {
			std::printf("Generating input #%d...\n", i);
		}
		std::system("gen > data.in");
		if (tmp) {
			std::printf("Generating answer #%d...\n", i);
		}
		std::system("std < data.in > data.ans");
		if (tmp) {
			std::printf("Running output #%d...\n", i);
		}
		auto start = std::chrono::system_clock::now();
		std::system("myans < data.in > data.out");
		auto end = std::chrono::system_clock::now();
		int time = static_cast<int>(std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count());
		if (time >= 5000) {
			std::printf("Time Limit (5s) Exceeded on test #%d with %d ms!", i, time);
			return 0;
		}
		if (std::system("fc data.out data.ans")) {
			std::printf("Wrong Answer on test #%d with %d ms!\n", i, time);
			return 0;
		}
		std::printf("Accepted on test #%d with %d ms.\n", i, time);
		++i;
	}
}
