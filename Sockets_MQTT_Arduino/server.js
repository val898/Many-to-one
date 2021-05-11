/*
note this is my first web-app build in glitch
 it likely has some issues
 if you remixed this you need a shiftr URL in the
 .env file
 var name is ::
 process.env.SHIFTR_URL -- you will need to supply a value from shiftr
 it will be the url presented at end of token creation -- full url
 you also need to pick a parser -- Arduino can use JSON, raw or maybe OSC
*/

// first part of this is from shifman -- code train socket servers
// proper links etc coming

const env = require('dotenv');
env.config();

const PORT = process.env.PORT || 3009;

var express = require('express');

var app = express();

var server = app.listen(PORT);

app.use(express.static('public'));

console.log("my socket server is running on Port :: " + PORT);


// _______________________________________________________
// fancy socket server
// it 'upgrades' connection, listens for data and draws it
// _______________________________________________________

// create a web socket server -- sockets let node talk to web pages
var socket = require('socket.io');
var io = socket(server);

io.sockets.on('connection', newConnection);

// create a function that handles incoming messages to the server
// and passes them out to all the clients as needed
function newConnection (socket) {

  // set up event handlers for messages FROM WEB clients
  socket.on('topic1_s', handleSocket_topic1_s); // eventType, fxn

  // call back for EACH topic subscribed -- this is for topic1
  function handleSocket_topic1_s(data){
    console.log('Node server handleSocket_topic1_s :: ',data);

    // VDL -- you probably can comment this out
    // can use this to send WEB CLIENT message all the other WEB clients -- but not the source ::
    socket.broadcast.emit('topic1_s', data);
    // alt ::  send to all clients including echo to source
    // io.sockets.emit('mouse',data);

    // call this function to send web-client data via shiftr (MQTT)
    // data is JSON OBJECT
    printWithType(data);
    mqttSend('topic1', data); // change topic for mqtt

    //console.log(data);
  }

  socket.on('disconnect', function() {
      // do stuff when web client leaves
  });

}

// MQTT connections
// this code provides a bridge to shiftr.
// mqtt library is included in dependecies of JSON file
const mqtt = require('mqtt');

// the URL is the url presented at end of token process in shiftr
let mqttClient = mqtt.connect(process.env.SHIFTR_URL, {clientId: 'JSNODEserver_sd'}); // clientId is tag or visualizing data flow

// event handles for connect to shiftr and message received
mqttClient.on('connect', mqttConnect);
mqttClient.on('message', mqttMessageEvent); // message is generic handler

function mqttConnect(e) {

  console.log('connected to shiftr!');

  mqttClient.subscribe('topic1'); // subscribe to published topics
  mqttClient.subscribe('topic2');

  // if you want to make this function happen on a timer
  //setInterval(function() {
  //  client.publish('jsMouse');
  //}, 5000);

}


// messages from MQTT clients -- in this case wrist bands show up here
function mqttMessageEvent(topic, message) {

  // need topic routing here as required by project
  console.log('in mqttMessageEvent --> topic : '+ topic + '-- message : ' + message);

  switch (topic){
    case 'topic1':
      console.log('top1');
    break;
    case 'topic2':
      console.log('top2');
    break;
    default:
  }

}

// this fxn is called from inside the socket above

// this fxn takes WEB client data and PUBLISHES it to shiftr for physical clients ( arduinos )
function mqttSend(theTopic, messageToSend){
  console.log('messageToSend ',messageToSend);

  // data is jason and we need to unpack it and reconfigure for MQTT string transport

  // what to do about ids ? do you need to talk to specific bands - yes -- need to think about this.
  let firstVar_idorOther = messageToSend.idOrOther;
  let messageBody = messageToSend.str;

  console.log('thisTopic ', theTopic);
  console.log('messageBody ', messageBody);

  // // stringify is needed bc data is a JSON <OBJECT> -- and MQTT lib needs a String
  // var data = {
  //   cc: connectCounter
  // }
  //
  // let s = JSON.stringify(data); // https://stackoverflow.com/questions/4162749/convert-js-object-to-json-string
  //console.log('server mqttSend ' + s);

  let stringMessage = JSON.stringify(messageToSend); // MQTT needs string
  printWithType (stringMessage);

  mqttClient.publish(theTopic, stringMessage);// topic, data
}

// utility function may no longer be needed.
function printWithType(val){
  console.log('val&type ',val,'\t',typeof(val));
}
