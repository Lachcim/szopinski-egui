class Event {
	constructor(id, date, description) {
		//validate id
		if (id !== null) {
			if (!Number.isInteger(id) || id < 0)
				throw new RangeError("Invalid id.");
		}
		
		this.id = id;
		this.date = date;
		this.description = description;
		
		Object.defineProperty(this, "id", { writable: false });
	}
	
	get date() {
		return this._date;
	}
	
	set date(value) {
		if (!(value instanceof Date))
			throw new RangeError("Invalid date.");
		
		this._date = value;
	}
	
	get description() {
		return this._description;
	}
	
	set description(value) {
		if (typeof value != "string")
			throw new RangeError("Invalid description.");
		
		if (value.includes("\n"))
			throw new RangeError("Description cannot be multi-line.");
		
		this._description = value;
	}
}

export default Event;
