import serial
import time
from datetime import datetime
import requests
import json

ser = serial.Serial('/dev/ttyUSB0', 9600)
outputfile = open("sensorsRoom8.csv","w");
outputfile.write("sensorLDR,sensorLux,sensorDHT22Humidity,sensorDHT22TemperatureCelsius,sensorADS_0_1,sensorADS_2_3")

# uuid = 'f6a72f8c-dc5f-42b9-bc25-cd7c6a66d1c9' ## uuid for testing
uuid = 'e8dcf7f8-4977-4295-a955-3ee3daf18714'

while True:
	# timestamp = datetime.datetime.fromtimestamp(time.time()).strftime('%Y-%m-%d %H:%M:%S.%f').split()
	timestamp = datetime.utcnow().strftime('%Y-%m-%dT%H:%M:%S.%fZ')
	data = ser.readline().replace('\r\n','').split(',')
	# print data
	# continue;
	if data[0].startswith("sensor"):
		continue;

	data_string = '{\"data\": [ {\"timestamp\": \"'+str(timestamp)+'\", \"ldr\": '+str(data[0])+', \"lux\": '+str(data[1])+', \"humidity\": '+str(data[2])+', \"temperature\": '+str(data[3])+', \"ads_0_1\": '+str(data[4])+', \"ads_2_3\": '+str(data[5])+' }]}'
	json_data = json.loads(data_string)

	r = requests.post('http://143.107.45.126:30134/adaptor/resources/' + uuid + '/data/room_monitoring', json=json_data)
	# print r.text

	outputfile.write(timestamp+","+data[0]+","+data[1]+","+data[2]+","+data[3]+","+data[4]+","+data[5]+","+r.text+"\n");