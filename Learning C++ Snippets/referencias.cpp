#include "pch.h"
#include <iostream>

int main()
{
	// las referencias son variables que contienen direcciones de memoria (igual que los punteros)
	// las diferencias con los punteros son:
	// - las referencias tienen que ser obligatoriamente inicializadas en el momento de la declaracion 
	// - no es necesario el operador unario & para asignar la direccion de memoria a la referencia. (se llama automaticamente) -- int *pall = &all
	// - cuando se modifica el valor de una referencia, en realidad se esta cambiando el valor del contenido. -- *pall = 30

	int all = 42;
	int& ref_all = all;
	ref_all = 30;
	std::cout << ref_all << std::endl;
}
