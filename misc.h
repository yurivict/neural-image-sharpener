#pragma once

#include <iostream>
#include <sstream>

#include <stdio.h>
#include <stdlib.h>

#define STR(msg...) \
	([&]() { \
	  std::ostringstream __ss__; \
	  __ss__ << msg; \
	  return __ss__.str(); \
	}())
#define CSTR(msg...) (STR(msg).c_str())

#define PRINT(msg...) \
	{ \
		std::cout << STR(msg << std::endl); \
	}
#define PRINTe(msg...) \
	{ \
		std::cerr << STR(msg << std::endl); \
	}
#define FAIL(msg...) \
	{ \
		std::cerr << STR("error: " << msg << std::endl); \
		::fflush(stdout); \
		::fflush(stderr); \
		::exit(EXIT_FAILURE); \
	}

