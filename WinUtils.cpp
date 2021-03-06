#include "OSCompatibilityLayer.h"
#include <Windows.h>
#include <iostream>
#include <list>
#include "Log.h"
#include <filesystem>
namespace fs = std::filesystem;

#pragma warning(disable: 4996)	// supress warnings about wcscmp()


namespace Utils
{

std::set<std::string> GetAllFilesInFolderRecursive(const std::string& path)
{
	std::set<std::string> fileNames;
	for (const auto& p : fs::recursive_directory_iterator(fs::u8path(path)))
	{
		if (!p.is_directory())
		{
			auto lastSlash = p.path().native().find_last_of(L'\\');
			auto tempDir = p.path().native().substr(0, lastSlash);
			lastSlash = tempDir.find_last_of(L'\\');
			auto dirName = tempDir.substr(lastSlash + 1, tempDir.length());
			auto returnName = "/" + UTF16ToUTF8(dirName) + "/" + p.path().filename().string();
			fileNames.insert(returnName);
		}
	}
	return fileNames;
}


std::string GetLastErrorString()
{
	const DWORD errorCode = ::GetLastError();
	const DWORD errorBufferSize = 256;
	wchar_t errorBuffer[errorBufferSize];
	const BOOL success = ::FormatMessageW(FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		errorCode,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		errorBuffer,
		errorBufferSize - 1,
		NULL);
	if (success)
	{
		return UTF16ToUTF8(errorBuffer);
	}
	else
	{
		return "Unknown error";
	}
}

std::string convertUTF8ToASCII(const std::string& UTF8)
{
	int requiredSize = WideCharToMultiByte(20127 /*US-ASCII (7-bit)*/, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, NULL, 0, "0", NULL);
	char* asciiArray = new char[requiredSize];

	if (0 == WideCharToMultiByte(20127 /*US-ASCII (7-bit)*/, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, asciiArray, requiredSize, "0", NULL))
	{
		Log(LogLevel::Error) << "Could not translate string to ASCII - " << GetLastErrorString();
	}
	std::string returnable(asciiArray);

	delete[] asciiArray;

	return returnable;
}


std::string convertUTF8To8859_15(const std::string& UTF8)
{
	int requiredSize = WideCharToMultiByte(28605 /*8859-15*/, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, NULL, 0, "0", NULL);
	char* asciiArray = new char[requiredSize];

	if (0 == WideCharToMultiByte(28605 /*8859-15*/, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, asciiArray, requiredSize, "0", NULL))
	{
		Log(LogLevel::Error) << "Could not translate string to ASCII - " << GetLastErrorString();
	}
	std::string returnable(asciiArray);

	delete[] asciiArray;

	return returnable;
}


std::string convertUTF8ToWin125_(const std::string& UTF8, int codepage)
{
	int requiredSize = WideCharToMultiByte(codepage, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, NULL, 0, "0", NULL);
	char* asciiArray = new char[requiredSize];

	if (0 == WideCharToMultiByte(codepage, 0, convertUTF8ToUTF16(UTF8).c_str(), -1, asciiArray, requiredSize, "0", NULL))
	{
		Log(LogLevel::Error) << "Could not translate string to ASCII - " << GetLastErrorString();
	}
	std::string returnable(asciiArray);

	delete[] asciiArray;

	return returnable;
}

std::string convertUTF8ToWin1252(const std::string& UTF8)
{
	return convertUTF8ToWin125_(UTF8, 1252);
}

std::string convertUTF8ToWin1251(const std::string& UTF8)
{
	return convertUTF8ToWin125_(UTF8, 1251);
}

std::string convertUTF8ToWin1250(const std::string& UTF8)
{
	return convertUTF8ToWin125_(UTF8, 1250);
}

std::string convert8859_15ToASCII(const std::string& input)
{
	return Utils::convertUTF8ToASCII(Utils::convert8859_15ToUTF8(input));
}


std::string convert8859_15ToUTF8(const std::string& input)
{
	return UTF16ToUTF8(convert8859_15ToUTF16(input));
}


std::wstring convert8859_15ToUTF16(const std::string& input)
{
	const int requiredSize = MultiByteToWideChar(28605 /* 8859-15*/, MB_PRECOMPOSED, input.c_str(), -1, NULL, 0);
	auto* wideKeyArray = new wchar_t[requiredSize];

	if (0 == MultiByteToWideChar(28605 /* 8859-15*/, MB_PRECOMPOSED, input.c_str(), -1, wideKeyArray, requiredSize))
	{
		Log(LogLevel::Error) << "Could not translate string to UTF-16 - " << GetLastErrorString();
	}
	std::wstring returnable(wideKeyArray);

	delete[] wideKeyArray;

	return returnable;
}


std::string convertWin1252ToASCII(const std::string& input)
{
	return convertUTF8ToASCII(convertWin1252ToUTF8(input));
}


std::string convertWin1252ToUTF8(const std::string& input)
{
	return UTF16ToUTF8(convertWin1252ToUTF16(input));
}

std::string convertWin1250ToUTF8(const std::string& input)
{
	return UTF16ToUTF8(convertWin1250ToUTF16(input));
}

std::wstring convertWin1250ToUTF16(const std::string& input)
{
	const int requiredSize = MultiByteToWideChar(1250, MB_PRECOMPOSED, input.c_str(), -1, NULL, 0);
	auto* wideKeyArray = new wchar_t[requiredSize];

	if (0 == MultiByteToWideChar(1250, MB_PRECOMPOSED, input.c_str(), -1, wideKeyArray, requiredSize))
	{
		Log(LogLevel::Error) << "Could not translate string to UTF-16 - " << GetLastErrorString();
	}
	std::wstring returnable(wideKeyArray);

	delete[] wideKeyArray;

	return returnable;
}

std::wstring convertWin1252ToUTF16(const std::string& input)
{
	const int requiredSize = MultiByteToWideChar(1252, MB_PRECOMPOSED, input.c_str(), -1, NULL, 0);
	auto* wideKeyArray = new wchar_t[requiredSize];

	if (0 == MultiByteToWideChar(1252, MB_PRECOMPOSED, input.c_str(), -1, wideKeyArray, requiredSize))
	{
		Log(LogLevel::Error) << "Could not translate string to UTF-16 - " << GetLastErrorString();
	}
	std::wstring returnable(wideKeyArray);

	delete[] wideKeyArray;

	return returnable;
}


std::wstring convertUTF8ToUTF16(const std::string& UTF8)
{
	const int requiredSize = MultiByteToWideChar(CP_UTF8, 0, UTF8.c_str(), -1, NULL, 0);
	if (requiredSize == 0)
	{
		Log(LogLevel::Error) << "Could not translate string to UTF-16 - " << GetLastErrorString();
	}
	auto* wideKeyArray = new wchar_t[requiredSize];

	if (0 == MultiByteToWideChar(CP_UTF8, 0, UTF8.c_str(), -1, wideKeyArray, requiredSize))
	{
		Log(LogLevel::Error) << "Could not translate string to UTF-16 - " << GetLastErrorString();
	}
	std::wstring returnable(wideKeyArray);

	delete[] wideKeyArray;

	return returnable;
}


std::string convertToUTF8(const std::wstring& input)
{
	return UTF16ToUTF8(input);
}

void WriteToConsole(LogLevel level, const std::string& logMessage)
{
	if (level == LogLevel::Debug)
	{	// Don't log debug messages to console.
		return;
	}

	HANDLE console = GetStdHandle(STD_OUTPUT_HANDLE);	// a handle to the console window
	if (console != INVALID_HANDLE_VALUE)
	{
		CONSOLE_SCREEN_BUFFER_INFO oldConsoleInfo;	// the current (soon to be outdated) console data
		const BOOL success = GetConsoleScreenBufferInfo(console, &oldConsoleInfo);	// whether or not the console data could be retrieved
		if (success)
		{
			WORD color;
			switch (level)
			{
				case LogLevel::Error:
					color = FOREGROUND_RED | FOREGROUND_INTENSITY;
					break;

				case LogLevel::Warning:
					color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY;
					break;

				case LogLevel::Info:
					color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE | FOREGROUND_INTENSITY;
					break;

				case LogLevel::Debug:
				default:
					color = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
					break;
			}
			SetConsoleTextAttribute(console, color);
			DWORD bytesWritten = 0;
			WriteConsoleW(console, Utils::convertUTF8ToUTF16(logMessage).c_str(), static_cast<DWORD>(logMessage.size()), &bytesWritten, nullptr);

			// Restore old console color.
			SetConsoleTextAttribute(console, oldConsoleInfo.wAttributes);

			return;
		}
	}

	std::cout << logMessage;
}

std::string convertWin1251ToUTF8(const std::string& Win1251)
{
	return cp2utf(Win1251);
}

std::string convertUTF8toWin1251(const std::string& UTF8)
{
	return utf2cp(UTF8);
}

std::optional<std::wstring> getSteamInstallPath(const std::string& steamID)
{
	if (steamID.empty())
		return std::nullopt;

	wchar_t value[255];
	value[0] = 0;
	DWORD BufferSize = 8192;
	std::wstring registryPath = convertUTF8ToUTF16(R"(SOFTWARE\Wow6432Node\Microsoft\Windows\CurrentVersion\Uninstall\Steam App )" + steamID);
	const std::wstring installPath = convertUTF8ToUTF16(R"(InstallLocation)");

	RegGetValue(HKEY_LOCAL_MACHINE, registryPath.c_str(), installPath.c_str(), RRF_RT_ANY, nullptr, (PVOID)&value, &BufferSize);

	if (value[0] != 0)
	{
		auto result = std::wstring(value);
		if (result.length() > 2)
		{
			return result;
		}
	}

	registryPath = convertUTF8ToUTF16(R"(SOFTWARE\Microsoft\Windows\CurrentVersion\Uninstall\Steam App )" + steamID);
	RegGetValue(HKEY_LOCAL_MACHINE, registryPath.c_str(), installPath.c_str(), RRF_RT_ANY, nullptr, (PVOID)&value, &BufferSize);

	if (value[0] != 0)
	{
		auto result = std::wstring(value);
		if (result.length() > 2)
		{
			return result;
		}
	}

	return std::nullopt;
}

} // namespace Utils
