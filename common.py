from minheap import Minheap
from maxheap import Maxheap
from math import sqrt, floor
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
    Generator that splits up the container into chunks of size n.
    The last chunk may have < n elements (i.e. n < len(xx) and len(xx) % n != 0)
    or it may just consist of the containter itself (i.e. n >= len(xx)).
    Assumption(s):
        xx is a container.
        n > 0

    :param n: The size of each chunk.
    :return:  A list containing the split chunks.
    """
    assert n > 0

    i, j = 0, n
    while i < len(xx):
        end = min(j, len(xx))
        yield xx[i:end]
        i += n
        j += n


def is_prime(x):
    """
    Checks to see if a number is prime.
    Uses a known fact that all primes are of the form 6k ± 1 (for some integer k).
    Assumption(s):
        x >= 0
    
    :param x: The number to be checked.
    :return:  Boolean value indicating whether the number is prime,
    """

    if x < 2:
        return False
    if x == 2 or x == 3:
        return True
    if x % 2 == 0 or x % 3 == 0:
        return False

    i, w = 5, 2
    while i**2 <= x:
        if x % i == 0:
            return False
        i += w
        w = 6 - w

    return True


def prime_factorization(n):
    """
    Computes the prime factorization of a number.
    Assumption(s):
        n >= 0

    :param n: The number to be factorized.
    :return:  A list containing the prime factors.
    """
    
    assert n >= 0
    
    factors = []
    if n <= 1:
        return factors
    
    while n % 2 == 0:
        factors.append(2)
        n = n // 2

    end = floor(sqrt(n))
    for factor in range(3, end+1, 2):
        while n % factor == 0:
            factors.append(factor)
            n = n // factor
    
    if n > 2:
        factors.append(n)

    return factors


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
    
    return kth_smallest_element(v, len(v) - k + 1)

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
    :return:  k-th smallest element of the list.
    """
    assert 1 <= k and k <= len(v)

    median = lambda xx: sorted(xx)[len(xx)//2]
    def partition(v, l, r, x):
        i = l
        while i < r:
            if v[i] == x:
                break
            i += 1
        v[i], v[r] = v[r], v[i]

        i = l
        for j in range(l, r):
            if v[j] <= x:
                v[i], v[j] = v[j], v[i]
                i += 1
    
        v[i], v[r] = v[r], v[i]
        return i

    l, r = 0, len(v) - 1
    while k > 0 and k <= r - l + 1:
        groups = chunks(v[l:r+1], 5)
        medians = [median(group) for group in groups]
        mom = median(medians)

        pos = partition(v, l, r, mom)
        numElements = pos - l + 1
        if numElements == k:
            return v[pos]
        elif numElements > k:
            r = pos - 1
        else:
            l, k = pos + 1, k - numElements


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
        # sorted array might be empty
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

