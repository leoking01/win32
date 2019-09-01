// las referencias son variables que contienen direcciones de memoria (igual que los punteros)
// las diferencias con los punteros son:
// - las referencias tienen que ser obligatoriamente inicializadas en el momento de la declaracion 
// - no es necesario el operador unario & para asignar la direccion de memoria a la referencia. (se llama automaticamente) -- int *pall = &all
// - cuando se modifica el valor de una referencia, en realidad se esta cambiando el valor del contenido. -- *pall = 30
#include "pch.h"
#include <iostream>
#include <cstring>

class Beer {
public:
	char name[32];
	Beer() {
		strcpy_s(name, "");
	}
};

void renameBeer(Beer& beer) {
	std::cout << "Old name:" << beer.name << "\n";
	strcpy_s(beer.name, "Heineken");
}

int main(int argc, char* argv[])
{
	Beer beer = Beer();
	strcpy_s(beer.name, "Voll Damm");
	renameBeer(beer);
	std::cout << "New name:" << beer.name << "\n";
	return 0;
}