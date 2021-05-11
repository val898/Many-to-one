
// ITP Networked Media, Fall 2014
// https://github.com/shiffman/itp-networked-media
// Daniel Shiffman

// Keep track of our socket connection
var socket;

function setup() {
  createCanvas(400, 400);
  background(0);
  // Start a socket connection to the server
  // Some day we would run this server somewhere else
  // socket = io.connect('http://localhost:3000');
  socket = io.connect();
  // We make a named event called 'mouse' and write an
  // anonymous callback function
  socket.on('mouse', handleMouseData
    // When we receive data
    // function(data) {
    //   console.log("Got: " + data.x + " " + data.y);
    //   // Draw a blue circle
    //   fill(0,0,255);
    //   noStroke();
    //   ellipse(data.x, data.y, 20, 20);
    // }
  );
}
function handleMouseData(frog) {
	// function(data) {
	  console.log("What kind is frog: " + typeof frog);
	  console.log(frog);
      console.log("Got: " + frog.ex + " " + frog.y);
      // Draw a blue circle
      fill(0,0,255);
      noStroke();
      ellipse(frog.ex, frog.y, 20, 20);
    // }
}

function draw() {
  // Nothing
}

function mouseDragged() {
  // Draw some white circles
  fill(255);
  noStroke();
  ellipse(mouseX,mouseY,20,20);
  // Send the mouse coordinates
  sendmouse(mouseX,mouseY);
}

// Function for sending to the socket
function sendmouse(xpos, ypos) {
  // We are sending!
  console.log("sendmouse: " + xpos + " " + ypos);

  // Make a little object with  and y
  var data = {
    ex: xpos,
    y: ypos
  };

  // Send that object to the socket
  socket.emit('cat',data);
}
