#include <cstdio>
#include <cstdlib>
#include <chrono>
#include <cstring>
#include <string>

int show_step = 0;
int test_no = 0;
int64_t time_limit = 5000; // in milliseconds.

bool compile(void);
bool generate(void);
bool test(void);

int main(int argc, char** argv) {
	if (compile()) {
		return 0;
	}
	while (true) {
		if (generate() != false) {
			break;
		}
		if (test() != false) {
			break;
		}
	}
	return 0;
}

/**
 * @brief Compile std.cpp, myans.cpp and gen.cpp.
 * 
 * @return true when at least one compilation failed.
 * @return false when all files were compiled successfully.
 */
bool compile(void) {
	std::puts("Compiling std.cpp...");
	if (std::system("g++ std.cpp -o std.exe -std=c++14 -Wall -O2 -Wl,-stack=512000000")) {
		std::printf("CE #%d: std.cpp compilation failed.", test_no);
		return true;
	}
	
	std::puts("Compiling myans.cpp...");
	if (std::system("g++ myans.cpp -o myans.exe -std=c++14 -Wall -O2 -Wl,-stack=512000000")) {
		std::printf("CE #%d: myans.cpp compilation failed.", test_no);
		return true;
	}
	
	std::puts("Compiling gen.cpp...");
	if (std::system("g++ gen.cpp -o gen.exe -std=c++14 -Wall -O2 -Wl,-stack=512000000")) {
		std::printf("CE #%d: gen.cpp compilation failed.", test_no);
		return true;
	}
	return false;
}

/**
 * @brief Generating data.in, data.out(empty) data.ans.
 * 
 * @return true when any file generation failed(return non-zero value).
 * @return false when all files were generated successfully.
 */
bool generate(void) {
	if (show_step) {
		std::printf("Generating data #%d...\n", test_no);
	}
	int ret = 0;
	if ((ret = std::system("cd . > test.out"))) {
		std::printf("DF #%d: Command cd returned a non-zero value %d.\n", test_no, ret);
		return true;
	}
	if ((ret = std::system("gen.exe > data.in"))) {
		std::printf("DF #%d: gen.exe returned a non-zero value %d.\n", test_no, ret);
		return true;
	}
	if ((ret = std::system("std.exe < data.in > data.ans"))) {
		std::printf("DF #%d: std.exe returned a non-zero value %d.\n", test_no, ret);
		return true;
	}
	return false;
}

/**
 * @brief Test the data with executable file.
 * 
 * @return true when RE(return non-zero value), TLE or WA.
 * @return false when AC.
 */
bool test(void) {
	test_no++;
	if (show_step) {
		std::printf("Running data #%d...\n", test_no);
	}
	auto start = std::chrono::steady_clock::now();
	int ret = std::system("myans.exe < data.in > data.out");
	auto end = std::chrono::steady_clock::now();
	auto time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
	
	if (time > time_limit) {
		std::printf("TLE #%d: Executable used %lld milliseconds, exceeding the %lld milliseconds limit.\n", test_no,
		  static_cast<long long>(time), static_cast<long long>(time_limit));
		return true;
	}
	if (ret != 0) {
		std::printf("RE #%d: Executable returned a non-zero value %d.\n", test_no, ret);
		return true;
	}
	if (std::system("fc data.out data.ans > tmp")) {
		std::printf("WA #%d: Difference between data.out and data.ans detected.\n", test_no);
		std::printf("Output of command fc:\n");
		if (std::system("type tmp")) {
			std::printf("Cannot show output of command fc. Please check test.out and test.ans.\n");
		}
		return true;
	}
	
	std::printf("AC #%d.\n", test_no);
	return false;
}
