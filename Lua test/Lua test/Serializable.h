#pragma once
#include <string>

class Serializable
{
public:
	virtual void Serialize(std::ostream & s) = 0;
};