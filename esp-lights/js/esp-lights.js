var updateStatus = function (status) {
	updateStatusForLight('#red', status.red);
	updateStatusForLight('#yellow', status.yellow);
	updateStatusForLight('#green', status.green);
};

var updateStatusForLight = function (light, status) {
	if (status === 1)
		turnLightOn(light);
	else
		turnLightOff(light);
};

var changeState = function (light, state) {
	$.ajax({
		method: 'POST',
		url: '/light?light=' + light + '&state=' + state,
		data: ''
	}).done(function (data) {
		console.log(data);
	}).fail(function (err) {
		console.log(err);
	});
};

var getStatus = function () {
	$.ajax({
		method: 'GET',
		url: '/status'
	}).done(function (data) {
		console.log(data);
	}).fail(function (err) {
		console.log(err);
	});
};

var turnLightOn = function (lightId) {
	$(lightId).addClass('active');
	var lightName = getLightName(lightId);
	changeState(lightName, 'ON');
};

var turnLightOff = function (lightId) {
	$(lightId).removeClass('active');
	var lightName = getLightName(lightId);
	changeState(lightName, 'OFF');
};

var toggle = function (lightId) {
	var lightElement = $(lightId);

	if (lightElement.hasClass('active'))
		turnLightOff(lightId);
	else
		turnLightOn(lightId);
	getStatus();
};

var getLightName = function (lightId) {
	return lightId.substring(1, a.length)
}

$('#red').on('mouseup', function (ev) { toggle('#red') });
$('#yellow').on('mouseup', function (ev) { toggle('#yellow') });
$('#green').on('mouseup', function (ev) { toggle('#green') });