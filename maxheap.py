import heapq

class Maxheap():
    """
    A wrapped interface for a heapq (used as a max heap).

    Attributes:
        max_heap: The max heap itself.
    """

    def __init__(self):
        self._maxheap = []

    def push(self, x):
        """
        Pushes an item into the max heap.

        :param x: The item to be pushed in the max heap.
        """

        heapq.heappush(self._maxheap, _MaxheapObject(x))

    def pop(self):
        """
        Takes the top/root item of the max heap out,
        reducing the size of the queue by 1.

        Assumption(s):
            The max heap has at least one element.

        :return: The root of the max heap, popped out.
        """
        
        assert not self.empty()
        return heapq.heappop(self._maxheap).val
    
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
    
class _MaxheapObject(object):
    def __init__(self, x):
        self.val = x

    def __lt__(self, other):
        return self.val > other.val
        
    def __eq__(self, other):
        return self.val == other.val

    def __str__(self):
        return str(self.val)

    def __repr__(self):
        return repr(self.val)

