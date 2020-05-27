import React from 'react';

class CalendarEvent extends React.Component {
	constructor(props) {
		super(props);
		
		this.state = {
			data: props.data
		};
	}
	
	render() {
		const displayTime = ("0" + this.state.data.date.getHours()).substr(-2) +
			':' +
			("0" + this.state.data.date.getMinutes()).substr(-2);
		
		return (
			<li>
				<h2>{displayTime}</h2>
				<p>{this.state.data.description}</p>
				<form class="actions">
					<a href="#" class="button">Edit</a>
					<button name="action" value="delete">Delete</button>
				</form>
			</li>
		);
	}
}

export default CalendarEvent;
