
#include<unordered_map>
#include<cstdarg>
#include<cstdio>

#define TEST_FUNC_NAME(i) tester_at_index_ ## i
#define TEST_VARIABLE_NAME(i) registered_tester_for_ ## i

#define TEST_WITH_IDX(name, idx) \
	void TEST_FUNC_NAME(idx)(Testing& t); \
	bool TEST_VARIABLE_NAME(idx) = register_test(#name, &TEST_FUNC_NAME(idx)); \
	void TEST_FUNC_NAME(idx) 

#define TEST(name) TEST_WITH_IDX(name, __LINE__)

class Testing {
	bool failedToggle;
	std::string failureMessage;

	public:
		Testing() {
			failedToggle = false;
			failureMessage = "Failed";
		}

		void fail() {
			failedToggle = true;
		};


		void fail(std::string message) {
			failedToggle = true;
			failureMessage = message;
		};
		
		void failf(std::string format, ...) {
			failedToggle = true;
			int sz = 100;
			char buf[sz];
			va_list args;
			va_start(args, format);
			std::vsnprintf(buf, sz, format.c_str(), args);
			failureMessage = std::string(buf);
		};

		bool failed() {
			return failedToggle;
		};

		std::string message() {
			return failureMessage;
		};
};

typedef void (*testerfunc)(Testing&);

// Turn back into a vector to get working again.
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
			cerr << x.first << ": " << t.message() << endl;
		}
	}

	if (failed > 0) {
		cerr << failed << "/" << registered_tests.size() << " Failed!"<< endl;
	} else {
		cerr << "All " << registered_tests.size() << " tests passed!" << endl;
	}
}
