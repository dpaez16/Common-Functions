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

    def _is_singular_branch(self, root):
        if root is None: 
            return True
        if len(root.children) > 1: 
            return False

        for c in root.children:
            return self._is_singular_branch(root.children[c])

        return True

    def delete(self, word):
        if len(word) == 0:
            return

        curr = pivotNode = self.root
        pivotIdx = 0

        for i in range(len(word)):
            c = word[i]

            if c not in curr.children:
                return

            if len(curr.children) > 1:
                pivotNode = curr
                pivotIdx = i

            curr = curr.children[c]

        if not curr.is_terminal: 
            return

        curr.is_terminal = False
        if len(curr.children) > 0:
            return

        c = word[pivotIdx]
        if not self._is_singular_branch(pivotNode.children[c]):
            return

        del pivotNode.children[c]

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