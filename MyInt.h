#pragma once
#include <string>
#include <iostream>
class MyInt
{
public:
	bool isKnown;
	int value;
	MyInt() {}
	MyInt(bool knwon, int val) : isKnown(knwon), value(val) {}
	MyInt operator + (MyInt other)
	{
		MyInt result;
		if (!other.isKnown || !this->isKnown)
		{
			result.isKnown = false;
			value = 0;
			return result;
		}
		result.isKnown = true;
		result.value = this->value + other.value;
		return result;
	}
	MyInt operator - (MyInt other)
	{
		other.value *= -1;
		return *this + other;
	}
	MyInt operator * (MyInt other)
	{
		MyInt result;
		if (!other.isKnown || !this->isKnown)
		{
			result.isKnown = false;
			value = 0;
			return result;
		}
		result.isKnown = true;
		result.value = this->value * other.value;
		return result;
	}
	MyInt operator / (MyInt other)
	{
		MyInt result;
		if (!other.isKnown || !this->isKnown)
		{
			result.isKnown = false;
			value = 0;
			return result;
		}
		result.isKnown = true;
		result.value = this->value / other.value;
		return result;
	}
	friend std::ostream& operator <<(std::ostream& os,const MyInt mi)
	{
		if (!mi.isKnown)
			std::cout << "Unknown";
		else
			std::cout << mi.value;
		return os;
	}
	void revise(bool known, int val) { isKnown = known; value = val; }
	void revise(MyInt val) { isKnown = val.isKnown; value = val.value; }
};