#include "Trie.hpp"

Trie::Trie() {
	root = new TrieNode('\0');
}

Trie::~Trie() {
	destroy(root);
}

void Trie::destroy(TrieNode* n) {
	for (TrieNode* n : n->children)
		if (n) 
			destroy(n);
	delete n;
}


void Trie::insert(const std::string& word) {
	TrieNode* current = root;
	for (char c : word) {
		if (!current->children[c - 'a'])
			current->children[c - 'a'] = new TrieNode(c);
		current = current->children[c - 'a'];
	}
	current->isWord = true;
}


bool Trie::search(const std::string& word) {
	TrieNode* n = get_TrieNode(word);
	return n != nullptr && n->isWord;
}

bool Trie::starts_with(const std::string& word) {
	TrieNode* n = get_TrieNode(word);
	return n != nullptr;
}

Trie::TrieNode* Trie::get_TrieNode(const std::string& word) {
	TrieNode* current = root;
	for (char c : word) {
		if (!current->children[c - 'a'])
			return nullptr;
		current = current->children[c - 'a'];
	}
	return current;
}