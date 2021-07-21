// Example reterived from https://www.youtube.com/playlist?list=PLbghT7MmckI4Za8nEwpwzKsK3V0Tf53Tp
// This is the entry point for our whole application

var express = require("express");

var app = express();

var count = 0;

app.get("/", (req, res) => {
   // MyCoolGame.com was accessed via web browser, so server up 
   // the static content for out WebGL Unity application
   res.send("Hello, here is you Unity WebGL game: ");
});

app.get("/user/:id", (req, res) => {
    // Return the user's info of wins/losses/etc...
    // The goal, will be to read this from some sort of database. 

    var dummyData = {
        "userid": req.params["id"],
        "username": "val898",
        "wins": 18,
        "losses": 1000,
        "someArray": [
            { name: "foo", value: 2.5 },
            { name: "bar", value: 7.1 },
            { name: "baz", value: 9000.001 }
        ]
    };

    // JSON

    res.json(dummyData);
});

app.listen (8000, () => {
    console.log("Server has started!");
});

// app.post("/") //needed for example when sumbiting forms