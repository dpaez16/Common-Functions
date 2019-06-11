import heapq

class Minheap():
    """
    A wrapped interface for a heapq (used as a min heap).
    Attributes:
        min_heap: The min heap itself.
    """
    
    def __init__(self):
        self._minheap = []

    def push(self, x):
        """
        Pushes an item into the min heap.

        :param x: The item to be pushed in the min heap.
        """
        
        heapq.heappush(self._minheap, x)

    def pop(self):
        """
        Takes the top/root item of the min heap out,
        reducing the size of the queue by 1.

        Assumption(s):
            The min heap has at least one element.

        :return: The root of the min heap, popped out.
        """
        
        assert not self.empty()
        return heapq.heappop(self._minheap)
    
    def root(self):
        """
        Returns the top/root item of the min heap, but 
        without popping it out of the min heap.

        Assumption(s):
            The min heap has at least one element.

        :return: The root of the min heap.
        """
        
        assert not self.empty()
        return self._minheap[0]
    
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
