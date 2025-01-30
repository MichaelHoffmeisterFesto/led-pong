#pragma once

// Macro determines the size of an arry
#define SIZE_OF_ARR(X) (sizeof(X) / sizeof(X[0]))

// helper function for instance of a type
template<typename Base, typename T>
inline bool instanceof(const T* ptr) {
	return dynamic_cast<const Base*>(ptr) != nullptr;
}