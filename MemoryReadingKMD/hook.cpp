#include "hook.h"

bool nullhook::call_kernel_function(void* kernel_function_address) {
	if (!kernel_function_address)
		return false;
	PVOID* function = reinterpret_cast<PVOID*>(get_system_module_export("\\SystemRoot\\System32\\drivers\\dxgkrnl.sys","NtOpenCompositionSurfaceSectionInfo")); //NtOpenCompositionSurfaceSectionInfo
	if (!function)
		return false;
	
	BYTE orig[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	BYTE shell_code[] = { 0x48, 0xB8 }; // mov rax, fffxxx... //48 C7 C0 xx xx xx xx

	BYTE shell_code_end[] = { 0xFF, 0xE0 };// jmp rax // FF 10

	//change 11 and 12
	RtlSecureZeroMemory(&orig, sizeof(orig));
	memcpy((PVOID)((ULONG_PTR)orig), &shell_code, sizeof(shell_code));
	uintptr_t hook_address = reinterpret_cast<uintptr_t>(kernel_function_address);
	memcpy((PVOID)((ULONG_PTR)orig + sizeof(shell_code)), & hook_address, sizeof(void*));
	memcpy((PVOID)((ULONG_PTR)orig + sizeof(shell_code) + sizeof(void*)), &shell_code_end, sizeof(shell_code_end));
	
	write_to_read_only_memory(function, &orig, sizeof(orig));
	return true;
}

NTSTATUS nullhook::hook_handler(PVOID called_param) {
	NULL_MEMORY* instructions = (NULL_MEMORY*)called_param;

	if (instructions->req_base != FALSE) {
		ANSI_STRING AS;
		UNICODE_STRING ModuleName;

		RtlInitAnsiString(&AS, instructions->module_name);
		RtlAnsiStringToUnicodeString(&ModuleName, &AS, TRUE);
		PEPROCESS process;
		PsLookupProcessByProcessId((HANDLE)instructions->pid, &process);
		ULONG base_address64 = NULL;
		base_address64 = get_module_base_x64(process, ModuleName);
	}
}