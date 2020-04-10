function changeDate(up) {
	var month = document.getElementById("month");
	var monthDisplay = document.getElementById("monthDisplay");
	var year = document.getElementById("year");
	var accept = document.getElementById("acceptDate");
	
	if (up) {
		month.value++;
		if (month.value == 13) {
			month.value = 1;
			year.innerText = Number(year.innerText) + 1;
		}
	}
	else {
		month.value--;
		if (month.value == 0) {
			month.value = 12;
			year.innerText = Number(year.innerText) - 1;
		}
	}
	
	var monthDict = [
		"January",
		"February",
		"March",
		"April",
		"May",
		"June",
		"July",
		"August",
		"September",
		"October",
		"November",
		"December"];
	
	monthDisplay.innerText = monthDict[month.value - 1];
	accept.disabled = false;
}
function applyDate() {
	var month = document.getElementById("month").value;
	var year = document.getElementById("year").innerText;
	
	window.location = year + "-" + ("0" + month).toString().substr(-2);
}
