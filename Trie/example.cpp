#include "Trie.hpp"

int main() {
	std::string s1 = "car";
	std::string s2 = "call";
	std::string s3 = "chris";

	Trie t;
	t.insert(s1);
	t.insert(s2);
	t.insert(s3);

	std::cout << t.search(s1) << '\n';
	std::cout << t.starts_with(std::string("ca")) << '\n';
	std::cout << t.starts_with(std::string("ba")) << '\n';

	return 0;
}