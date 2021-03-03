import heapq

class Minheap():
    """
    A wrapped interface for a heapq (used as a min heap).

    Attributes:
        min_heap: The min heap itself.
        key: Pre-processing function that will be used on each object whenever pushing/popping.
    """
    
    class _MinheapObject(object):
        def __init__(self, x, key):
            self.val = x
            self.key = key

        def __lt__(self, other):
            return self.key < other.key

        def __gt__(self, other):
            return self.key > other.key
        
        def __eq__(self, other):
            return self.key == other.key

        def __str__(self):
            return "({}, {})".format(self.val, self.key)

        def __repr__(self):
            return self.__str__()
    
    def __init__(self):
        self._minheap = []
        self._minheap_set = set()

    def push(self, x, key):
        """
        Pushes an item into the min heap.

        :param x: The item to be pushed in the min heap.
        :param key: The key associated with the item.
        """

        if x in self._minheap_set:
            return

        heap_elem = Minheap._MinheapObject(x, key)
        heapq.heappush(self._minheap, heap_elem)
        self._minheap_set.add(x)

    def pop(self):
        """
        Takes the top/root item of the min heap out,
        reducing the size of the queue by 1.

        Assumption(s):
            The min heap has at least one element.

        :return: The root of the min heap, popped out.
        """
        
        assert not self.empty()
        top_elem = heapq.heappop(self._minheap).val
        self._minheap_set.remove(top_elem)
        return top_elem

    def update_key(self, x, new_key):
                """
        Changes the key of an item to a new key.

        Assumption(s):
            The max heap has the item.

        :param x: The item to change its key.
        :param new_key: The new key to update for the item.
        """

        assert x in self._minheap_set

        for elem in self._minheap:
            if elem.val != x:
                continue
    
            elem.key = new_key
            heapq.heapify(self._minheap)

            break
    
    def root(self):
        """
        Returns the top/root item of the min heap, but 
        without popping it out of the min heap.

        Assumption(s):
            The min heap has at least one element.

        :return: The root of the min heap.
        """
        
        assert not self.empty()
        return self._minheap[0].val
    
    def empty(self):
        """
        Checks to see if the min heap is empty or not.

        :return: True if the min heap is empty.
        """
        
        return self.__len__() == 0

    def __len__(self):
        return len(self._minheap)
    
    def __str__(self):
        return "{}".format(self._minheap)

    def __repr__(self):
        return "Minheap: {}".format(self.__str__())

    def __in__(self, elem):
        return elem in self._minheap_set
