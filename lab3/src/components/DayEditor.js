import React from 'react';
import MainWrapper from './MainWrapper';
import CalendarEvent from './CalendarEvent';
import Loader from './Loader';
import Event from '../datatypes/Event';

class DayEditor extends React.Component {
	constructor(props) {
		super(props);
		
		this.state = {
			day: props.day,
			dayEvents: [],
			onClose: props.onClose,
			onEditEvent: props.onEditEvent,
			fetching: false
		};
		
		this.close = this.close.bind(this);
		this.addEvent = this.addEvent.bind(this);
		this.editEvent = this.editEvent.bind(this);
		this.eraseEvent = this.eraseEvent.bind(this);
	}
	
	componentDidMount() {
		this.fetchData();
	}
	
	close(e) {
		if (e) e.preventDefault();
		if (!this.state.onClose) return;
		
		this.state.onClose(this.state.day.getFullYear(), this.state.day.getMonth() + 1);
	}
	addEvent(e) {
		if (e) e.preventDefault();
		if (this.state.onEditEvent) this.state.onEditEvent(null, this.state.day);
	}
	editEvent(id) {
		if (this.state.onEditEvent) this.state.onEditEvent(id, this.state.day);
	}
	eraseEvent(id) {
		this.setState(state => ({
			dayEvents: this.state.dayEvents.filter(ev => ev.id != id)
		}));
	}
	fetchData() {
		const year = this.state.day.getFullYear();
		const month = this.state.day.getMonth() + 1;
		const date = this.state.day.getDate();
		
		this.setState({ fetching: true });
		
		fetch('/api/date/' + year + '-' + month + '-' + date)
			.then(response => response.json())
			.then(data => {
				const dayEvents = [];
				
				for (let i = 0; i < data.events.length; i++)
					dayEvents.push(Event.fromJSON(data.events[i]));
				
				this.setState({ dayEvents });
			})
			.catch(error => {
				alert('Error loading day data!');
				console.error(error);
			})
			.finally(() => {
				this.setState({ fetching: false });
			});
	}
	
	render() {
		const displayDate = this.state.day.getFullYear() +
			'-' +
			('0' + (this.state.day.getMonth() + 1)).substr(-2) +
			'-' +
			('0' + this.state.day.getDate()).substr(-2);
		const eventItems = this.state.dayEvents.map(ev => (
			<CalendarEvent data={ev} key={ev.id} onEdit={this.editEvent} onRemove={this.eraseEvent}/>
		));
		
		return (
			<MainWrapper compact>
				{this.state.fetching && <Loader/>}
				<header>
					<h1>{displayDate}</h1>
				</header>
				<main>
					<ul className="events">
						{eventItems}
					</ul>
					<div className="finalbuttons">
						<a href="#" className="button large" onClick={this.close}>Back</a>
						<a href="#" className="button large" onClick={this.addEvent}>Add new</a>
					</div>
				</main>
			</MainWrapper>
		);
	}
}

export default DayEditor;
