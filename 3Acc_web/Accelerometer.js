// The serviceUuid must match the serviceUuid of the device you would like to connect
const serviceUuid = "52c0fb16-8ced-11eb-8dcd-0242ac130003";
let myCharacteristic;
let myValue = 0;
let myBLE;

// //  an instance of the serialport library:
// let serial;

function setup() {
  colorMode(RGB, 100);
  // Create a p5ble class
  myBLE = new p5ble();

  canvas = createCanvas(200, 200, WEBGL);

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
  myBLE.startNotifications(myCharacteristic, handleNotifications);
}

// A function that will be called once got characteristics
function handleNotifications(data) {
  console.log('data: ', data);
  myValue = data;
}

// A function to stop notifications
function stopNotifications() {
  myBLE.stopNotifications(myCharacteristic);
}

// orientation variables:
let heading = 0.0;
let pitch = 0.0;
let roll = 0.0;

function serialEvent() {
  // read from port until new line:
  let message = serial.readStringUntil('\n');
  if (message != null) {
    let list = split(trim(message), ',');
    if (list.length >= 3) {
      // conver list items to floats:
      heading = float(list[0]);
      pitch = float(list[1]);
      roll = float(list[2]);
      console.log(heading + ',' + pitch + ',' + roll);
      // send a byte to the microcontroller to get new data:
      serial.write('x');
    }
  }
}

// draws the Arduino Nano:
function drawArduino() {
  // the base board:
  stroke(0, 90, 90); // set outline color to darker teal
  fill(0, 130, 130); // set fill color to lighter teal
  box(300, 10, 120); // draw Arduino board base shape

  // the CPU:
  stroke(0);         // set outline color to black
  fill(80);          // set fill color to dark grey
  translate(30, -6, 0); // move to correct position
  box(60, 0, 60);    // draw box

  // the radio module:
  stroke(80);       // set outline color to grey
  fill(180);        // set fill color to light grey
  translate(80, 0, 0); // move to correct position
  box(60, 15, 60);  // draw box

  // the USB connector:
  translate(-245, 0, 0); // move to correct position
  box(35, 15, 40);   // draw box


  function draw() {
    // update the drawing:
    background(255); // set background to white
    push();          // begin object to draw

    // variables for matrix translation:
    let c1 = cos(radians(roll));
    let s1 = sin(radians(roll));
    let c2 = cos(radians(pitch));
    let s2 = sin(radians(pitch));
    let c3 = cos(radians(heading));
    let s3 = sin(radians(heading));
    applyMatrix(c2 * c3, s1 * s3 + c1 * c3 * s2,
      c3 * s1 * s2 - c1 * s3, 0, -s2, c1 * c2,
      c2 * s1, 0, c2 * s3, c1 * s2 * s3 - c3 * s1,
      c1 * c3 + s1 * s2 * s3, 0, 0, 0, 0, 1);

      // draw arduino board:
      drawArduino();
      pop(); // end of object
    }
  }
