import React from 'react';

function CalendarDay(props) {
	//handle week-filler day
	if (!props.month || !props.date)
		return (<div className="day"></div>);
	
	//generate event list
	const events = props.month.days[props.date];
	const listItems = events.map(event => (<li key={event.id}>{event.description}</li>));
	
	//fire onPick event for the parent component to pass upwards
	function handleClick(e) {
		e.preventDefault();
		props.onPick(props.date);
	}
	
	return (
		<div className={events.length > 0 ? 'day busy' : 'day'}>
			<a href="#" onClick={handleClick}>
				<h2>{props.date}</h2>
				<ul>
					{listItems}
				</ul>
			</a>
		</div>
	);
}

export default CalendarDay;
