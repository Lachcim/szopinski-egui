import React from 'react';
import MainWrapper from './MainWrapper';
import Loader from './Loader';
import Event from '../datatypes/Event';

class EventEditor extends React.Component {
	constructor(props) {
		super(props);
		
		this.state = {
			addingMode: !props.id,
			eventId: props.id,
			event: null,
			defaultDay: props.day,
			onClose: props.onClose,
			fetching: false
		};
		
		this.timeInput = React.createRef();
		this.descriptionInput = React.createRef();
		
		this.apply = this.apply.bind(this);
		this.discard = this.discard.bind(this);
	}
	
	componentDidMount() {
		if (!this.state.addingMode) this.fetchData();
	}
	
	apply(e) {
		if (e) e.preventDefault();
		
		let url = '/api/event/' + this.state.eventId;
		if (this.state.addingMode) {
			url = 'api/date/' +
				this.state.defaultDay.getFullYear() +
				'-' +
				(this.state.defaultDay.getMonth() + 1) +
				'-' +
				this.state.defaultDay.getDate();
		}
		
		this.setState({ fetching: true });
		
		fetch(url, {
			method: 'post',
			headers: {'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8'},
			body: "id=" + this.state.eventId +
				"&time=" + this.timeInput.current.value +
				"&description=" + encodeURIComponent(this.descriptionInput.current.value)
			})
			.then(response => response.json())
			.then((x) => {
				this.discard();
			})
			.catch(error => {
				this.setState({ fetching: false });
				alert('Error saving event data!');
				console.error(error);
			});
	}
	discard(e) {
		if (e) e.preventDefault();
		if (this.state.onClose) this.state.onClose(this.state.defaultDay);
	}
	fetchData() {
		this.setState({ fetching: true });
		
		fetch('/api/event/' + this.state.eventId)
			.then(response => response.json())
			.then(data => {
				this.setState({ event: Event.fromJSON(data) });
			})
			.catch(error => {
				alert('Error loading event data!');
				console.error(error);
			})
			.finally(() => {
				this.setState({ fetching: false });
			});
	}
	
	render() {
		let defaultDesc = '';
		let defaultTime = '';
		
		if (this.state.event) {
			defaultDesc = this.state.event.description;
			defaultTime = ('0' + this.state.event.date.getHours()).substr(-2) + 
				':' +
				('0' + this.state.event.date.getMinutes()).substr(-2);
		}
		else if (this.state.addingMode) {
			defaultTime = '12:00';
		}
		
		return (
			<MainWrapper compact>
				{this.state.fetching && <Loader/>}
				<header>
					<h1>{this.state.addingMode ? 'Add event' : 'Edit event'}</h1>
				</header>
				<main className="inputs">
					<form onSubmit={this.apply}>
						<div className="inputholder">
							<h2>Time</h2>
							<input type="time" ref={this.timeInput} defaultValue={defaultTime}/>
							<h2>Description</h2>
							<input type="text" ref={this.descriptionInput} autoComplete="off" defaultValue={defaultDesc}/>
						</div>
						<div className="finalbuttons">
							<a href="#" className="button large" onClick={this.discard}>Cancel</a>
							<button className="large">Apply</button>
						</div>
					</form>
				</main>
			</MainWrapper>
		);
	}
}

export default EventEditor;
