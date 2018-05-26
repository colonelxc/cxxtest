
#include<cstdarg>
#include<cstdio>
#include<iostream>
#include<vector>
#include<unordered_map>


#define TEST_FUNC_NAME(i) tester_at_index_ ## i
#define TEST_VARIABLE_NAME(i) registered_tester_for_ ## i

#define TEST_WITH_IDX(name, idx) \
	void TEST_FUNC_NAME(idx)(Testing& t); \
	bool TEST_VARIABLE_NAME(idx) = register_test(#name, &TEST_FUNC_NAME(idx)); \
	void TEST_FUNC_NAME(idx) 

#define TEST(name) TEST_WITH_IDX(name, __LINE__)

class Testing {
	bool failedToggle;
	std::vector<std::string> failureMessages;

	public:
		Testing() {
			failedToggle = false;
		}

		void fail() {
			failedToggle = true;
			failureMessages.push_back("Failed");
		};


		void fail(const std::string& message) {
			failedToggle = true;
			failureMessages.push_back(message);
		};
		
		void failf(const std::string& format, ...) {
			failedToggle = true;
			int sz = 100;
			char buf[sz];
			va_list args;
			va_start(args, format);
			std::vsnprintf(buf, sz, format.c_str(), args);
			failureMessages.push_back(std::string(buf));
		};

		bool failed() {
			return failedToggle;
		};

		std::vector<std::string> messages() {
			return failureMessages;
		};
};

typedef void (*testerfunc)(Testing&);

std::unordered_map<std::string, testerfunc> registered_tests;
bool register_test(std::string testname, testerfunc tester) {
	registered_tests.insert(std::make_pair(testname, tester));
	return true;
}

int main(int argc, char ** argv) {
	int failed = 0;
	for (std::pair<std::string, testerfunc> x: registered_tests) {
		Testing t;
		x.second(t);

		if (t.failed()) {
			failed += 1;
			for (const std::string& err : t.messages()) {
				std::cerr << x.first << ": " << err << std::endl;
			}
		}
	}

	if (failed > 0) {
		std::cerr << failed << "/" << registered_tests.size() << " Failed!"<< std::endl;
	} else {
		std::cerr << "All " << registered_tests.size() << " tests passed!" << std::endl;
	}
}
