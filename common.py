import heapq    # min-heap by default, negate values for max-heap
from functools import reduce

INFINITY = float("inf")

def takeWhile(f, xx):
    """
    takeWhile from Haskell.
    
    :param f:  Function to use over the list.
    :param xx: List to be taken from.
    :return:   Prefix of the list that satisfies the predicate, f.
    """

    idx = 0
    while f(xx[idx]):
        idx += 1

    return xx[:idx]


def dropWhile(f, xx):
    """
    dropWhile from Haskell.
    
    :param f:  Function to use over the list.
    :param xx: List to be dropped from.
    :return:   The list without the prefix that 
               satisfies the predicate, f.
    """
    
    idx = 0
    while f(xx[idx]):
        idx += 1

    return xx[idx:]


def foldl(f, z, xx):
    """
    foldl from Haskell.
    
    :param f:  Function to fold over the list.
    :param z:  Starting value to fold over.
    :param xx: List that will be folded over.
    :return:   Value of the fold.
    """
    
    return reduce(lambda x, y: f(x, y), xx, z)


def foldr(f, z, xx):
    """
    foldr from Haskell.
    
    :param f:  Function to fold over the list.
    :param z:  Starting value to fold over.
    :param xx: List that will be folded over.
    :return:   Value of the fold.
    """
    
    return reduce(lambda x, y: f(y, x), xx, z)


def k_largest_elements(v, k):
    """
    Extracts the k largest elements of a list.
    They are put in descending order.
    
    Assumption(s):
        1 <= k <= len(v)
    
    :param v: List of elements.
    :param k: Number of largest elements to extract.
    :return:  A list of the k largest elements.
    """
    
    assert 1 <= k and k <= len(v)
    
    min_heap = []
    for idx in range(k):
        heapq.heappush(min_heap, v[idx])
    
    for idx in range(k, len(v)):
        root = min_heap[0]
        if v[idx] > root:
            heapq.heappop(min_heap)
            heapq.heappush(min_heap, v[idx])

    k_largest_items = []
    while len(min_heap) != 0:
        root = min_heap[0]
        k_largest_items.append(root)
        heapq.heappop(min_heap)

    k_largest_items = k_largest_items[::-1]
    return k_largest_items


def kth_largest_element(v, k):
    """
    Extracts the k-th largest element of a list.
    
    Assumption(s):
        1 <= k <= len(v)
    
    :param v: List of elements.
    :param k: Index of the largest element to extract.
    :return:  k-th largest element of the list.
    """
    
    assert 1 <= k and k <= len(v)
    
    min_heap = []
    for idx in range(k):
        heapq.heappush(min_heap, v[idx])
    
    for idx in range(k, len(v)):
        root = min_heap[0]
        if v[idx] > root:
            heapq.heappop(min_heap)
            heapq.heappush(min_heap, v[idx])

    root = min_heap[0]
    return root


def k_smallest_elements(v, k):
    """
    Extracts the k largest elements of a list.
    They are put in ascending order.
    
    Assumption(s):
        1 <= k <= len(v)
    
    :param v: List of elements.
    :param k: Number of smallest elements to extract.
    :return:  A list of the k smallest elements.
    """
    
    assert 1 <= k and k <= len(v)
    
    max_heap = []
    for idx in range(k):
        heapq.heappush(max_heap, -v[idx])
    
    for idx in range(k, len(v)):
        root = -max_heap[0]
        if v[idx] < root:
            heapq.heappop(max_heap)
            heapq.heappush(max_heap, -v[idx])

    k_smallest_items = []
    while len(max_heap) != 0:
        root = -max_heap[0]
        k_smallest_items.append(root)
        heapq.heappop(max_heap)

    k_smallest_items = k_smallest_items[::-1]
    return k_smallest_items


def kth_smallest_element(v, k):
    """
    Extracts the k-th smallest element of a list.
    
    Assumption(s):
        1 <= k <= len(v)
    
    :param v: List of elements.
    :param k: Index of the smallest element to extract.
    :return:  k-th smallest element of the list.
    """
    
    assert 1 <= k and k <= len(v)
    
    max_heap = []
    for idx in range(k):
        heapq.heappush(max_heap, -v[idx])
    
    for idx in range(k, len(v)):
        if v[idx] < -max_heap[0]:
            heapq.heappop(max_heap)
            heapq.heappush(max_heap, -v[idx])
    
    root = -max_heap[0]
    return root


def find_missing_element(A, n):
    """
    Find the missing element of an array of size n-1.
    The numbers in the array fall in [1, n].
    All numbers are distinct.
    Ex.
    A = [1, 2, 4], n = 4  ==> 3

    :param A: The array.
    :param n: The upper bound for the range of numbers.
    """
    
    x1 = reduce(lambda x, y: x ^ y, A, 0)
    x2 = reduce(lambda x, y: x ^ y, range(n + 1), 0)

    msg = x1 ^ x2

    return msg


def two_sum(A, s):
    """
    Finds all possible pairs of integers (x, y) in A
    such that they sum to s.
    
    :param A: The array to check.
    :param s: The sum.
    """

    sums = []
    y = {}

    for x in A:
        diff = s - x

        if diff in y:
            sums.append((x, diff))

        y[x] = diff

    return sums


def merge_k_sorted_arrays(A):
    """
    Merged k sorted arrays in A.

    :param A: An array of k sorted arrays.
    """

    k = len(A)
    min_heap, merged = [], []

    for idx in range(k):
        if len(A[idx]) != 0:
            heapq.heappush(min_heap, (A[idx][0], idx, 0))

    if len(min_heap) == 0:
        return merged

    while min_heap[0][0] != 1e99:
        min_elem, row_idx, col_idx = heapq.heappop(min_heap)
        merged.append(min_elem)
        
        val = 1e99
        if col_idx != (len(A[row_idx]) - 1):
            val = A[row_idx][col_idx + 1]

        heapq.heappush(min_heap, (val, row_idx, col_idx + 1))

    return merged

