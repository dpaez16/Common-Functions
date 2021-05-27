from collections import defaultdict


class FibonacciHeap:
    def __init__(self):
        self.min_node = None
        self.size = 0
        self.node_map = {}
        self.root_list = LL()

    def push(self, elem, key=None):
        assert elem is not None
        assert elem not in self

        self.size += 1
        new_node = self.root_list.push(elem, key)
        self.node_map[elem] = new_node
        
        if len(self) == 1:
            self.min_node = new_node
        else:
            self.min_node = min(self.min_node, new_node)

    def pop(self):
        assert not self.empty()

        elem = self.root()
        self.size -= 1
        del self.node_map[elem]

        node = self.root_list.remove(self.min_node)
        children = node.data.child_list

        ptr = children.head
        while ptr is not None:
            ptr.data.parent = None
            ptr = ptr.next

        self.root_list.merge(children)
        self._consolidate()
        self.min_node = self.root_list.min()

        return elem

    def _consolidate(self):
        rank_map = defaultdict(lambda: None)

        ptr = self.root_list.head
        while ptr is not None:
            ptr_rank = ptr.data.rank()

            if rank_map[ptr_rank] is None:
                rank_map[ptr_rank] = ptr
                ptr = ptr.next
            elif rank_map[ptr_rank] == ptr:
                ptr = ptr.next
            else:
                other_node = rank_map[ptr_rank]
                main_node, child_node = self._get_nodes(ptr, other_node)
                self.root_list.remove(child_node)
                child_node.data.parent = main_node
                main_node.data.child_list.push_node(child_node)

                rank_map[ptr_rank] = None
                ptr = self.root_list.head

        return

    def _get_nodes(self, ptr, other_node):
        if ptr.data.key <= other_node.data.key:
            return ptr, other_node
        else:
            return other_node, ptr

    def _cut(self, node):
        assert isinstance(node, LL._LL_Node)

        parent = node.data.parent
        if parent is None:
            parent.data.unmark()
            return

        parent.data.child_list.remove(node)
        node.data.parent = None
        self.root_list.push_node(node)
        node.data.unmark()

    def decrease_key(self, elem, new_key):
        assert elem in self

        node = self.node_map[elem]
        if new_key == node.data.key:
            return
        
        assert new_key < node.data.key

        node.data.key = new_key
        parent = node.data.parent
        if parent is None:
            self.min_node = min(self.min_node, node)
            return

        if parent.data.key <= new_key:
            return

        self._cut(node)
        self.min_node = min(self.min_node, node)

        node = parent
        parent = parent.data.parent
        while parent is not None:
            if not node.data.is_marked():
                node.data.mark()
                break
            else:
                self._cut(node)

                node = parent
                parent = parent.data.parent

    def root(self):
        assert not self.empty()
        return self.min_node.data.elem

    def empty(self):
        return self.__len__() == 0

    def __len__(self):
        return self.size

    def __contains__(self, elem):
        return elem in self.node_map

class LL:
    def __init__(self):
        self.head = None
        self.tail = None
        self.size = 0

    def push(self, elem, key=None):
        data = FibNode(elem, key)
        node = LL._LL_Node(data)

        if len(self) == 0:
            self.head = node
            self.tail = node
        else:
            self.tail.next = node
            node.prev = self.tail
            self.tail = node

        self.size += 1
        return node

    def push_node(self, node):
        assert isinstance(node, LL._LL_Node)

        if len(self) == 0:
            self.head = node
            self.tail = node
        else:
            self.tail.next = node
            node.prev = self.tail
            self.tail = node

        self.size += 1
        return node

    def pop(self):
        assert len(self) > 0

        node = self.tail

        if len(self) == 1:
            self.head = None
            self.tail = None
        else:
            self.tail = self.tail.prev
            self.tail.next = None

        self.size -= 1
        node.prev = None
        node.next = None
        return node

    def remove(self, node):
        assert isinstance(node, LL._LL_Node)

        if node == self.tail:
            return self.pop()

        self.size -= 1

        if node == self.head:
            self.head = self.head.next
            self.head.prev = None

            node.prev = None
            node.next = None
            return node

        front = node.next
        back = node.prev

        back.next = front
        front.prev = back

        node.next = None
        node.prev = None
        return node

    def merge(self, other):
        assert isinstance(other, LL)

        if len(self) == 0:
            self.head = other.head
            self.tail = other.tail
            self.size = len(other)
        else:
            self.size += len(other)
            self.tail.next = other.head

            if other.head is not None:
                other.head.prev = self.tail

            if other.tail is not None:
                self.tail = other.tail

    def min(self):
        min_node = self.head
        ptr = self.head

        while ptr is not None:
            min_node = min(min_node, ptr)
            ptr = ptr.next

        return min_node

    def __len__(self):
        return self.size

    class _LL_Node:
        def __init__(self, data):
            self.next = None
            self.prev = None
            self.data = data

        def __lt__(self, other):
            return self.data < other.data

        def __gt__(self, other):
            return self.data > other.data

        def __eq__(self, other):
            return self.data == other.data

class FibNode:
    def __init__(self, elem, key):
        self.elem = elem
        self.key = key if key is not None else elem
        self.parent = None
        self.child_list = LL()
        self.marked = False

    def rank(self):
        return len(self.child_list)

    def mark(self):
        self.marked = True

    def unmark(self):
        self.marked = False

    def is_marked(self):
        return self.marked

    def __lt__(self, other):
        return self.key < other.key

    def __gt__(self, other):
        return self.key > other.key

    def __eq__(self, other):
        return self.key == other.key and self.elem == other.elem
