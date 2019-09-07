#include <iostream>
#include <cstdio>
#include <Windows.h>

// macro para convertir de RVA a VA (sumando la RVA a la base)
#define RVA2VA(base, rva) ((ULONG_PTR)base+rva)

int main(int argc, char* argv[]) {
	HMODULE hLibrary = LoadLibrary(L"user32.dll");
	if (hLibrary == NULL) {
		return FALSE;
	}

	PIMAGE_DOS_HEADER pDOSHdr = (PIMAGE_DOS_HEADER)hLibrary;
	if (pDOSHdr->e_magic != IMAGE_DOS_SIGNATURE) {
		return FALSE;
	}

	PIMAGE_NT_HEADERS pNTHdr = (PIMAGE_NT_HEADERS) RVA2VA(hLibrary, pDOSHdr->e_lfanew);
	if (pNTHdr->Signature != IMAGE_NT_SIGNATURE) {
		return FALSE;
	}

	//std::cout << "base: " << (ULONG_PTR)hLibrary << ", RVA: " << (ULONG_PTR)pDOSHdr->e_lfanew << ", VA: " << (LPVOID)pNTHdr << std::endl;
	//printf("base: %p, RVA: %p, VA: %p", hLibrary, pDOSHdr->e_lfanew, pNTHdr);

	if (pNTHdr->FileHeader.Characteristics & IMAGE_FILE_DLL == 0 ||
		pNTHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress == 0 ||
		pNTHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size == 0) {
		return FALSE;
	}

	PIMAGE_EXPORT_DIRECTORY pIED = (PIMAGE_EXPORT_DIRECTORY)RVA2VA(hLibrary, 
									pNTHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	PDWORD pdwAddress = (PDWORD)RVA2VA(hLibrary, pIED->AddressOfFunctions);
	PDWORD pdwNames = (PDWORD)RVA2VA(hLibrary, pIED->AddressOfNames);
	PWORD pwOrdinals = (PWORD)RVA2VA(hLibrary, pIED->AddressOfNameOrdinals);
	PCHAR dll = (PCHAR)RVA2VA(hLibrary, pIED->Name);

	std::cout << dll << std::endl;
	std::cout << "number of functions: " << pIED->NumberOfNames << std::endl;
	std::cout << "number of names: " << pIED->NumberOfNames << std::endl;
    /*
	for (DWORD i = 0; i < pIED->NumberOfFunctions; i++) {
		std::cout << (LPVOID)RVA2VA(hLibrary, pdwAddress[pwOrdinals[i]]) << " ";
		std::cout << (LPCSTR)RVA2VA(hLibrary, pdwNames[i]) << std::endl;
	}
	*/

	PCHAR api;      // nombre de la api
	int i;
	LPVOID api_addr=NULL;
	i = pIED->NumberOfNames;
	if (i == 0) {
		return FALSE;
	}
	do {
		api = (PCHAR)RVA2VA(hLibrary, pdwNames[i]);
			std::cout << api << std::endl;
	} while (--i && api_addr == 0);

	return 0;
}