export default class Deque {
	constructor() {
		this.q = {};
		this.head = 0;
		this.tail = 0;
	}

	pushBack(item) {
		this.q[this.tail++] = item;
	}

	pushFront(item) {
		this.q[this.head--] = item;
	}

	popFront() {
		const item = this.q[this.head];
		delete this.q[this.head];
		this.head++;
		return item;
	}

	popBack() {
		const item = this.q[this.tail];
		delete this.q[this.tail];
		this.tail--;
		return item;
	}

	get front() {
		return this.q[this.head];
	}

	get back() {
		return this.q[this.tail];
	}

	get length() {
		return this.tail - this.head;
	}

	get empty() {
		return this.length === 0;
	}
};
