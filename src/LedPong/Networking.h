#pragma once
#include <algorithm> // max, min
#include <iostream>
#include <string.h>
using namespace std;

/// <summary>
/// This class provides utilities for networking.
/// </summary>
class Networking
{
public:
	void ReadAdapterInfo();
	string EthIp = "";
	string WifiIp = "";
};

