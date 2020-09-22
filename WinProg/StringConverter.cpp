#include "StringConverter.h"

std::wstring StringConverter::ToWstring(const std::string& str)
{
	return std::wstring(str.begin(), str.end());
}
