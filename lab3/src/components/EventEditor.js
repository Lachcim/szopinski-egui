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
			defaultDay: props.day, //date before the event data is loaded
			onClose: props.onClose,
			fetching: false
		};
		
		//obtain references to form inputs
		this.timeInput = React.createRef();
		this.descriptionInput = React.createRef();
		
		this.apply = this.apply.bind(this);
		this.discard = this.discard.bind(this);
	}
	
	componentDidMount() {
		//fetch event data unless the event is being created
		if (!this.state.addingMode) this.fetchData();
	}
	
	apply(e) {
		//call to API to create or edit existing event
		//fire onClose for the parent container to display the day editor on success
		if (e) e.preventDefault();
		
		//format the url depending on whether the event is being created or edited
		let url = '/api/event/' + this.state.eventId;
		if (this.state.addingMode) {
			url = 'api/date/' +
				this.state.defaultDay.getFullYear() +
				'-' +
				(this.state.defaultDay.getMonth() + 1) +
				'-' +
				this.state.defaultDay.getDate();
		}
		
		//display the loader
		this.setState({ fetching: true });
		
		//make API call
		fetch(url, {
			method: 'post',
			headers: {'Content-Type': 'application/x-www-form-urlencoded; charset=UTF-8'},
			body: 'id=' + this.state.eventId +
				'&time=' + encodeURIComponent(this.timeInput.current.value) +
				'&description=' + encodeURIComponent(this.descriptionInput.current.value)
			})
			.then(response => response.json())
			.then(() => {
				//on success, fire onClose and await unmounting
				this.discard();
			})
			.catch(error => {
				//on failure, hide loader and show error message
				this.setState({ fetching: false });
				alert('Error saving event data!');
				console.error(error);
			});
	}
	discard(e) {
		//fire onClose for the parent container to display the day editor
		if (e) e.preventDefault();
		if (this.state.onClose) this.state.onClose(this.state.defaultDay);
	}
	fetchData() {
		//show loader
		this.setState({ fetching: true });
		
		//make API call
		fetch('/api/event/' + this.state.eventId)
			.then(response => response.json())
			.then(data => {
				//construct event from JSON
				this.setState({ event: Event.fromJSON(data) });
			})
			.catch(error => {
				//show error message
				alert('Error loading event data!');
				console.error(error);
			})
			.finally(() => {
				//hide loader
				this.setState({ fetching: false });
			});
	}
	
	render() {
		//prepare default form values
		let defaultDesc = '';
		let defaultTime = '';
		
		if (this.state.event) {
			//fill description field and format time
			defaultDesc = this.state.event.description;
			defaultTime = ('0' + this.state.event.date.getHours()).substr(-2) + 
				':' +
				('0' + this.state.event.date.getMinutes()).substr(-2);
		}
		else if (this.state.addingMode) {
			//changing defaultValue from non-empty string doesn't work, set it once
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
