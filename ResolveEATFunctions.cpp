#include <iostream>
#include <Windows.h>

// macro para convertir de RVA (relative virtual adress) a VA (sumando la RVA a la base)
#define RVA2VA(base, rva) ((ULONG_PTR)base+rva)

// entrada para los nombres de las funciones de api
typedef struct ENTRY {
	LPCWSTR u32Hash;	// "hash" de la funcion en XOR con clave 'x'
	DWORD length;		// longitud del hash
	LPCWSTR cszwMod;	// nombre de la DLL
	PVOID lpAddr;		// direccion de la funcion
} ENTRY, *PENTRY;

typedef struct TABLE_ENTRIES {
	ENTRY reEntries[];
} TABLE_ENTRIES, *PTABLE_ENTRIES;


std::string XOREncrypt(std::string toEncrypt, char key) {
	std::string Encrypted;

	for (int i = 0; i < toEncrypt.size(); i++) {
		std::string encodedChar(1, toEncrypt[i] ^ key);
		Encrypted.append(encodedChar);
	}

	return Encrypted;
}

std::string wstrtostr(LPCWSTR s, DWORD size) {
	std::string str;

	for (DWORD i = 0; i < size; i++)
		str += s[i];

	return str;
}

BOOL resolve_entry(PENTRY pentry) {

	HMODULE hLibrary = LoadLibrary(pentry->cszwMod);
	if (hLibrary == NULL) {
		return FALSE;
	}

	PIMAGE_DOS_HEADER pDOSHdr = (PIMAGE_DOS_HEADER)hLibrary;
	if (pDOSHdr->e_magic != IMAGE_DOS_SIGNATURE) {
		return FALSE;
	}

	PIMAGE_NT_HEADERS pNTHdr = (PIMAGE_NT_HEADERS)RVA2VA(hLibrary, pDOSHdr->e_lfanew);
	if (pNTHdr->Signature != IMAGE_NT_SIGNATURE) {
		return FALSE;
	}

	//std::cout << "base: " << (ULONG_PTR)hLibrary << ", RVA: " << (ULONG_PTR)pDOSHdr->e_lfanew << ", VA: " << (LPVOID)pNTHdr << std::endl;
	//printf("base: %p, RVA: %p, VA: %p", hLibrary, pDOSHdr->e_lfanew, pNTHdr);

	if ((pNTHdr->FileHeader.Characteristics & IMAGE_FILE_DLL) == 0 ||
		pNTHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress == 0 ||
		pNTHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].Size == 0) {
		return FALSE;
	}

	// estructura IMAGE_EXPORT_DIRECTORY la cual tiene las direcciones de las funciones
	PIMAGE_EXPORT_DIRECTORY pIED = (PIMAGE_EXPORT_DIRECTORY)RVA2VA(hLibrary,
		pNTHdr->OptionalHeader.DataDirectory[IMAGE_DIRECTORY_ENTRY_EXPORT].VirtualAddress);

	// para buscar una funcion hay que recorrer AdressOfNames y si coincide el nombre
	// entonces tomamos el valor de AdressOfNameOrdinals en el mismo indice que AdressOfNames
	// que contiene el indice de la funcion en AdressOfFunctions
	PDWORD pdwAddress = (PDWORD)RVA2VA(hLibrary, pIED->AddressOfFunctions);
	PDWORD pdwNames = (PDWORD)RVA2VA(hLibrary, pIED->AddressOfNames);
	PWORD pwOrdinals = (PWORD)RVA2VA(hLibrary, pIED->AddressOfNameOrdinals);
	PCHAR dll = (PCHAR)RVA2VA(hLibrary, pIED->Name);

	/*
	std::cout << dll << std::endl;
	std::cout << "number of functions: " << pIED->NumberOfNames << std::endl;
	std::cout << "number of names: " << pIED->NumberOfNames << std::endl;
	*/

	PCHAR api;
	int i;
	LPVOID api_addr = NULL;

	i = pIED->NumberOfNames;
	if (i == 0) {
		return FALSE;
	}
	do {
		// nombre de la funcion en la DLL
		api = (PCHAR)RVA2VA(hLibrary, pdwNames[i]);
		// obtener el "hash" cifrado en XOR
		std::string hash_tabla = wstrtostr((pentry->u32Hash), pentry->length);
		std::string api_encoded = XOREncrypt(std::string(api), 'x');
		if (hash_tabla == api_encoded) {
			//std::cout << "**" << api << std::endl;
			pentry->lpAddr = (LPVOID)RVA2VA(hLibrary, pdwAddress[pwOrdinals[i]]);
			return TRUE;
		}
	} while (--i && api_addr == 0);

	return FALSE;
}

// obtener todas las direcciones de las funciones api
BOOL resolve_table(PTABLE_ENTRIES ptable, size_t size) {
	for (int i = 0; i < size; i++) {
		if (!resolve_entry((PENTRY)&ptable->reEntries[i]))
			return FALSE;
	}

	return TRUE;
}

int main(int argc, char* argv[]) {
	// punteros a funciones de la api
	// MessageBoxA
	typedef INT(WINAPI * _MBA)(HWND, LPCSTR, LPCSTR, UINT);
	// Beep
	typedef BOOL(WINAPI * _BP)(DWORD, DWORD);

	TABLE_ENTRIES eExampleTable = { 
		{
			{ L"\x35\x1d\x0b\x0b\x19\x1f\x1d\x3a\x17\x00\x39", 11, L"user32.dll", NULL },			// MessageBoxA
			{ L"\x3a\x1d\x1d\x08", 4, L"kernel32.dll", NULL}										// Beep
		} 
	};

	resolve_table(&eExampleTable, 2);

	_MBA mba = (_MBA)eExampleTable.reEntries[0].lpAddr;
	mba(NULL, (const char *)"hola", "mundologo", MB_OKCANCEL);

	_BP bp = (_BP)eExampleTable.reEntries[1].lpAddr;
	bp(750, 500);

	return 0;
}