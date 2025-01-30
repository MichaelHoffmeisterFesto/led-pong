#pragma once

#include <iostream>

template <typename T>
class SampleGen
{
public:
	T data;
	void Display();
};

// when it is a template, the code needs to be in the header file!!

template <typename T>
void SampleGen<T>::Display()
{
	std::cout << "Hallo" << std::endl;
}

