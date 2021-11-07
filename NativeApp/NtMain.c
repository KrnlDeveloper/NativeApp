#include <Windows.h>
#include <winternl.h>



NTSYSAPI NTSTATUS NTAPI NtDisplayString(PUNICODE_STRING DisplayString);
NTSYSAPI NTSTATUS NTAPI NtTerminateProcess(HANDLE ProcessHandle, NTSTATUS ExitStatus);



#define NtCurrentProcess() ((HANDLE)((LONG_PTR)-1))


NTSYSAPI NTSTATUS NTAPI NtDelayExecution(IN BOOLEAN Alertable, IN PLARGE_INTEGER DelayInterval);
NTSYSAPI NTSTATUS NTAPI NtWriteFile(HANDLE FileHandle, HANDLE Event, PIO_APC_ROUTINE ApcRoutine, PVOID ApcContext,
	PIO_STATUS_BLOCK IoStatusBlock, PVOID Buffer, ULONG Length, PLARGE_INTEGER ByteOffset, PULONG Key);


NTSYSAPI ULONG NTAPI DbgPrint(PCSTR Format, ...);

VOID DbgMsg(PWCHAR Message)
{
	UNICODE_STRING str;
	RtlInitUnicodeString(&str, Message);
	NtDisplayString(&str);
}


NTSTATUS NtAddToLogFile(PUNICODE_STRING FileName)
{
	HANDLE logFile;
	OBJECT_ATTRIBUTES objAttributes = { 0 };
	IO_STATUS_BLOCK statusBlock = { 0 };
	LARGE_INTEGER allocSize = { 0 }; //????? Maybe NULL ?

	InitializeObjectAttributes(&objAttributes, FileName, OBJ_OPENIF, NULL, NULL);

	NTSTATUS status = NtCreateFile(&logFile, FILE_WRITE_DATA, &objAttributes,
		&statusBlock, &allocSize, FILE_ATTRIBUTE_NORMAL,
		FILE_SHARE_READ, FILE_OPEN_IF, FILE_NON_DIRECTORY_FILE | FILE_RANDOM_ACCESS, NULL, 0);

	if (!NT_SUCCESS(status))
	{
		DbgMsg(L"ERROR after NtCreateFile\n");
		return status;
	}

	CHAR buffer[] = "Program working!";
	LARGE_INTEGER offset = {0};
	offset.HighPart = -1;
	offset.LowPart = 0xffffffff;
	status = NtWriteFile(logFile, NULL, NULL, NULL, &statusBlock, &buffer, sizeof(buffer), &offset, NULL);
	if (!NT_SUCCESS(status))
	{
		DbgMsg(L"ERROR after NtWriteFile\n");
		return status;
	}

	status = NtClose(logFile);
	//if (!NT_SUCCESS(status))
	//{
	//	return status;
	//}
	//return STATUS_SUCCESS;

	return status;
}



VOID NtProcessStartup(PPEB StartupArgument)
{
	UNICODE_STRING str;
	RtlInitUnicodeString(&str, L"Hello, world!\n");
	NtDisplayString(&str);
	DbgPrint("Hi, there!\n");
		
	UNICODE_STRING fileName;
	RtlInitUnicodeString(&fileName, L"\\??\\C:\\Users\\Dev\\Documents\\log.txt");
	NtAddToLogFile(&fileName);

	LARGE_INTEGER delay = { 0 };
	delay.QuadPart = -150000000;
	NtDelayExecution(FALSE, &delay);

	NtTerminateProcess(NtCurrentProcess(), 0);
}