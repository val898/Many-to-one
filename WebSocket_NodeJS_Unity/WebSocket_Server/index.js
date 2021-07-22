//https://www.youtube.com/watch?v=GDmXz0mboB0&list=PLbghT7MmckI4Za8nEwpwzKsK3V0Tf53Tp&index=4

const WebSocket = require('ws')
const wss = new WebSocket.Server({port:8080}, ()=>{
    console.log('Server started!')
})

wss.on('connection', (ws)=>{
    ws.on('message', (data)=>{
        console.log('Data received %o', data)
        ws.send(data)
    })
})

wss.on('listening', ()=>{
    console.log('Server is listening on port 8080')
})