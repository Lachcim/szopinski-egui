import React from 'react';

function MainWrapper(props) {
	return (
		<div id="mainwrapper" className={props.compact ? "compact" : ""}>
			{props.children}
			<footer>
				<p>Calendar by Szopiński</p>
			</footer>
		</div>
	);
}

export default MainWrapper;
