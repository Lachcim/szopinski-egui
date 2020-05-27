import React from 'react';
import Loader from './Loader';

class CalendarEvent extends React.Component {
	constructor(props) {
		super(props);
		
		this.state = {
			data: props.data,
			onEdit: props.onEdit,
			onRemove: props.onRemove,
			fetching: false
		};
		
		this.edit = this.edit.bind(this);
		this.remove = this.remove.bind(this);
	}
	
	edit(e) {
		//fire onEdit event for the parent component to pass upwards
		if (e) e.preventDefault();
		if (this.state.onEdit) this.state.onEdit(this.state.data.id);
	}
	remove(e) {
		//call to API to remove the event and notify the parent component
		
		//display loader
		if (e) e.preventDefault();
		this.setState({ fetching: true });
		
		//make API call
		fetch('api/event/' + this.state.data.id, {method: 'delete'})
			.then(response => response.json())
			.then(() => {
				//fire event for parent component, this component will be unmounted
				if (this.state.onRemove) this.state.onRemove(this.state.data.id);
			})
			.catch(error => {
				//hide loader, show error message
				this.setState({ fetching: false });
				alert('Error deleting event!');
				console.error(error);
			});
	}
	
	render() {
		//format time
		const displayTime = ('0' + this.state.data.date.getHours()).substr(-2) +
			':' +
			('0' + this.state.data.date.getMinutes()).substr(-2);
		
		return (
			<li>
				{this.state.fetching && <Loader/>}
				<h2>{displayTime}</h2>
				<p>{this.state.data.description}</p>
				<form className="actions" onSubmit={this.remove}>
					<a href="#" className="button" onClick={this.edit}>Edit</a>
					<button>Delete</button>
				</form>
			</li>
		);
	}
}

export default CalendarEvent;
