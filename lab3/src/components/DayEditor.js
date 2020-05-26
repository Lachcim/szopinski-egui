import React from 'react';
import MainWrapper from './MainWrapper';

class DayEditor extends React.Component {
	constructor(props) {
		super(props);
		
		this.state = {
			day: props.day,
			onClose: props.onClose
		};
		
		this.close = this.close.bind(this);
		this.addEvent = this.addEvent.bind(this);
	}
	
	close(e) {
		if (e) e.preventDefault();
		this.state.onClose(this.state.day.getFullYear(), this.state.day.getMonth() + 1);
	}
	addEvent(e) {
		if (e) e.preventDefault();
		this.state.onAddEvent(this.state.day);
	}
	
	render() {
		let displayDate = this.state.day.getFullYear() +
			'-' +
			('0' + (this.state.day.getMonth() + 1)).substr(-2) +
			'-' +
			('0' + this.state.day.getDate()).substr(-2);
		
		return (
			<MainWrapper compact>
				<header>
					<h1>{displayDate}</h1>
				</header>
				<main>
					<ul className="events">
						
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
