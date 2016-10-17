var i = 0;
var data = 0;

$(document).ready(function(){
	initFireBase();
  window.setInterval(updateStatus, 1500);
});

function updateStatus()
{
  status = firebase.database().ref('testData/on').once('value').then(function(snapshot){
    if(snapshot == null)
      return;
    data = snapshot.val();
  });
  var current = Date.now();
  current /= 1000;
  console.log(data+" now: "+ current);
  if(current  < data + 10)
  {
    $('#statusValue').text('on');
    $('#statusValue').addClass('status-on');
  }
  else
  {
    $('#statusValue').text('off');
    $('#statusValue').removeClass('status-on');
  }
}

function programExecuted(data)
{
  console.log('executing '+data);
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
  $("#commands").append($code);

  var read = firebase.database().ref('testData/read/'+newKey);
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