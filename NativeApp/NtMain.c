#include <Windows.h>
#include <winternl.h>



NTSYSCALLAPI NTSTATUS NTAPI NtDisplayString(PUNICODE_STRING DisplayString);
NTSYSAPI NTSTATUS NTAPI NtTerminateProcess(HANDLE ProcessHandle, NTSTATUS ExitStatus);



#define NtCurrentProcess() ((HANDLE)((LONG_PTR)-1))



VOID NtProcessStartup(PPEB StartupArgument)
{
	UNICODE_STRING str;
	RtlInitUnicodeString(&str, L"Hello, world!\n");
	NtDisplayString(&str);
	NtTerminateProcess(NtCurrentProcess(), 0);
}