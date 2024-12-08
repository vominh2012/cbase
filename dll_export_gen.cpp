#include <string>
#include <iostream>
#include <sstream>

// util for create def file from dumbin exports
// i use it when you dont have source code of dll to build
// Usage: dumpbin /exports lib.dll | dll_export_gen.exe > lib.def
int main() {
	for (std::string line; std::getline(std::cin, line) && std::string::npos == line.find("    ordinal hint"); );
    
	int ord;
	std::string name, hint, rva, crap1, crap2;
    
	std::cout << "EXPORTS\n";
    std::string line; 
    
	while (std::getline(std::cin, line))
    {
        ord = 0;
        name.clear();
        std::stringstream(line) >> ord >> hint >> rva >> name;
        if (name.length() > 0 && ord > 0)
            std::cout << name << "  @" << ord << '\n';
    }
}