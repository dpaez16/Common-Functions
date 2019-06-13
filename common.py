from minheap import Minheap
from maxheap import Maxheap
from functools import reduce
import operator

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


def chunks(xx, n):
    """
    Splits up the container into chunks of size n.
    The last chunk may have < n elements (i.e. n < len(xx) and len(xx) % n != 0)
    or it may just consist of the containter itself (i.e. n >= len(xx)).
    Assumption(s):
        xx is a container.
        n > 0

    :param n: The size of each chunk.
    :return:  A list containing the split chunks.
    """
    assert n > 0

    return [xx[i:i + n] for i in range(0, len(xx), n)]


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
    
    min_heap = Minheap()
    for idx in range(k):
        min_heap.push(v[idx])
    
    for idx in range(k, len(v)):
        root = min_heap.root()
        if v[idx] > root:
            min_heap.pop()
            min_heap.push(v[idx])

    k_largest_items = []
    while len(min_heap) != 0:
        root = min_heap.root()
        k_largest_items.append(root)
        min_heap.pop()

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
    
    min_heap = Minheap()
    for idx in range(k):
        min_heap.push(v[idx])
    
    for idx in range(k, len(v)):
        root = min_heap.root()
        if v[idx] > root:
            min_heap.pop()
            min_heap.push(v[idx])

    root = min_heap.root()
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
    
    max_heap = Maxheap()
    for idx in range(k):
        max_heap.push(v[idx])
    
    for idx in range(k, len(v)):
        root = max_heap.root()
        if v[idx] < root:
            max_heap.pop()
            max_heap.push(v[idx])

    k_smallest_items = []
    while len(max_heap) != 0:
        root = max_heap.root()
        k_smallest_items.append(root)
        max_heap.pop()

    k_smallest_items = k_smallest_items[::-1]
    return k_smallest_items


def kth_smallest_element(v, k):
    """
    Extracts the k-th smallest element of a list.
    
    Assumption(s):
        1 <= k <= len(v)
    
    :param v: List of elements.
    :param k: Index of the smallest element to extract.
    :return:  k-th smallesccccccjeirvdnjliitblinunnvtddrnlifrtrlbjujtu
    t element of the list.
    """
    
    assert 1 <= k and k <= len(v)
    
    max_heap = Maxheap()
    for idx in range(k):
        max_heap.push(v[idx])
    
    for idx in range(k, len(v)):
        root = max_heap.root()
        if v[idx] < root:
            max_heap.pop()
            max_heap.push(v[idx])
    
    root = max_heap.root()
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
    
    x1 = foldr(operator.__xor__, 0, A)
    x2 = foldr(operator.__xor__, 0, range(n + 1))

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
    min_heap, merged = Minheap(), []

    for idx in range(k):
        if len(A[idx]) != 0:
            min_heap.push((A[idx][0], idx, 0))

    if len(min_heap) == 0:
        return merged

    while min_heap.root()[0] != INFINITY:
        min_elem, row_idx, col_idx = min_heap.pop()
        merged.append(min_elem)
        
        val = INFINITY
        if col_idx != (len(A[row_idx]) - 1):
            val = A[row_idx][col_idx + 1]

        min_heap.push((val, row_idx, col_idx + 1))

    return merged

