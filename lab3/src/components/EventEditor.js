import React from 'react';
import MainWrapper from './MainWrapper';
import Event from '../datatypes/Event';

class EventEditor extends React.Component {
	constructor(props) {
		super(props);
		
		this.state = {
			addingMode: !props.id,
			eventId: props.id,
			event: null,
			defaultDay: props.day,
			onClose: props.onClose
		};
		
		this.timeInput = React.createRef();
		this.descriptionInput = React.createRef();
		
		this.apply = this.apply.bind(this);
		this.discard = this.discard.bind(this);
	}
	
	apply(e) {
		if (e) e.preventDefault();
		
		// TODO: commit changes
		
		if (this.state.onClose) this.state.onClose(this.state.defaultDay);
	}
	discard(e) {
		if (e) e.preventDefault();
		if (this.state.onClose) this.state.onClose(this.state.defaultDay);
	}
	
	render() {
		let defaultDesc = '';
		let defaultTime = '12:00';
		
		if (this.state.event) {
			defaultDesc = this.state.event.description;
			defaultTime = ('0' + this.state.event.date.getHours()).substr(-2) + 
				':' +
				('0' + this.state.event.date.getMinutes());
		}
		
		return (
			<MainWrapper compact>
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
