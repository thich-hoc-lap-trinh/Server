#include "Library.h"
using namespace std;

void shutdownServer()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        wcerr << L"Failed to open process token." << endl;
        return;
    }

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);

    if (GetLastError() != ERROR_SUCCESS)
    {
        wcerr << L"Failed to adjust token privileges." << endl;
        CloseHandle(hToken);
        return;
    }

    if (!InitiateSystemShutdownEx(NULL, NULL, 0, TRUE, FALSE, SHTDN_REASON_MAJOR_OTHER))
    {
        wcerr << L"Failed to shutdown the server. Error: " << GetLastError() << endl;
    }

    CloseHandle(hToken);
}

void resetServer()
{
    HANDLE hToken;
    TOKEN_PRIVILEGES tkp;

    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &hToken))
    {
        wcerr << L"Failed to open process token." << endl;
        return;
    }

    LookupPrivilegeValue(NULL, SE_SHUTDOWN_NAME, &tkp.Privileges[0].Luid);
    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(hToken, FALSE, &tkp, 0, NULL, 0);

    if (GetLastError() != ERROR_SUCCESS)
    {
        wcerr << L"Failed to adjust token privileges." << endl;
        CloseHandle(hToken);
        return;
    }

    if (!InitiateSystemShutdownEx(NULL, NULL, 0, TRUE, TRUE, SHTDN_REASON_MAJOR_OTHER)) {
        wcerr << L"Failed to restart the server. Error: " << GetLastError() << endl;
    }

    CloseHandle(hToken);
}