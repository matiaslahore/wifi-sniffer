require('./config/config');
const express = require('express');
const bodyParser = require('body-parser');
const fs = require('fs');
const app = express();
var conn = require('./config/connection');
var mysql = require('mysql'),
    connection = mysql.createConnection(conn);
var cron = require('node-cron');

// parse application/x-www-form-urlencoded
app.use(bodyParser.urlencoded({extended: false}));

// parse application/json
app.use(bodyParser.json());


app.get('/', function (request, response) {
    response.status(200).json("API VS");
});

app.post('/phone', function (request, response) {
    let data = request.body;
    let phone = data['phone'];
    let mac = data['mac'];
    if (phone && mac) {
        let new_phone = {
            phone: phone,
            mac: mac
        };
        if (connection) {
            connection.query('INSERT INTO pn_mac SET ?', new_phone, function (error, result) {
                if (error) {
                    throw error;
                } else {
                    response.status(200).json("DATA SAVED, ID: " + result.insertId);
                }
            });
        }
    } else {
        response.status(400).json("Please send data in body like {\n" +
            "  \"phone\": \"1122334455\",\n" +
            "  \"mac\": \"11:22:33:44:55\"\n" +
            "}");
    }
});

app.get('/mac/:mac', function (request, response) {
    mac = request.params.mac;
    if (mac) {
        if (connection) {
            var sql = 'SELECT * FROM pn_mac WHERE mac = ' + connection.escape(mac);
            connection.query(sql, function (error, row) {
                if (error) {
                    throw error;
                } else {
                    response.status(200).json(row);
                }
            });
        }
    } else {
        response.status(400).json("Please send mac");
    }
});

app.get('/phone/:phone', function (request, response) {
    phone = request.params.phone;
    if (phone) {
        if (connection) {
            let sql = 'SELECT * FROM pn_mac WHERE phone = ' + connection.escape(phone);
            connection.query(sql, function (error, row) {
                if (error) {
                    throw error;
                } else {
                    response.status(200).json(row);
                }
            });
        }
    } else {
        response.status(400).json("Please send phone");
    }
});


////////////////////////////////////////    MQTT    ////////////////////////////////////////

var hot_macs = {};

var mqtt = require('mqtt');
var client = mqtt.connect('mqtt://181.15.155.210', {port: 1883});
client.on('connect', function () {
        client.subscribe('MACF-001', function (err) {
            if (!err) {
                client.publish('presence', 'Hello mqtt');
                console.log('connected to Sniffer MACF-001');
            }
        });
        client.subscribe('NODE', function (err) {
            if (!err) {
                client.publish('presence', 'Hello mqtt')
                console.log('connected to NODE');
            }
        });
        client.subscribe('HOT-MACS', function (err) {
            if (!err) {
                client.publish('presence', 'Hello mqtt')
                console.log('connected to HOT-MACS');
            }
        });
    }
);

app.get('/macs', (request, response) => {
    let sql = 'SELECT mac FROM mac_founds';
    connection.query(sql, function (error, row) {
        client.publish('FIND-OLD-MACS', JSON.stringify(row));
        setTimeout(function () {
            console.log("SENDING OLD MACS TO NODES....");
            console.log(hot_macs);
            sendLookMacToNodes();
        }, (row.length * 20 * 1000)); //20 sec per mac
        response.status(200).json(row);
    });
});

let sendLookMacToNodes = async () => {
    // first clean the list, past more than 5 minutes ?
    for (let i = 0; i < hot_macs.length; i++) {
        console.log("holaaa");
        console.log(hot_macs[i]);
        console.log((new Date().getTime() - hot_macs[i]) / (1000 * 60));
        if ((new Date().getTime() - hot_macs[i]) / (1000 * 60) > 2) {
            delete hot_macs[i];
        }
    }
    client.publish('LOOK-MAC', JSON.stringify(Object.keys(hot_macs)));
};

client.on('message', async (topic, message) => {
    if (topic === 'MACF-001') {
        //Sniffer
        snifferMessage(message);
    } else if (topic === 'HOT-MACS') {
        //RPI olds macs in store
        gatewayMessage(message);
    } else if (topic === 'NODE') {
        //RPI Zero Nodes
        nodeMessage(message);
    }
});

let gatewayMessage = async message => {
    let msg = JSON.parse(message);
    let device = msg["NODE"];
    let mac = msg["MAC"];
    let rssi = msg["RSSI"];
    hot_macs[mac] = new Date().getTime();
    if (device && mac && rssi && connection) {
        let report = {zone: device, mac: mac, rssi: rssi};
        connection.query('INSERT INTO reports SET ?', report);
    }
};

let nodeMessage = async message => {
    let msg = JSON.parse(message);
    let device = msg["NODE"];
    let mac = msg["MAC"];
    let rssi = msg["RSSI"];
    hot_macs[mac] = new Date().getTime();
    if (device && mac && rssi && connection) {
        let report = {zone: device, mac: mac, rssi: rssi};
        connection.query('INSERT INTO reports SET ?', report);
    }
};

let snifferMessage = async message => {
    let msg = JSON.parse(message.toString());
    let device = msg["NODE"];
    let macs = msg["MAC"];
    let rssi = msg["RSSI"];
    if (device && macs && rssi && connection) {
        let len = Math.min(macs.length, rssi.length);
        for (let i = 0; i < len; i++) {
            let sql = 'SELECT * FROM mac_founds WHERE mac = ' + connection.escape(macs[i]);
            connection.query(sql, function (error, row) {
                if ((!error) && (row.length === 0)) {
                    let mac_found = {store: device, mac: macs[i], phone: null, rssi: rssi[i]};
                    connection.query('INSERT INTO mac_founds SET ?', mac_found);
                    gatewayMessage(JSON.stringify({NODE: device, mac: macs[i], rssi: rssi[i]}));
                }
            });
        }
    }
};

cron.schedule('*/5 * * * *', () => {
    console.log('running a task every 5 minute');
    console.log(new Date());
    let sql = 'SELECT mac FROM mac_founds';
    connection.query(sql, function (error, row) {
        client.publish('FIND-OLD-MACS', JSON.stringify(row));
        setTimeout(function () {
            console.log("SENDING OLD MACS TO NODES....");
            console.log(hot_macs);
            sendLookMacToNodes();
        }, (row.length * 20 * 1000)); //20 sec per mac
    });
});

app.listen(process.env.PORT, () => {
    console.log(`API is up, serving:  http://localhost:${process.env.PORT}`);
});