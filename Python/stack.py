from collections import deque

class Stack():
    """
    A wrapped interface for a deque (used as a stack).
    Attributes:
        s: The stack itself.
    """
    
    def __init__(self):
        self._s = deque([])

    def push(self, x):
        """
        Pushes an item on the top of the stack.

        :param x: The item to be pushed on the stack.
        """
        
        self._s.append(x)

    def pop(self):
        """
        Takes the top item of the stack out,
        reducing the size of the stack by 1.

        Assumption(s):
            The stack has at least one element.

        :return: The front of the stack, popped out.
        """
        
        assert not self.empty()
        return self._s.pop()

    def top(self):
        """
        Returns the top of the stack, but 
        without popping it out of the stack.

        Assumption(s):
            The stack has at least one element.

        :return: The front of the stack.
        """
        
        assert not self.empty()
        return self._s[-1]

    def empty(self):
        """
        Checks to see if the stack is empty or not.

        :return: True if the stack is empty.
        """
        
        return self.__len__() == 0

    def __len__(self):
        return len(self._s)
    
    def __str__(self):
        return "{}".format(list(self._s))

    def __repr__(self):
        return "Stack: {}".format(self.__str__())
