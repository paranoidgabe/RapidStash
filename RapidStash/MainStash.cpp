
#include <iostream>

#include <gason.h>


int main(int argc, char* argv[]) {
	if (argc != 2) return 0;
	char *herp = argv[1];
	JsonValue value;
	JsonAllocator allocator;
	char* endptr = nullptr;
	int status = jsonParse(herp, &endptr, &value, allocator);
	if (status != JSON_OK) {
		std::cout << "Failure?" << std::endl;
	}
	JsonIterator beg = begin(value);
	while (beg != end(value)) {
		std::cout << beg->key << ":" << beg->value.toString() << std::endl;
		++beg;
	}
	return 0;
}