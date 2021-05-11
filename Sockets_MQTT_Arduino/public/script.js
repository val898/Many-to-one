// handling floats from p5JS to arduino

// if floats are active in BLE library, check script_withFloat.js

/*
  constraints:
    JS -- have type number and string but not float
    JS- unicode 16 -- not ascii
    ARD -- has floats and ascii
    ARD -- use characteristic type to control parsing.
        -- if you are just sending frmo ARD -- then float characteristics are great.
*/

// BLE functions and vars are in traceBLE_variables.js
// wifi socket vars

let client;
let socket;

let input,button,greeting,messageToSend, package, messageRecieved;

function setup() {

  noCanvas();
  //socket = io.connect('http://127.0.0.1:8888'); // when testing in local context with local MQTT broker
  // make sure node mqtt lib and client mqtt lib are same version
  socket = io.connect();

  // callback for socket events with label topic1_s
  socket.on('topic1_s', handle_Topic1_s);
  //socket.on('mqtt',  mqttEvent);

  setupWebElements();

}

// keeping the web interface context seperate from the actual send
function formatMessage() {

  messageToSend = input.value().toString();
  console.log(typeof messageToSend);
  // update page with what you are sending
  rawContent.html('Raw message to send : ' + messageToSend );
  input.value(''); // clear input box

  // if need to send raw -- but we will send JSON
  //package = '*'+messageToSend + '#'; // add start and end bytes for ard parse

  package = parseInt(messageToSend); // input string to number
  console.log(package);
  packageContent.html('Formatted Package : ' + package );
  socketSend ('topic1_s', package);
}

  // these are web events -- CLIENT to NODE server
  // need to clarify this protocol -- ID passed too ?
function socketSend( theTopic, stringToSend ){

  console.log('socket :: sending string ', stringToSend );
  // MQTT sends strings
  // socket data mays well be JSON ? -- since its JS to JS
  // construct data package to send to server
  // id is dummy var holding for writband id ? needed - how to generate?  leaving this here as JSON parse is useful -- but probs just make a string with id in package above.

  var payload = {
    idOrOther: 1, //ID tag (will need to ID each wearable differently)
    str: stringToSend //numberToSend
  }
  // send the message to the server -- event c(sh)ould match pub/sub topics
  socket.emit(theTopic, payload); // event name, data
}

// incoming socket message arrives here -- need 2 clients b/c sockets do not send to self -- see
function handle_Topic1_s (data){
  console.log("in handle_Topic1_s" + data);
  // parse data
  console.log(data.idOrOther);
  console.log(data.str);

  messageRecieved.html('Message Recieved :: ' + data.idOrOther + ' :: ' + data.str);
}




function draw(){;}

function setupWebElements(){
  // client web page formatting and elements
  // heading
  greeting = createElement('h2', 'WiFi - send strings to node (socket)');
  greeting.position(20, 50);

  // input box to enter string you want to send
  input = createInput();
  input.addClass('input');
  input.position(20, 120);
  input.size(230);

  // send button
  button = createButton('send over WiFi');
  button.position(input.x + input.size().width+10, 120);
  button.mousePressed(formatMessage);

  // place holder to echo back the string entered
  rawContent = createElement('p', 'Raw message to send : <em>waiting for input</em>');
  rawContent.position(20,160);

  // place holder to echo back the string formatted to parsing
  packageContent = createElement('p', 'Formatted Package : <em>waiting for input</em>');
  packageContent.position(20,200);

  // place holder to show messages received
  messageRecieved = createElement('p', 'Message Recieved : <em>waiting for input</em>');
  messageRecieved.position(20,250);
}

// utility function may no longer be needed.
function printWithType(val){
  console.log('val&type ',val,'\t',typeof(val));
}
