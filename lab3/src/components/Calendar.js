import React from 'react';
import CalendarDay from './CalendarDay';
import Month from '../datatypes/Month';

class Calendar extends React.Component {
	constructor(props) {
		super(props);
		
		this.state = {};
		
		//if no year and month specified, display current month
		if (props.year || props.month)
			this.state.month = new Month(props.year, props.month);
		else
			this.state.month = new Month();
	}
	
	render() {
		//compute collection of filler components
		let fillerDays = [];
		for (let i = 0; i < this.state.month.startsOn; i++)
			fillerDays.push(<CalendarDay key={i}/>);
		
		//compute collection of day components
		let days = [];
		for (let i = 1; i <= this.state.month.daysInMonth; i++)
			days.push(<CalendarDay month={this.state.month} date={i} key={i}/>);
		
		return (
			<>
				<header>
					<h1>{this.state.month.formatName()}</h1>
				</header>
				<main className="calendar">
						<a className="navigation previous" href="#">
							<img className="default" src="left.svg"/>
							<img className="alternative" src="up.svg"/>
							<img className="alternative2" src="leftbright.svg"/>
						</a>
						<a className="navigation next" href="#">
							<img className="default" src="right.svg"/>
							<img className="alternative" src="down.svg"/>
							<img className="alternative2" src="rightbright.svg"/>
						</a>
						
						{fillerDays}
						{days}
				</main>
			</>
		);
	}
}

export default Calendar;
