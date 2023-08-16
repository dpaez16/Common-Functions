export default class Stack {
	constructor() {
		this.stk = [];
	}

	push(item) {
		this.stk.push(item);
	}

	pop() {
		return this.stk.pop();
	}

	get length() {
		return this.stk.length;
	}

	get top() {
		return this.stk[this.length - 1];
	}

	get empty() {
		return this.length === 0;
	}
};
