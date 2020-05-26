import React from 'react';
import Calendar from './Calendar';
import DayEditor from './DayEditor';
import EventEditor from './EventEditor';

class SzopinskiCalendar extends React.Component {
	constructor(props) {
		super(props);
		
		this.state = {
			activeComponent: null
		};
		
		this.showCalendar = this.showCalendar.bind(this);
		this.showDayEditor = this.showDayEditor.bind(this);
		this.showEventEditor = this.showEventEditor.bind(this);
	}
	
	componentDidMount() {
		this.showCalendar();
	}
	
	showCalendar(year, month) {
		this.setState({
			activeComponent: (<Calendar year={year} month={month} onDayPick={this.showDayEditor}/>)
		});
	}
	showDayEditor(day) {
		this.setState({
			activeComponent: (<DayEditor day={day} onClose={this.showCalendar} onEditEvent={this.showEventEditor}/>)
		});
	}
	showEventEditor(id, day) {
		this.setState({
			activeComponent: (<EventEditor id={id} day={day} onClose={this.showDayEditor}/>)
		});
	}
	
	render() {
		return this.state.activeComponent;
	}
}

export default SzopinskiCalendar;
