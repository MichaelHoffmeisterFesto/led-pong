#pragma once

// use unsafe strcpy (same functions for Windows/ Linux)
#ifdef WIN32
	#define _CRT_SECURE_NO_WARNINGS
	#pragma warning(disable : 4996)
#endif // WIN32


// Macro determines the size of an arry
#define SIZE_OF_ARR(X) (sizeof(X) / sizeof(X[0]))

// helper function for instance of a type
template<typename Base, typename T>
inline bool instanceof(const T* ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}