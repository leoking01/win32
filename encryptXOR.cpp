#include <iostream>

void XOREncrypt(std::string toEncrypt, char key) {
	std::string Encrypted;

	std::cout << "XOR key: " << key << std::endl;
	std::cout << "Xor Encrypted: ";
	for (int i = 0; i < toEncrypt.size(); i++) {
		std::cout << (size_t)(toEncrypt[i] ^ key) << ',';
	}
	// last char is key
	std::cout << (size_t)key << std::endl;

	std::cout << "Length with key: " << toEncrypt.size()+1 << std::endl;
	
	return;
}

int main(int argc, char* argv[]) {
	char key;

	if (argc == 3) {
		key = *argv[2];
		XOREncrypt(std::string(argv[1]), key);
	}
	else {
		std::cout << "Usage: ./EncryptXOR <string> <key>";
	}
    
	return 0;
}