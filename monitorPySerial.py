import serial
import time
from datetime import datetime
import requests
import json

ser = serial.Serial('/dev/ttyUSB0', 9600)
outputfile = open("sensorsRoom8-"+datetime.utcnow().strftime('%Y-%m-%dT%H:%M:%S.%fZ')+".csv","w");
outputfile.write("sensorLDR,sensorSoilMoisture,sensorDHT22Humidity,sensorDHT22TemperatureCelsius,sensorADS_0_1,sensorADS_2_3")

# uuid = 'f6a72f8c-dc5f-42b9-bc25-cd7c6a66d1c9' ## uuid for testing
uuid = 'e8dcf7f8-4977-4295-a955-3ee3daf18714'

while True:
	# timestamp = datetime.datetime.fromtimestamp(time.time()).strftime('%Y-%m-%d %H:%M:%S.%f').split()
	timestamp = datetime.utcnow().strftime('%Y-%m-%dT%H:%M:%S.%fZ')
	data = ser.readline().replace('\r\n','').split(',')
	print (data)
	# continue;
	if data[0].startswith("sensor"):
		continue;

	#data_string = '{\"data\": [ {\"timestamp\": \"'+str(timestamp)+'\", \"ldr\": '+str(data[0])+', \"lux\": '+str(data[1])+', \"humidity\": '+str(data[2])+', \"temperature\": '+str(data[3])+', \"ads_0_1\": '+str(data[4])+', \"ads_2_3\": '+str(data[5])+' }]}'
	data_string = '''
        {{
            "data": [
                {{
                    "timestamp"		: "{0}",
                    "round"		: "{1}",
                    "ldr"		: "{2}",
                    "soil_moisture"	: "{3}",
                    "humidity"   	: "{4}",
                    "temperature"	: "{5}",
                    "ads_0_1"    	: "{6}",
                    "ads_2_3"    	: "{7}",
                    "adsm_0_1"   	: "{8}",
                    "adsm_2_3"   	: "{9}",
                    "ads2_0_1"		: "{10}",
                    "ads2_2_3"		: "{11}",
                    "heating"    	: "{12}"
                }}
            ]
        }}
        '''.format(timestamp, *data)

        print (data_string)

	json_data = json.loads(data_string)

	r = requests.post('http://143.107.45.126:30134/adaptor/resources/' + uuid + '/data/room_monitoring', json=json_data)
	print (len(r.text))

	outputfile.write(timestamp+","+','.join(str(e) for e in data)+","+str(len(r.text))+"\n");
