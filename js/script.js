var i = 0;

$(document).ready(function(){
	$("#center").height("7em");
	$("#center").click(function(){
		$("#center").toggleClass('highlighted');
	});


	initFireBase();
});

function programExecuted(data)
{
  console.log('executing');
  if(data == null)
    return;
  if(data.read == true)
  {
    var j = data.i;
   $('#'+j).removeClass('program-unread');
   $('#'+j).addClass('program-read');
 }
}

function func()
{
  var program = document.getElementById('code').value;
  var updateData = {"code":program,
                  "i":i,
                  "read":false};

  var newKey = firebase.database().ref().push().key;
  var update = {};
  update['testData/unread/'+newKey] = updateData;

  firebase.database().ref().update(update);

  $code = $("<p id='" + (i++) + "'>" + program + "</p>");
  $code.toggleClass('program-unread');
  $("#commands").prepend($code);

  var read = firebase.database().ref('testData/read'+newKey);
  read.on('value', function(snapshot) {
    programExecuted(snapshot.val());
  });
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