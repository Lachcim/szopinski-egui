function changeMonth() {
	var month = document.getElementById("month").value;
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
		
	document.getElementById("monthDisplay").innerText = monthDict[month - 1];
	document.getElementById("acceptDate").disabled = false;
}
function changeYear(up) {
	var year = parseInt(document.getElementById("year").innerText);
	
	if (up) year++;
	else year--;
	
	document.getElementById("year").innerText = year;
	document.getElementById("acceptDate").disabled = false;
}
function changeDate() {
	var month = document.getElementById("month").value;
	var year = document.getElementById("year").innerText;
	
	window.location = year + "-" + month;
}
