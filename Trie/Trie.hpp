#include <string>

class Trie {
public:
	struct TrieNode {
		char c;
		TrieNode* children[26] = { nullptr };
		bool isWord;
		
		TrieNode(char in_c) : c{in_c}, isWord{false} {}
	};

	// Constructor - Destructor
	Trie() {
		root = new TrieNode('\0');
	}

	~Trie() {
		destroy(root);
	}
	
	// Functions
	void insert(const std::string& word) {
		TrieNode* current = root;
		for (char c : word) {
			if (!current->children[c - 'a'])
				current->children[c - 'a'] = new TrieNode(c);
			current = current->children[c - 'a'];
		}
		current->isWord = true;
	}

	bool search(const std::string& word) {
		TrieNode* n = get_TrieNode(word);
		return n != nullptr && n->isWord;
	}

	bool starts_with(const std::string& word) {
		TrieNode* n = get_TrieNode(word);
		return n != nullptr;
	}

private:
	TrieNode* root;

	TrieNode* get_TrieNode(const std::string& word) {
		TrieNode* current = root;
		for (char c : word) {
			if (!current->children[c - 'a'])
				return nullptr;
			current = current->children[c - 'a'];
		}
		return current;		
	}

	void destroy(TrieNode* node) {
		for (TrieNode* n : node->children)
			if (n) 
				destroy(n);
		delete node;
	}
};