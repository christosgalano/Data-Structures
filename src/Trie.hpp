#include <string>
#include <initializer_list>


namespace cds {

class Trie {
public:
	Trie();
	explicit Trie(const std::initializer_list<std::string>& words);
	~Trie();
	
	void insert(const std::string& word);
	void remove(const std::string& word);
	
	bool search(const std::string& word);
	bool starts_with(const std::string& word);

	std::size_t size() const { return sz; 	   }
	bool empty()       const { return sz == 0; }
	
private:
	struct TrieNode;

	TrieNode* root;
	std::size_t sz;

	TrieNode* get_TrieNode(const std::string& word);
	void destroy(TrieNode* node);
	TrieNode* p_remove(TrieNode* node, const std::string& word, int depth);
};


struct Trie::TrieNode {
	char c;
	TrieNode* children[26] = { nullptr };
	bool isWord;
		
	TrieNode(char in_c) : c{in_c}, isWord{false} {}

	bool is_leaf() {
		for (TrieNode* n : children)
			if (n)
				return false;
		return true;
	}
};


Trie::Trie() 
	: root{new TrieNode{'\0'}}, sz{} {}
	

Trie::Trie(const std::initializer_list<std::string>& words) 
	: root{new TrieNode{'\0'}}, sz{}
{
	for (auto& w : words)
		insert(w);		
}


Trie::~Trie() {
	destroy(root);
	sz = 0;
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


void Trie::destroy(TrieNode* node) {
	for (TrieNode* n : node->children)
		if (n) 
			destroy(n);
	delete node;
}


Trie::TrieNode* Trie::p_remove(TrieNode* node, const std::string& word, int depth = 0) {
    if (!node)
        return nullptr;
 
    // If last character of key is being processed
    if (depth == word.size()) {
 
        // This node is no more end of word after
        // removal of given key
        if (node->isWord)
            node->isWord = false;
 
        // If given is not prefix of any other word
        if (node->is_leaf()) {
            delete node;
            node = nullptr;
        }

		--sz;
        return node;
    }
 
    // If not last character, recur for the child obtained using ASCII value
    int index = word[depth] - 'a';
    node->children[index] = p_remove(node->children[index], word, depth + 1);
 
    // If node does not have any child (its only child got
    // deleted), and it is not end of another word.
    if (node->is_leaf() && node->isWord == false) {
        delete node;
        node = nullptr;
    }
 
    return node;	
}


void Trie::insert(const std::string& word) {
	if (search(word))  // To ensure the same word did not already exist in the Trie
		return;
	
	TrieNode* current = root;
	for (char c : word) {
		if (!current->children[c - 'a']) 
			current->children[c - 'a'] = new TrieNode{c};
		current = current->children[c - 'a'];
	}

	current->isWord = true;
	++sz;
}


void Trie::remove(const std::string& word) {
	if (!search(word))
		return;
	p_remove(root, word);
}


bool Trie::search(const std::string& word) {
	TrieNode* n = get_TrieNode(word);
	return n != nullptr && n->isWord;
}


bool Trie::starts_with(const std::string& word) {
	TrieNode* n = get_TrieNode(word);
	return n != nullptr;
}

}



