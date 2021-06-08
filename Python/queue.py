from collections import deque

class Queue():
    """
    A wrapped interface for a deque (used as a queue).
    Attributes:
        q: The queue itself.
    """

    def __init__(self):
        self._q = deque()

    def push(self, x):
        """
        Pushes an item into the end of the queue.

        :param x: The item to be pushed in the queue.
        """

        self._q.append(x)

    def pop(self):
        """
        Takes the front item of the queue out,
        reducing the size of the queue by 1.

        Assumption(s):
            The queue has at least one element.

        :return: The front of the queue, popped out.
        """

        assert not self.empty()
        return self._q.popleft()

    def front(self):
        """
        Returns the front of the queue, but 
        without popping it out of the queue.

        Assumption(s):
            The queue has at least one element.

        :return: The front of the queue.
        """

        assert not self.empty()
        return self._q[0]

    def empty(self):
        """
        Checks to see if the queue is empty or not.

        :return: True if the queue is empty.
        """

        return self.__len__() == 0

    def __len__(self):
        return len(self._q)
    
    def __str__(self):
        return "{}".format(list(self._q))

    def __repr__(self):
        return "Queue: {}".format(self.__str__())
