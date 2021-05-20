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
            self.min_node = key

        self.size += 1

    def _insert(self, node, node_prev, root):
        front = root.next
        root.next = node
        node_prev.next = front
        front.prev = node_prev
        node.prev = root

    def _remove_node(self, node):
        front = node.next
        prev = node.prev
        prev.next = front
        front.prev = prev

    def _update_min_node(self):
        tail = self.head.prev
        new_min_node = self.head
        ptr = self.head

        while True:
            if ptr.key < new_min_node.key:
                new_min_node = ptr

            if ptr.next.elem == tail.elem:
                break

            ptr = ptr.next

        return new_min_node

    def _consolidate(self):
        return

    def pop(self):
        assert not self.empty()

        elem, key = self.root()
        if self.size == 1:
            self.min_node = None
            self.head = None
            return

        child = self.min_node.child
        if child is not None:
            self._insert(child, child.prev, self.head)

        if self.head.elem == self.min_node.elem:
            self.head = self.head.next

        self._remove_node(self.min_node)
        self.size -= 1

        self._update_min_node()
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