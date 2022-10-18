
#ifndef TEST_H
#define TEST_H


// Test is a base class for all tests.  It provides a command interface for
// running tests (run) as well as a data member for recording the name of 
// the test.
//
// Tests are constructed using the TEST macro.  TEST creates a subclass of
// Test and static instance of that subclass.  If you look at the constructor
// for the Test class, you'll notice that it registers the created object 
// with a global TestRegistry.  These features combine to make test creation
// particularly easy.

#include <vector>
#include <string>
#include <math.h>


class TestSetup
{
public:
    virtual void setup() = 0;
    virtual void teardown() = 0;
};



class TestResult;


class Test : public TestSetup
{
public:
	Test (const std::string& testName);

	virtual void	run (TestResult& result);
	virtual void	runTest (TestResult& result) = 0;

protected:
	std::string		name;

};



#define TEST(name,classUnderTest)\
	class classUnderTest##name##Test : public Test\
	{ \
		public: \
			classUnderTest##name##Test () : Test (#name "Test") {} \
            void setup() {}; \
            void teardown() {}; \
			void runTest (TestResult& result_); \
	} classUnderTest##name##Instance; \
	void classUnderTest##name##Test::runTest (TestResult& result_) \


#define TESTWITHSETUP(name,classUnderTest)\
	class classUnderTest##name##Test : public Test, name##Setup\
	{ \
		public: \
			classUnderTest##name##Test () : Test (#name "Test") {} \
            void setup() {name##Setup::setup();} \
            void teardown() {name##Setup::teardown();} \
			void runTest (TestResult& result_); \
	} classUnderTest##name##Instance; \
	void classUnderTest##name##Test::runTest (TestResult& result_) \



// Here is a collection of testing macros that can be used in the 
// bodies of tests.  CHECK tests a boolean expression and records
// a failure if the expression evaluates to false.  CHECK_LONGS_EQUAL
// and CHECK_DOUBLES_EQUAL compare longs and doubles respectively.
//
// To make this an industrial strength test harness, you should
// add equals macros for various low level types as you develop them.
// If, for instance, you have a daterange class, the ability to compare
// them directly and print out their values in the test output is 
// invaluable.



#ifndef DONT_CATCH_EXCEPTIONS


#define CHECK(condition) \
    try { \
    if (!(condition)) \
       result_.addFailure (Failure (#condition, name, __FILE__, __LINE__)); \
    } catch(...) { \
        result_.addFailure (Failure ("Unhandled exception", name, __FILE__, __LINE__)); \
    }

#define CHECK_LONGS_EQUAL(expected,actual)\
{\
    try { \
        long _expected = (expected);\
        long _actual = (actual);\
        if (_expected != _actual) {\
            char message [80];\
            sprintf (message, "expected %ld but was: %ld", _expected, _actual);\
            result_.addFailure (Failure (message, name, __FILE__, __LINE__));\
        }\
    } catch(...) { \
    result_.addFailure (Failure ("Unhandled exception", name, __FILE__, __LINE__)); \
    }\
}


#define CHECK_DOUBLES_EQUAL(expected,actual)\
{\
    try { \
        double _expected = (expected);\
        double _actual = (actual);\
        if (fabs ((_expected)-(_actual)) > 0.001) {\
            char message [80];\
            sprintf (message, "expected %lf but was: %lf", (_expected), (_actual));\
            result_.addFailure (Failure (message, name, __FILE__, __LINE__));\
        }\
    } catch(...) { \
    result_.addFailure (Failure ("Unhandled exception", name, __FILE__, __LINE__)); \
    }\
}


#define CHECK_POINTS_EQUAL(expected,actual)\
{\
    try { \
        Point3d _expected = (expected); \
        Point3d _actual = (actual); \
        if (!_actual.Equals(_expected)) { \
            char message [256];\
            sprintf (message, "expected point (%f, %f, %f) but was: (%f, %f, %f)", \
                 _expected.x, _expected.y, _expected.z,\
                 _actual.x, _actual.y, _actual.z); \
            result_.addFailure (Failure (message, name, __FILE__, __LINE__));\
        }\
    } catch(...) { \
    result_.addFailure (Failure ("Unhandled exception", name, __FILE__, __LINE__)); \
    }\
}


#define CHECK_STRINGS_EQUAL(expected,actual)\
{\
    try { \
        std::string _expected(expected);\
        std::string _actual(actual);\
        if (_expected != _actual) {\
            std::string msg = std::string("expected '") + _expected + \
                              std::string("' but was: '") + _actual + "'"; \
            result_.addFailure (Failure (msg.c_str(), name, __FILE__, __LINE__));\
        }\
    } catch(...) { \
        result_.addFailure (Failure ("Unhandled exception", name, __FILE__, __LINE__)); \
    }\
}




#else


#define CHECK(condition) \
	if (!(condition)) \
		result_.addFailure (Failure (#condition, name, __FILE__, __LINE__));

#define CHECK_LONGS_EQUAL(expected,actual)\
{\
	long _expected = (expected);\
	long _actual = (actual);\
	if (_expected != _actual) {\
		char message [80];\
		sprintf (message, "expected %ld but was: %ld", _expected, _actual);\
		result_.addFailure (Failure (message, name, __FILE__, __LINE__));\
	}\
}


#define CHECK_DOUBLES_EQUAL(expected,actual)\
{\
	double _expected = (expected);\
	double _actual = (actual);\
	if (fabs ((_expected)-(_actual)) > 0.001) {\
		char message [80];\
		sprintf (message, "expected %lf but was: %lf", (_expected), (_actual));\
		result_.addFailure (Failure (message, name, __FILE__, __LINE__));\
	}\
}


#define CHECK_POINTS_EQUAL(expected,actual)\
{\
    Point3d _expected = (expected); \
    Point3d _actual = (actual); \
    if (!_actual.Equals(_expected)) { \
        char message [256];\
        sprintf (message, "expected point (%f, %f, %f) but was: (%f, %f, %f)", \
                _expected.x, _expected.y, _expected.z,\
                _actual.x, _actual.y, _actual.z); \
        result_.addFailure (Failure (message, name, __FILE__, __LINE__));\
    }\
}


#define CHECK_STRINGS_EQUAL(expected,actual)\
{\
    std::string _expected(expected);\
    std::string _actual(actual);\
    if (_expected != _actual) {\
        std::string msg = std::string("expected '") + _expected + \
                            std::string("' but was: '") + _actual + "'"; \
        result_.addFailure (Failure (msg.c_str(), name, __FILE__, __LINE__));\
    }\
}




#endif // DONT_CATCH_EXCEPTIONS


#endif



