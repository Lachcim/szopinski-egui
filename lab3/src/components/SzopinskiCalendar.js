import React from 'react';
import Calendar from './Calendar';
import DayEditor from './DayEditor';

class SzopinskiCalendar extends React.Component {
	constructor(props) {
		super(props);
		
		this.state = {
			activeComponent: null
		};
		
		this.showCalendar = this.showCalendar.bind(this);
		this.showDayEditor = this.showDayEditor.bind(this);
	}
	
	componentDidMount() {
		this.showCalendar();
	}
	
	showCalendar(year, month) {
		this.setState({
			activeComponent: (<Calendar year={year} month={month} onDayPick={this.showDayEditor}/>)
		});
	}
	showDayEditor(day, events) {
		this.setState({
			activeComponent: (<DayEditor day={day} onClose={this.showCalendar}/>)
		});
	}
	
	render() {
		return this.state.activeComponent;
	}
}

export default SzopinskiCalendar;
