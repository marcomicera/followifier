const express = require("express");
const MongoClient = require('mongodb').MongoClient;
const assert = require('assert');

// Connection URL
const url = 'mongodb://localhost:27017';

// Database Name
const dbName = 'followifier';

// Number of devices time interval (in seconds)
const numDevicesTimeInterval = 1 * 60 * 5;

// Time window within which devices must be shown in the radar (scatter chart)
const devicesRadarTimeWindows = 1 * 60;

const app = express();


app.listen(8000, () => {
  console.log('Server started!')
})
app.use(function(req, res, next) {
  res.header("Access-Control-Allow-Origin", "*");
  res.header("Access-Control-Allow-Headers", "Origin, X-Requested-With, Content-Type, Accept");
  next();
});


// Create a new MongoClient
const client = new MongoClient(url);

// Use connect method to connect to the Server
app.route('/api/device').get((req, res) => {
  var resultArray = [];
  client.connect(function (err) {
    assert.equal(null, err);

    let date = Date.now()/1000;
    date = parseInt(date);
    const db = client.db(dbName);
    var coll = db.collection("messages");
    coll.distinct('mac', {timestamp:{$gt:date-5*60}},function(err, result) {
      if (err) {
        res.send(err);
      } else {

        res.send(JSON.stringify(result.length));
      }
    })
  });
})
app.route('/api/device/number').get((req, res) => {
  var resultArray = [];
  client.connect(function (err) {
    assert.equal(null, err);

    let date = Date.now()/1000;
    date = parseInt(date);
    const db = client.db(dbName);
    var coll = db.collection("messages");
    coll.distinct('mac', {timestamp:{$gt:date-numDevicesTimeInterval}},function(err, result) {
      if (err) {
        res.send(err);
      } else {

        res.send(JSON.stringify(result.length));
      }
    })
  });
});
app.route('/api/devices').get((req, res) => {
  var resultArray = [];
  client.connect(function (err) {
    assert.equal(null, err);

    let date = Date.now()/1000;
    date = parseInt(date);
    const db = client.db(dbName);
    var coll = db.collection("messages");
    coll.aggregate([
                    {$match:{timestamp:{$gt:date-devicesRadarTimeWindows}}},
                    {$group:{_id:"$mac",   x: {$avg: '$x'}, y: {$avg: '$y'}}},
                    {$sort: {_id: 1}},
                    ]).toArray(function (err, result) {
      if (err) {
        res.send(err);
      } else {
        console.log('devices: ' + JSON.stringify(result));
        res.send(JSON.stringify(result));
      }
    })
  });
});
app.route('/api/devices/all').get((req, res) => {
  console.log('ALL');
  client.connect(function (err) {
    assert.equal(null, err);
    const db = client.db(dbName);
    var coll = db.collection("messages");
    coll.aggregate([
      {$group:{_id:"$mac"}},
      {$sort: {total: -1}},
    ]).toArray(function (err, result) {
      if (err) {
        res.send(err);
      } else {
        console.log('devices: ' + JSON.stringify(result));
        res.send(JSON.stringify(result));
      }
    })
  });
});
app.route('/api/devices/historical').get((req, res)  => {
  client.connect(function (err) {
    assert.equal(null, err);
    let date = Date.now() / 1000;
    date = parseInt(date);
    const db = client.db(dbName);
    var coll = db.collection("messages");
    coll.aggregate([
      {$match: {timestamp: {$gt: date - req.query.minutes*60}}},
      {$unwind: "$mac"
      },
      {$group: {_id: "$mac",  n: { $sum: 1}}},
    ]).toArray(function (err, result) {
      if (err) {
        res.send(err);
      } else {
        console.log('minutes:' + req.query.minutes);
        console.log('devices: ' + JSON.stringify(result));
        res.send(JSON.stringify(result));
      }

    })
  });
});
app.route('/api/devices/position').get((req, res)  => {
  console.log('POSITION ' + req.query.mac);
  client.connect(function (err) {
    assert.equal(null, err);
    let date = Date.now() / 1000;
    date = parseInt(date);
    const db = client.db(dbName);
    var coll = db.collection("messages");
    coll.aggregate([
      {$match: {mac: {$eq: req.query.mac}}},
      {$group:{
        _id:"$timestamp",
        x: {$avg: '$x'}, y: {$avg: '$y'}
      }},
      {$sort: {_id: 1}},
    ]).toArray(function (err, result) {
      if (err) {
        res.send(err);
      } else {
        console.log('devices: ' + JSON.stringify(result));
        res.send(JSON.stringify(result));
      }
    })
  });
});
app.route('/api/boards').get((req, res) => {
  client.connect(function(err) {
    assert.equal(null, err);
    console.log("Connected successfully to server db");
    const db = client.db(dbName);
    var coll = db.collection("boards")

    coll.find({}).toArray(function (err, result) {
      if (err) {
        res.send(err);
      } else {

        res.send(JSON.stringify(result));
      }
    })
  });
});

app.route('/api/room').get((req, res) => {
  client.connect(function(err) {
    assert.equal(null, err);
    console.log("Connected successfully to server db");
    const db = client.db(dbName);
    var coll = db.collection("room")
    coll.find({}).toArray(function (err, result) {
      if (err) {
        res.send(err);
      } else {
        console.log('ROOM: ' + JSON.stringify(result));
        res.send(JSON.stringify(result));
      }
    })
  });
});
