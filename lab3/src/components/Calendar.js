import React from 'react';
import MainWrapper from './MainWrapper';
import CalendarDay from './CalendarDay';
import Loader from './Loader';
import Month from '../datatypes/Month';

class Calendar extends React.Component {
	constructor(props) {
		super(props);
		
		this.state = {
			fetching: false,
			onDayPick: props.onDayPick
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
		this.fetchData();
	}
	
	showPrevMonth(e) {
		if (e) e.preventDefault();
		
		this.setState(state => ({ month: state.month.getPrev() }), this.fetchData);
	}
	showNextMonth(e) {
		if (e) e.preventDefault();
		
		this.setState(state => ({ month: state.month.getNext() }), this.fetchData);
	}
	fetchData() {
		this.setState({ fetching: true });
		
		fetch('/api/month/' + this.state.month.year + '-' + this.state.month.month)
			.then(response => response.json())
			.then(data => {
				this.setState({ month: Month.fromJSON(data), fetching: false });
			})
			.catch(error => {
				this.setState({ fetching: false });
				alert("Error loading calendar data!");
				console.error(error);
			});
	}
	
	handleDayPick(date) {
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
