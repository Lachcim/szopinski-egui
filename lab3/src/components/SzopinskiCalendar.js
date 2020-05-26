import React from 'react';
import MainWrapper from './MainWrapper';
import Calendar from './Calendar';
import DayEditor from './DayEditor';

class SzopinskiCalendar extends React.Component {
	constructor(props) {
		super(props);
		
		this.state = {};
		
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
			activeComponent: (<DayEditor day={day} events={events}/>)
		});
	}
	
	render() {
		return (
			<MainWrapper>
				{this.state.activeComponent}
			</MainWrapper>
		);	
	}
}

export default SzopinskiCalendar;
