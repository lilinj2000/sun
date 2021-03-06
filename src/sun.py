#!/usr/bin/env python

import json

foal_trader = { 
    "flow_path": "./",

    "front_address": "tcp://180.169.101.189:7001",
    "broker_id": "0158",
    "user_id": "04452",
    "password": "111111",
    "investor_id": "04452"
    }

sink1 = {
    "sink": {
        "type": "stdout_sink_mt",
        "level": "trace"
        }
    }

sink2 = {
    "sink": {
        "type": "rotating_file_sink_mt",
        "level": "info",
        "file_name": "sun.log",
        "max_file_size": 5000000,
        "max_files": 10
        }
    }

log = {
    "level": "trace",
    "sinks": [sink1, sink2]
    }

sun = {
    "instru": "IF1712",
    "price": 3790,
    "volume": 1,
    "interval": 2000,
    "count": 1,
    "only_t1": True,
    "data_file": "sun_data.txt"
    }

config = {
    "foal_trader": foal_trader,
    "log": log,
    "sun": sun
    } 

with open('sun.json', 'w') as f:
    json.dump(config, f, sort_keys=True, indent=4)
