var i = 0;

$(document).ready(function(){
	$("#center").height("7em");
	$("#center").click(function(){
		$("#center").toggleClass('highlighted');
	});


	initFireBase();
	var read = firebase.database().ref('testData/read');
read.on('value', function(snapshot) {
  programExecuted(snapshot.val());
});
});

function programExecuted(data)
{
	$('.program-unread').toggleClass('program-unread');
}

function func()
{
  var program = document.getElementById('code').value;
  var updates = {};

  updates['testData/'] = {"code":program, "read":false};
  firebase.database().ref().update(updates);
  $code = $("<p id='" + (i++) + "'>" + program + "</p>");
  $code.toggleClass('program-unread');
  $("#commands").prepend($code);
}


function initFireBase()
{
	var config = {
    apiKey: "AIzaSyBfg1NPfetB5f_LgW0sDnmVpXQngRric-A",
    authDomain: "testproject-9ac78.firebaseapp.com",
    databaseURL: "https://testproject-9ac78.firebaseio.com",
    storageBucket: "testproject-9ac78.appspot.com",
  };
  firebase.initializeApp(config);
}