import React from 'react';

function CalendarDay(props) {
	//handle week-filler day
	if (!props.month || !props.date)
		return (<div className="day"></div>);
	
	//generate event list
	const events = props.month.days[props.date];
	const listItems = events.map(event => (<li key={event.id}>{event.description}</li>));
	
	return (
		<div className={events.length > 0 ? 'day busy' : 'day'}>
			<a href="#">
				<h2>{props.date}</h2>
				<ul>
					{listItems}
				</ul>
			</a>
		</div>
	);
}

export default CalendarDay;
