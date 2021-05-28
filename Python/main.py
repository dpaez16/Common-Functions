from common import kth_smallest_element


def main():
    arr = [5, 9, 3, 1, -3, 10, 20]
    elem = kth_smallest_element(arr, 2)
    assert elem == 1

    return

if __name__ == "__main__":
    main()
