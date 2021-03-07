#include <string>

class Trie {
public:
	struct TrieNode {
		char c;
		TrieNode* children[26] = { nullptr };
		bool isWord;
		
		TrieNode(char c) {
			this->c = c;
			isWord = false;
		}
	};

	// Constructor - Destructor
	Trie();
	~Trie();
	
	// Functions
	void insert(const std::string& word);
	bool search(const std::string& word);
	bool starts_with(const std::string& word);

private:
	TrieNode* root;
	TrieNode* get_TrieNode(const std::string& word);
	void destroy(TrieNode* to_destroy);
};

