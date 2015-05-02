#pragma once

#ifdef TALGA_DEBUG
#include <cassert>
#define TALGA_ASSERT(condition) \
	if (!(condition)) \
	( \
	(std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__), \
	(std::cerr << " inside " << __FUNCTION__ << std::endl), \
	(std::cerr << "Condition: " << #condition), \
	( abort() ) \
	)
#else
#define TALGA_ASSERT(condition)
#endif

#ifdef TALGA_WARNINGS_ENABLED
#include <iostream>
#define TALGA_WARN(expression, message) \
	if (!(expression))	\
		(std::cout << "WARNING: " << message << std::endl)
#else
#define TALGA_WARN(message)
#endif

#ifdef TALGA_MESSAGES_ENABLED
#include <iostream>
#define TALGA_MSG(message) \
	(std::cout << "MESSAGE: " << message << std::endl)
#else
#define TALGA_MSG(message)
#endif