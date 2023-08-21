export default class Heap {
	static #DEFAULT_TREE_LEVEL = 3;

	constructor(seq = []) {
		let n = seq.length;
		if (n === 0) {
			n = Heap.#DEFAULT_TREE_LEVEL;
		}

		const heapSize = Math.pow(2, Math.floor(Math.log2(n)) + 1) - 1;

		this.heap = [...Array(n)];
		seq.forEach((elem, idx) => {
			this.heap[idx] = elem;
		});

		this.size = seq.length;
	}

	#parentIdx(idx) {
		return Math.floor((idx - 1) / 2);
	}

	#leftChildIdx(idx) {
		return 2 * idx + 1;
	}
	
	#rightChildIdx(idx) {
		return 2 * idx + 2;
	}

	#getNextSiftDownIdx(parentIdx, childIdxs) {
		if (childIdxs.length === 1) {
			return childIdxs[0];
		}

		const vals = childIdxs.map(i => this.heap[i]);
		if (vals[0] > vals[1]) {
			return childIdxs[1];
		} else {
			return childIdxs[0];
		}
	}

	#siftDown(idx) {
		while (true) {
			const left = this.#leftChildIdx(idx);
			const right = this.#rightChildIdx(idx);

			const idxs = [left, right].filter(i => i < this.length);
			if (idxs.length === 0) {
				// leaf node
				break;
			} else {
				const nextIdx = this.#getNextSiftDownIdx(idx, idxs);
				[this.heap[idx], this.heap[nextIdx]] = [this.heap[nextIdx], this.heap[idx]];
				idx = nextIdx;
			}
		}
	}

	#siftUp(idx) {
		while (idx > 0) {
			const nextIdx = this.#parentIdx(idx);
			
			if (this.heap[nextIdx] < this.heap[idx]) {
				// heap property is satisfied
				break;
			} else {
				[this.heap[nextIdx], this.heap[idx]] = [this.heap[idx], this.heap[nextIdx]];
				idx = nextIdx;
			}
		}
	}

	push(elem) {
		if (this.length === this.heap.length) {
			// heap is full
			const newHeapLength = 2 * (this.length + 1) - 1;
			const newHeap = [...Array(newHeapLength)];

			this.heap.forEach((elem, idx) => {
				newHeap[idx] = elem;
			});

			this.heap = newHeap;
		}

		this.heap[this.length] = elem;	
		this.#siftUp(this.length);
		this.size++;
	}

	pop() {
		const topVal = this.top;
		const end = this.length - 1;

		[this.heap[0], this.heap[end]] = [this.heap[end], this.heap[0]];
		this.heap[end] = undefined;
		this.size--;

		this.#siftDown(0);
		return topVal;
	}

	get top() {
		return this.heap[0];
	}

	get length() {
		return this.size;
	}

	get empty() {
		return this.length === 0;
	}
};
