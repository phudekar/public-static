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
		url: '/light?light=light&state=' + state,
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

var turnLightOn = function (light) {
	$(light).addClass('active');
	changeState(light, 'ON');
};

var turnLightOff = function (light) {
	$(light).removeClass('active');
	changeState(light, 'OFF');
};

var toggle = function (light) {
	var lightElement = $(light);
	if (lightElement.hasClass('active'))
		turnLightOff(light);
	else
		turnLightOn(light);
	getStatus();
};

$('#red').on('mouseup', function (ev) { toggle('#red') });
$('#yellow').on('mouseup', function (ev) { toggle('#yellow') });
$('#green').on('mouseup', function (ev) { toggle('#green') });