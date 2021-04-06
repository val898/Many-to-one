// The serviceUuid must match the serviceUuid of the device you would like to connect
const serviceUuid = "52c0fb16-8ced-11eb-8dcd-0242ac130003";
let myCharacteristic;
let myValue = 0;
let myBLE;

function setup() {
  colorMode(RGB, 100);
  // Create a p5ble class
  myBLE = new p5ble();

  canvas = createCanvas(200, 200);
  canvas.stroke(128, 128, 128);
  canvas.position(60, 80);
  textSize(20);
  textAlign(80, 120);

  // Create a 'Connect' button
  const connectButton = createButton('Connect')
  connectButton.position(70, 300);
  connectButton.mousePressed(connectToBle);

  // Create a 'Disconnect' button
  const disconnectButton = createButton('Disconnect')
  disconnectButton.position(160, 300);
  disconnectButton.mousePressed(disconnectToBle);
}

function connectToBle() {
  // Connect to a device by passing the service UUID
  myBLE.connect(serviceUuid, gotCharacteristics);
}

function disconnectToBle() {
  // Disconnect to the device
  myBLE.disconnect();
}

// A function that will be called once got characteristics
function gotCharacteristics(error, characteristics) {
  if (error) console.log('error: ', error);
  console.log('characteristics: ', characteristics);
  myCharacteristic = characteristics[0];
  // Read the value of the first characteristic
  // myBLE.read(myCharacteristic, gotValue);
  myBLE.startNotifications(myCharacteristic, handleNotifications);
}

// // A function that will be called once got values
// function gotValue(error, value) {
//   if (error) console.log('error: ', error);
//   console.log('value: ', value);
//   myValue = value;
//   // After getting a value, call p5ble.read() again to get the value again
//   // myBLE.read(myCharacteristic, gotValue);
//   myBLE.startNotifications(myCharacteristic, gotValue);
//   // You can also pass in the dataType
//   // Options: 'unit8', 'uint16', 'uint32', 'int8', 'int16', 'int32', 'float32', 'float64', 'string'
//   // myBLE.read(myCharacteristic, 'string', gotValue);
// }

// A function that will be called once got characteristics
function handleNotifications(data) {
  console.log('data: ', data);
  myValue = data;
}

// A function to stop notifications
function stopNotifications() {
  myBLE.stopNotifications(myCharacteristic);
}

function draw() {
  if(myValue === 1) {
    background(0, 255, 255);
    text("CYAN");
  }
  else if (myValue === 2) {
    background(255, 255, 0);
    text("YELLOW");
  }
  else if (myValue === 3) {
    background(255, 0, 255);
    text("PINK");
  }
  else if (myValue === 4) {
    background(0, 255, 0);
    text("GREEN");
  }
  else {
    background(128, 128, 128);
  }
}
