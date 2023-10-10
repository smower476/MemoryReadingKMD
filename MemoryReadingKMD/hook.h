#pragma once
#include "memory.h"

namespace nullhook {
	bool call_kernel_function(void* kernel_functuion_address);
	NTSTATUS hook_handler(PVOID called_param);
}