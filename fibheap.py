class FibonacciHeap:
    def __init__(self):
        self.min_node = None
        self.size = 0

    def push(self, elem, key=None):
        node = FibonacciHeap._Node(elem, key)

        if self.size == 0:
            self.min_node = node
        else:
            front = self.min_node.next
            self.min_node.next = node
            node.next = front
            front.prev = node
            node.prev = self.min_node

        self.size += 1

    def pop(self):
        return

    def decrease_key(self):
        return

    def root(self):
        return self.min_node

    def empty(self):
        return self.__len__() == 0

    def __len__(self):
        return self.size

    class _Node:
        def __init__(self, elem, key):
            self.elem = elem
            self.key = key
            self.prev = self
            self.next = self