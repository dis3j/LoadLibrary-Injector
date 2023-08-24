#include <Windows.h>
#include <iostream>
#include "memory.hpp"
#include <string>
#include <format>
#include <filesystem>
#include <stdio.h>
#include <iostream>
#include <direct.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#pragma comment(lib, "urlmon.lib")

using namespace std;
string namedll;
Injector inj;

DWORD pid;


void bypass()
{
	LPVOID ntOpenFile = GetProcAddress(LoadLibraryW(L"ntdll"), "NtOpenFile");
	remove("C:\\Windows\\bcastdvr\\dll.dll"); // removing old dll


	if (ntOpenFile) {
		char originalBytes[5];
		memcpy(originalBytes, ntOpenFile, 5);
		WriteProcessMemory(inj.process, ntOpenFile, originalBytes, 5, NULL);
	}
	else
	{
		cout << "Unable to bypass.\n"; // idk
	}
}

std::string DownloadTextFromURL(const std::string& url) {
	HINTERNET hInternet = InternetOpenA("MyApp", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInternet == NULL) {
		return "";
	}

	HINTERNET hConnect = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (hConnect == NULL) {
		InternetCloseHandle(hInternet);
		return "";
	}

	std::string result;
	char buffer[1024];
	DWORD bytesRead;
	while (InternetReadFile(hConnect, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
		result.append(buffer, bytesRead);
	}

	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);

	return result;
}

void injectCheat()
{

	bypass();

	std::string dwnld_URL = "yoursite.com/raw/update.txt"; // printing update log. example "last update: 01.01.1970"
	std::string pageContent = DownloadTextFromURL(dwnld_URL);
	Sleep(1000);
	cout << "[+] Modules succesfully loaded" << endl;
	Sleep(520);
	string dwnld_URL1 = "yoursite.com/dll/dll.dll"; // downloading new dll
	string savepath = "C:\\Windows\\bcastdvr\\dll.dll"; // save path dll
	URLDownloadToFile(NULL, dwnld_URL1.c_str(), savepath.c_str(), 0, NULL);
	inj.inject(pid, savepath.c_str());
	cout << "[+] Inject successfully\n"; 
	cout << "[c:] Last update " << pageContent << "\n"; // update pring
	Sleep(500);
	cout << "[!] Loader will Closed in 5 seconds" << endl;
	Sleep(5000);
	system("exit");
}

int main()
{
	SetConsoleTitle("YourName Loader");
	CreateDirectory("C:\\Windows\\bcastdvr", NULL);
	//CreateDirectory("C:\\Sukaru", NULL);
	URLDownloadToFile(NULL, dwnld_URL.c_str(), savepath.c_str(), 0, NULL);
	URLDownloadToFile(NULL, dwnld_URL1.c_str(), savepath1.c_str(), 0, NULL);


	remove("C:\\Windows\\bcastdvr\\dll.dll");


	HWND rustWindow = FindWindowA(0, "Rust"); // windowcheck. example: Rust. Maybe im change window check to proccess check:p
	if (rustWindow != nullptr) {
		inj.hwndproc = rustWindow;
	}
	else {
		cout << "RustClient.exe not init\n" << endl;
		Beep(500, 100);
		cout << "Loader will Closed in 5 seconds" << endl;
		system("exit");
	}	GetWindowThreadProcessId(inj.hwndproc, &pid);
	inj.process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
	inj.clientDLL = inj.GetModule(pid, "dll.dll");

	if (pid > 1)
	{
		std::string dwnld_URL2 = "yoursite.com/raw/logo.txt"; // symbol logo print
		std::string pageContent1 = DownloadTextFromURL(dwnld_URL2);
		cout << pageContent1 << "\n";
		Sleep(5000);
		injectCheat();
	}
	else
	{
		while (true)
		{
			inj.hwndproc = FindWindowA(0, "Rust");
			GetWindowThreadProcessId(inj.hwndproc, &pid);
			inj.process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
			inj.clientDLL = inj.GetModule(pid, "dll.dll");

			if (pid > 1)
				break;
		}
		Sleep(1500);
		injectCheat();

	}
}