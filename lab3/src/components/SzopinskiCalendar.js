import React from 'react';
import Calendar from './Calendar';
import DayEditor from './DayEditor';
import EventEditor from './EventEditor';

class SzopinskiCalendar extends React.Component {
	constructor(props) {
		super(props);
		
		//maintain main active component, switch betwen three
		this.state = {
			activeComponent: null
		};
		
		this.showCalendar = this.showCalendar.bind(this);
		this.showDayEditor = this.showDayEditor.bind(this);
		this.showEventEditor = this.showEventEditor.bind(this);
	}
	
	componentDidMount() {
		//show calendar by default
		this.showCalendar();
	}
	
	showCalendar(year, month) {
		//when a day is picked, show day editor
		this.setState({
			activeComponent: (<Calendar year={year} month={month} onDayPick={this.showDayEditor}/>)
		});
	}
	showDayEditor(day) {
		//when the day editor closes, show calendar; when an event is edited, show event editor
		this.setState({
			activeComponent: (<DayEditor day={day} onClose={this.showCalendar} onEditEvent={this.showEventEditor}/>)
		});
	}
	showEventEditor(id, day) {
		//when the event editor closes, show day editor
		this.setState({
			activeComponent: (<EventEditor id={id} day={day} onClose={this.showDayEditor}/>)
		});
	}
	
	render() {
		//render the current main component
		return this.state.activeComponent;
	}
}

export default SzopinskiCalendar;
