import heapq

class Maxheap():
    """
    A wrapped interface for a heapq (used as a max heap).

    Attributes:
        max_heap: The max heap itself.
        key: Pre-processing function that will be used on each object whenever pushing/popping.
    """
    
    class _MaxheapObject(object):
        def __init__(self, x, key):
            self.val = x
            self.key = key

        def __lt__(self, other):
            return self.key > other.key
        
        def __gt__(self, other):
            return self.key < other.key
        
        def __eq__(self, other):
            return self.key == other.key

        def __str__(self):
            return "({}, {})".format(self.val, self.key)

        def __repr__(self):
            return self.__str__()

    def __init__(self):
        self._maxheap = []
        self._maxheap_set = set()

    def push(self, x, key):
        """
        Pushes an item into the max heap.

        :param x: The item to be pushed in the max heap.
        :param key: The key associated with the item.
        """

        if x in self._maxheap_set:
            return

        heap_elem = Maxheap._MaxheapObject(x, key)
        heapq.heappush(self._maxheap, heap_elem)
        self._maxheap_set.add(x)

    def pop(self):
        """
        Takes the top/root item of the max heap out,
        reducing the size of the queue by 1.

        Assumption(s):
            The max heap has at least one element.

        :return: The root of the max heap, popped out.
        """
        
        assert not self.empty()
        top_elem = heapq.heappop(self._maxheap).val
        self._maxheap_set.remove(top_elem)
        return top_elem

    def update_key(self, x, new_key):
        """
        Changes the key of an item to a new key.

        Assumption(s):
            The max heap has the item.

        :param x: The item to change its key.
        :param new_key: The new key to update for the item.
        """

        assert x in self._maxheap_set

        for elem in self._maxheap:
            if elem.val != x:
                continue
    
            elem.key = new_key
            heapq.heapify(self._maxheap)

            break
    
    def root(self):
        """
        Returns the top/root item of the max heap, but 
        without popping it out of the max heap.

        Assumption(s):
            The max heap has at least one element.

        :return: The root of the max heap.
        """
        
        assert not self.empty()
        return self._maxheap[0].val
    
    def empty(self):
        """
        Checks to see if the max heap is empty or not.

        :return: True if the max heap is empty.
        """
        
        return self.__len__() == 0

    def __len__(self):
        return len(self._maxheap)
    
    def __str__(self):
        return "{}".format([x for x in self._maxheap])

    def __repr__(self):
        return "Maxheap: {}".format(self.__str__())
