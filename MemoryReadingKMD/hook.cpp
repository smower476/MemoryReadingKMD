#include "hook.h"

bool nullhook::call_kernel_function(void* kernel_function_address) {
	if (!kernel_function_address)
		return false;
	PVOID* function = reinterpret_cast<PVOID*>(get_system_module_export("\\SystemRoot\\System32\\drivers\\dxgkrnl.sys","NtOpenCompositionSurfaceSectionInfo	"));
	if (!function)
		return false;
	
	BYTE orig[] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	BYTE shell_code = 
}