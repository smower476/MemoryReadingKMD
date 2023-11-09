#include "memory.h"



PVOID get_system_module_base(const char* module_name)
{
	ULONG bytes = 0;
	NTSTATUS status = ZwQuerySystemInformation(SystemModuleInformation, NULL, bytes, &bytes);

	if (!bytes)
		return NULL;

	PRTL_PROCESS_MODULES modules = (PRTL_PROCESS_MODULES)ExAllocatePoolWithTag(NonPagedPool, bytes, 0x4e554c4c);

	status = ZwQuerySystemInformation(SystemModuleInformation, modules, bytes, &bytes);

	if (!NT_SUCCESS(status))
		return NULL;

	PRTL_PROCESS_MODULE_INFORMATION module = modules->Modules;
	PVOID module_base = 0, module_size = 0;
	for (ULONG i = 0; i < modules->NumberOfModules;	 i++)
	{
		if (strcmp((char*)module[i].FullPathName, module_name) == NULL)
		{
			module_base = module[i].ImageBase;
			module_size = (PVOID)module[i].ImageSize;
			break;
		}
	}

	if (modules)
		ExFreePoolWithTag(modules, NULL);
	if (module_base == NULL) // <=
		return NULL;

	return module_base;
}

PVOID get_system_module_export(const char* module_name, LPCSTR routine_name)
{
	PVOID lpModule = get_system_module_base(module_name);
	if (!lpModule)
		return NULL;

	return RtlFindExportedRoutineByName(lpModule, routine_name);
}

bool write_memory(void* address, void* buffer, size_t size)
{
	if (memcpy(address, buffer, size)) // #define RtlCopyMemory(Destination,Source,Length) memcpy((Destination),(Source),(Length))
	{									// ! strange ! symbol
		return true;					// true <> false
	}									//  return (x >= 0 && x < 10);
	else {
		return false;
	}
}

bool write_to_read_only_memory(void* address, void* buffer, size_t size) {
	PMDL Mdl = IoAllocateMdl(address, size, FALSE, FALSE, NULL);
	if (!Mdl)
		return false;
	MmProbeAndLockPages(Mdl, KernelMode, IoReadAccess);
	PVOID Mapping = MmMapLockedPagesSpecifyCache(Mdl, KernelMode, MmNonCached, NULL, FALSE, NormalPagePriority);

	write_memory(Mapping, buffer, size);
	MmUnmapLockedPages(Mapping, Mdl);	
	MmUnlockPages(Mdl);
	IoFreeMdl(Mdl);
	return true;
}

ULONG64 get_module_base_x64(PEPROCESS proc, UNICODE_STRING module_name) {
	PPEB pPeb = PsGetProcessPeb(proc);
	if (!pPeb) {
		return NULL;
	}
	KAPC_STATE state;
	KeStackAttachProcess(proc, &state);
	PPEB_LDR_DATA pLdr = (PPEB_LDR_DATA)pPeb->Ldr;

}