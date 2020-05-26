import Event from './Event';

class Month {
	constructor(year, month) {
		//use today's month by default
		if (!year || !month) {
			const today = new Date();
			
			this.year = today.getFullYear();
			this.month = today.getMonth() + 1;
		}
		else {		
			//validate input
			if (!Number.isInteger(year) || !Number.isInteger(month) || !(month >= 1 && month <= 12))
				throw new RangeError('Invalid year or month.');
			
			this.year = year;
			this.month = month;
		}
		
		//create day dictionary
		this.days = [null];
		for (let i = 0; i < this.daysInMonth; i++)
			this.days.push([]);
		
		Object.defineProperties(this, {
			year: { writable: false },
			month: { writable: false },
			days: { writable: false }
		});
	}
	
	static fromJSON(json) {
		let output = new Month(json.year, json.month);
		
		for (let i = 1; i < json.days.length; i++)
			for (let j = 0; j < json.days[i].length; j++) {
				const id = json.days[i][j].id;
				const description = json.days[i][j].description;
				
				output.days[i].push(new Event(id, new Date(0), description));
			}
		
		return output;
	}
	
	get daysInMonth() {
		//get the date of the zeroth day of the next month (0 indexed)
		return new Date(this.year, this.month, 0).getDate();
	}
	get startsOn() {
		//index days starting at Monday
		return (new Date(this.year, this.month - 1, 1).getDay() + 6) % 7;
	}
	
	formatName() {
		const MONTH_NAMES = [
			'January',
			'February',
			'March',
			'April',
			'May',
			'June',
			'July',
			'August',
			'September',
			'October',
			'November',
			'December'
		];
		
		return MONTH_NAMES[this.month - 1] + ' ' + this.year;
	}
	
	getPrev() {
		let month = this.month - 1;
		let year = this.year;
		
		if (month == 0) {
			month = 12;
			year--;
		}
		
		return new Month(year, month);
	}
	getNext() {
		let month = this.month + 1;
		let year = this.year;
		
		if (month == 13) {
			month = 1;
			year++;
		}
		
		return new Month(year, month);
	}
}

export default Month;
