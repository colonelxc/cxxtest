# Simple one-file-include test library for c++11.

Though many like this exist, I thought I'd create my own for no reason at all. I use this for writing tests for week+ programming competitions (like a topcoder marathon match).

Let's start with an example.

mytest.cxx:
```
#include<test.h>

TEST(my test description)(Testing& t) {
	int x = 1 + 1;
	if (x != 2) {
		t.failf("math is broken, 1+1 != %d", x)
	}
}
```

There is a macro called TEST, which you give a string of text to identify the test case. It should be unique.

The TEST macro creates a function name for you, so you just need to fill out the rest of the function definition.
Similar to testing in Go, tests are provided with an object that can be used to denote a test has failed.
There are 3 functions to denote a failure, ```fail()```, ```fail("your message")```, ```failf("your message %s", "values")```

Unlike Go tests, this doesn't currently keep track of multiple failure reasons, so it makes sense to return after a failure.

Do not create a main function in your test, one will automagically be provided.
