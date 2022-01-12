var mqtt = require('mqtt')
var client  = mqtt.connect('mqtt://192.168.1.221:1883')
let topic_sub_admit = 'prueba'

client.on("error",(error)=>console.log)

client.on('connect', function () {
    console.log('COnectado')
    client.subscribe(topic_sub_admit, function (err) {
    if (!err) {
      //client.publish(topic_sub_admit, 'Hello mqtt de index.js')
    }
  })


})

client.on('message', function (topic, message) {
    if(topic = 'prueba'){
        const msg = message.toString()
        let data = JSON.parse(message.toString())
        console.log(data)

    }

  })



