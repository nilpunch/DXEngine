#include "ErrorLogger.h"
#include <comdef.h>

void ErrorLogger::Log(const std::string& message)
{
	std::string errorMessage = "Error: " + message;
	MessageBoxA(NULL, errorMessage.c_str(), "DXEngine Error", MB_ICONERROR | MB_OK);
}

void ErrorLogger::Log(HRESULT hr, const std::string& message)
{
	_com_error error(hr);
	std::wstring errorMessage = L"Error: " + StringConverter::ToWstring(message) + L'\n' + error.ErrorMessage();
	MessageBoxW(NULL, errorMessage.c_str(), L"DXEngine Error", MB_ICONERROR | MB_OK);
}

void ErrorLogger::Log(HRESULT hr, const std::wstring & message)
{
	_com_error error(hr);
	std::wstring errorMessage = L"Error: " + message + L'\n' + error.ErrorMessage();
	MessageBoxW(NULL, errorMessage.c_str(), L"DXEngine Error", MB_ICONERROR | MB_OK);
}

std::string ErrorLogger::ErrorLineFormat(const std::string & message, const std::string & file, int line)
{
	return "[" + message + "]>" + file + ":" + std::to_string(line);
}
