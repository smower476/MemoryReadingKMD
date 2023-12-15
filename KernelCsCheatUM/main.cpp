#include <iostream>
#include <Windows.h>
#include <TlHelp32.h>
#include <memory>
#include <string_view>
#include <cstdint>
#include <vector>

typedef struct _NULL_MEMORY {
	void* buffer_address;
	UINT_PTR address;
	ULONGLONG size;
	ULONG pid;
	BOOLEAN write;
	BOOLEAN read;
	BOOLEAN req_base;
	void* output;
	const char* module_name;
	ULONG64 base_address;
}NULL_MEMORY;

uintptr_t base_address = 0;
std::uint32_t process_id = 0;

template<typename ... Arg>
uint64_t call_hook(const Arg ... args) {
	//LoadLibrary("user32.dll");
	void* hooked_func = GetProcAddress(LoadLibrary("win32u.dll"),  "NtDxgkDuplicateHandle"); // NtOpenCompositionSurfaceSectionInfo 
	auto func = static_cast<uint64_t(_stdcall*)(Arg...)>(hooked_func);

		return func(args ...);
}

struct HandleDisposer
{
	using pointer = HANDLE;
	void operator()(HANDLE handle) const
	{
		if ((handle != NULL) or (handle != INVALID_HANDLE_VALUE)) 
		{
			CloseHandle(handle);
		}
	}
};

using unique_handle = std::unique_ptr<HANDLE, HandleDisposer>;

static std::uint32_t get_process_id(std::string_view process_name) {
	PROCESSENTRY32 processentry;
	const unique_handle snapshot_handle(CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL));
	if (snapshot_handle.get() == INVALID_HANDLE_VALUE) {
		return NULL;
		}

	processentry.dwSize = sizeof(MODULEENTRY32);
	while (Process32Next(snapshot_handle.get(), &processentry) == TRUE) {
		if (process_name.compare(processentry.szExeFile) == NULL) {
			return processentry.th32ProcessID;
		}
	}
	return NULL;
}

static ULONG64 get_module_base_address(const char* module_name) { // module name = cliend.dll for csgo
	NULL_MEMORY instructions = { 0 };
	instructions.pid = process_id;
	instructions.req_base = TRUE;
	instructions.read = FALSE;
	instructions.write = FALSE;
	instructions.module_name = module_name;
	call_hook(&instructions);

	ULONG base = NULL;
	base = instructions.base_address;
	return base;
}

template <class T>
T Read(UINT_PTR read_address)
{
	T response{};
	NULL_MEMORY instructions;
	instructions.pid = process_id;
	instructions.size = sizeof(T);
	instructions.address = read_address;
	instructions.read = TRUE;
	instructions.write = FALSE;
	instructions.req_base = FALSE;
	instructions.output = &response;
	call_hook(&instructions);
	return response;
}

bool write_memory(UINT_PTR write_address, UINT_PTR source_address, SIZE_T write_size) {
	NULL_MEMORY instructions;
	instructions.address = write_address;
	instructions.pid = process_id;
	instructions.write = TRUE;
	instructions.read = FALSE;
	instructions.req_base = FALSE;
	instructions.buffer_address = (void*)source_address;
	instructions.size = write_size;
	call_hook(&instructions);
	return true;
}

template<typename S>
bool write(UINT_PTR write_address, const S& value) {
	return write_memory(write_address, (UINT_PTR)&value, sizeof(S));
}

int main()
{
	LoadLibrary("user32.dll");
	
	process_id = get_process_id("WINMINE.EXE");
	std::cout<< process_id <<std::endl;
	if (process_id == 0) {
		std::cout << "process not found";
		goto leave;
	}
	std::cout << "process id = "<< process_id << std::endl;
	Sleep(1000);

	//std::cout << get_module_base_address("KERNELBASE.dll");
	base_address = get_module_base_address("WINMINE.EXE");
	if (base_address == 0) {
		std::cout << "dll not found";
		goto leave;
	}
	std::cout <<"base address = " << base_address << std::endl;
	Sleep(1000);
	

	char c;
	while (true) {
		std::cout << "Do you want to read or write memory? r/w" << std::endl << "e for exit" << std::endl;
		std::cin >> c;
		switch (c) {
			case 'r':
					std::cout<< "The value is " << Read<int32_t>(base_address + 0x5194) << std::endl;
					break;
			case 'w':
				int32_t number;
				std::cout << "Enter the number" << std::endl;
				std::cin >> number;
				write(base_address + 0x5194, number);
				std::cout << "Done!" << std::endl;
				break;
			case 'e':
				goto leave;
			default:
				std::cout << "Wrong value" << std::endl;
				break;
		}

	
	}

	//std::cout<<Read<int>(0x29D312C2044)<<" Read INT";
	
leave: 
	Sleep(5000);

	return NULL;
}