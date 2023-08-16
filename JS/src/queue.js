export default class Queue {
	constructor() {
		this.q = {};
		this.head = 0;
		this.tail = 0;
	}

	push(item) {
		this.q[this.tail++] = item;
	}

	pop() {
		const item = this.q[this.head];
		delete this.q[this.head];
		this.head++;
		return item;
	}

	get front() {
		return this.q[this.head];
	}

	get length() {
		return this.tail - this.head;
	}

	get empty() {
		return this.length === 0;
	}
};
