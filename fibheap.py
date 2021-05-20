class FibonacciHeap:
    def __init__(self):
        self.min_node = None
        self.head = None
        self.size = 0

    def push(self, elem, key=None):
        assert elem is not None
        
        node = FibonacciHeap._Node(elem, key)

        if self.size == 0:
            self.min_node = node
            self.head = node
        else:
            self._insert(node, node.prev, self.head)

        if node.key < self.min_node.key:
            self.min_node = node

        self.size += 1

    def _insert(self, node, node_prev, root):
        front = root.next
        root.next = node
        node_prev.next = front
        front.prev = node_prev
        node.prev = root

    def _remove_node(self, node):
        if self.head.elem == node.elem:
            self.head = node.next

        front = node.next
        prev = node.prev
        prev.next = front
        front.prev = prev

        node.prev = None
        node.next = None

    def _update_min_node(self):
        new_min_node = self.head
        ptr = self.head

        while True:
            if ptr.key < new_min_node.key:
                new_min_node = ptr

            ptr = ptr.next

            if ptr.elem == self.head.elem:
                break

        return new_min_node

    def _merge(self, child, root):
        self._remove_node(child)
        if root.rank == 0:
            root.child = child
        else:
            self._insert(child, child.prev, root.child)

        root.rank += 1

    def _consolidate(self):
        if self.size == 1:
            return

        rank_map = [None] * 4
        curr = self.head
        i = 0

        while i < 4:
            rank = curr.rank
            if rank_map[rank] is None:
                rank_map[rank] = curr
                i += 1
            elif rank_map[rank].elem == curr.elem:
                i += 1
            else:
                other_node = rank_map[rank]
                child = other_node if curr.key < other_node.key else curr
                root = curr if curr.key < other_node.key else other_node
                self._merge(child, root)
                rank_map[rank] = None
                curr = root
                i = 0

        return

    def pop(self):
        assert not self.empty()

        elem, key = self.root()
        self.size -= 1

        if self.size == 0:
            self.min_node = None
            self.head = None
            return

        child = self.min_node.child
        if child is not None:
            self._insert(child, child.prev, self.head)

        if self.head.elem == self.min_node.elem:
            self.head = self.head.next

        self._remove_node(self.min_node)
        self.min_node = self._update_min_node()
        self._consolidate()

        return elem, key

    def decrease_key(self):
        return

    def root(self):
        assert not self.empty()
        elem, key = self.min_node.elem, self.min_node.key
        return elem, key

    def empty(self):
        return self.__len__() == 0

    def __len__(self):
        return self.size

    class _Node:
        def __init__(self, elem, key):
            self.elem = elem
            self.key = key if key is not None else elem
            self.prev = self
            self.next = self
            self.child = None
            self.rank = 0