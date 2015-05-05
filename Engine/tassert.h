#pragma once

#ifdef TALGA_ASSERTIONS_ENABLED
#include <cassert>

#define TALGA_ASSERT(condition, msg) \
	if (!(condition)) \
			{ \
	std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__ << std::endl; \
	std::cerr << " inside " << __FUNCTION__ << std::endl; \
	std::cerr << "Condition: " << #condition << std::endl; \
	std::cerr << msg << std::endl; \
	 abort(); \
	}
#else
#define TALGA_ASSERT(condition, msg)
#endif

#ifdef TALGA_WARNINGS_ENABLED
#include <iostream>
#define TALGA_WARN(expression, message) \
	if (!(expression))	\
		std::cout << "WARNING: " << message << std::endl;
#else
#define TALGA_WARN(condition, msg) {}
#endif

#ifdef TALGA_MESSAGES_ENABLED
#include <iostream>
#define TALGA_MSG(message) \
	std::cout << "MESSAGE: " << message << std::endl;
#else
#define TALGA_MSG(message) {}
#endif