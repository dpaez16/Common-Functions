class Trie:
    def __init__(self):
        self.root = Trie._TrieNode()

    def insert(self, word):
        curr = self.root
        for i in range(len(word)):
            c = word[i]

            if c not in curr.children:
                new_node = Trie._TrieNode()
                new_node.is_terminal = i + 1 == len(word)
                curr.children[c] = new_node

            curr = curr.children[c]

    def delete(self, word):
        return

    def find(self, word):
        curr = self.root

        for i in range(len(word)):
            c = word[i]

            if c not in curr.children:
                return False

            curr = curr.children[c]

        return curr.is_terminal

    class _TrieNode:
        def __init__(self):
            self.children = {}
            self.is_terminal = False