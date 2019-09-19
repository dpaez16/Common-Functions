import heapq

class Minheap():
    """
    A wrapped interface for a heapq (used as a min heap).
    Attributes:
        min_heap: The min heap itself.
        key: Pre-processing function that will be used on each object whenever pushing/popping.
    """
    
    class _MinheapObject(object):
        def __init__(self, x, key=lambda x: x):
            self.val = x
            self._key = key

        def __lt__(self, other):
            return self._key(self.val) < self._key(other.val)

        def __gt__(self, other):
            return self._key(self.val) > self._key(other.val)
        
        def __eq__(self, other):
            return self._key(self.val) == self._key(other.val)

        def __str__(self):
            return str(self.val)

        def __repr__(self):
            return repr(self.val)
    
    def __init__(self, key=lambda x: x):
        self._minheap = []
        self._key = key

    def push(self, x):
        """
        Pushes an item into the min heap.

        :param x: The item to be pushed in the min heap.
        """
        
        heapq.heappush(self._minheap, Minheap._MinheapObject(x, self._key))

    def pop(self):
        """
        Takes the top/root item of the min heap out,
        reducing the size of the queue by 1.

        Assumption(s):
            The min heap has at least one element.

        :return: The root of the min heap, popped out.
        """
        
        assert not self.empty()
        return heapq.heappop(self._minheap).val
    
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
