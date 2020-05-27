import React from 'react';
import MainWrapper from './MainWrapper';
import CalendarDay from './CalendarDay';
import Loader from './Loader';
import Month from '../datatypes/Month';

class Calendar extends React.Component {
	constructor(props) {
		super(props);
		
		this.state = {
			onDayPick: props.onDayPick,
			fetching: false
		};
		
		//if no year and month specified, display current month
		if (props.year || props.month)
			this.state.month = new Month(props.year, props.month);
		else
			this.state.month = new Month();
		
		this.showPrevMonth = this.showPrevMonth.bind(this);
		this.showNextMonth = this.showNextMonth.bind(this);
		this.handleDayPick = this.handleDayPick.bind(this);
		this.fetchData = this.fetchData.bind(this);
	}
	
	componentDidMount() {
		//fetch data for the current month
		this.fetchData();
	}
	
	showPrevMonth(e) {
		if (e) e.preventDefault();
		
		//decrement displayed month, fetch data for it
		this.setState(state => ({ month: state.month.getPrev() }), this.fetchData);
	}
	showNextMonth(e) {
		if (e) e.preventDefault();
		
		//increment displayed month, fetch data for it
		this.setState(state => ({ month: state.month.getNext() }), this.fetchData);
	}
	fetchData() {
		//display loader
		this.setState({ fetching: true });
		
		//make API call
		fetch('/api/month/' + this.state.month.year + '-' + this.state.month.month)
			.then(response => response.json())
			.then(data => {
				//obtain month data from API
				this.setState({ month: Month.fromJSON(data) });
			})
			.catch(error => {
				//display error message
				alert('Error loading calendar data!');
				console.error(error);
			})
			.finally(() => {
				//hide loader
				this.setState({ fetching: false });
			});
	}
	
	handleDayPick(date) {
		//fire onDayPick event for the parent component to display the day editor
		if (!this.state.onDayPick) return;
		
		const fullDate = new Date(this.state.month.year, this.state.month.month - 1, date);
		this.state.onDayPick(fullDate);
	}
	
	render() {
		//compute collection of filler components
		let fillerDays = [];
		for (let i = 0; i < this.state.month.startsOn; i++)
			fillerDays.push(<CalendarDay key={i}/>);
		
		//compute collection of day components
		let days = [];
		for (let i = 1; i <= this.state.month.daysInMonth; i++)
			days.push(<CalendarDay month={this.state.month} date={i} key={i} onPick={this.handleDayPick}/>);
		
		return (
			<MainWrapper>
				{this.state.fetching && <Loader/>}
				<header>
					<h1>{this.state.month.formatName()}</h1>
				</header>
				<main className="calendar">
						<a className="navigation previous" href="#" onClick={this.showPrevMonth}>
							<img className="default" src="left.svg"/>
							<img className="alternative" src="up.svg"/>
							<img className="alternative2" src="leftbright.svg"/>
						</a>
						<a className="navigation next" href="#" onClick={this.showNextMonth}>
							<img className="default" src="right.svg"/>
							<img className="alternative" src="down.svg"/>
							<img className="alternative2" src="rightbright.svg"/>
						</a>
						
						{fillerDays}
						{days}
				</main>
			</MainWrapper>
		);
	}
}

export default Calendar;
