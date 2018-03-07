import requests
import numpy as np
import matplotlib.pyplot as plt
import time
from datetime import date
import datetime

uuid = 'e8dcf7f8-4977-4295-a955-3ee3daf18714'

t0 = 1.3500000E+02
v0 = 5.9588600E+00

p = [ 2.0325591E+01, 3.3013079E+00, 1.2638462E-01, -8.2883695E-04 ]
q = [ 1.7595577E-01, 7.9740521E-03, 0.0 ]

scales = [ 0.1875, 0.125, 0.0625, 0.03125, 0.015625, 0.0078125 ]
gainIdx = 3


def calc(v):
  d1 = 0.
  d2 = 1.
  t = 0

  t = v - v0

  for i in range(0, 4):
    d1 += t * p[i]
    t = t * (v - v0)

  t = v - v0

  for i in range(0, 3):
    d2 += t * q[i]
    t = t * (v - v0)

  return (t0 + d1/d2)

#today = datetime.datetime.now()



sensor1 = [ ]
sensor2 = [ ]
humidity = [ ]
lux = [ ]
ldr = [ ]

dht = [ ]

i = time.time() - 3600 * (12)
#end = time.time() - 1000 * 2 - 3600 * ( 12 - 6 )

today = datetime.datetime.fromtimestamp(i)
t = today.strftime('%Y-%m-%dT%H:%M:%S')

# t = datetime.datetime.fromtimestamp('2018-03-06T09:30:30')
labels = []

while True:

	#if len(dht) >= 1000:
	#	break

	print t

	json = requests.post(
		'http://143.107.45.126:30134/collector/resources/' + uuid + '/data',
		json={'start_date': t, 'end_date': '2018-03-06T16:30:00' })
    # json = requests.post(
	# 	'http://143.107.45.126:30134/collector/resources/' + uuid + '/data',
	# 	json={'start_date': t })

	#print 'curl -H "Content-Type: application/json" -X POST -d \'{"start_date":"%s", "end_date": "%s"}\' http://143.107.45.126:30134/collector/resources/e8dcf7f8-4977-4295-a955-3ee3daf18714/data' % (t, endTime)

	print len(json.json()['resources'][0]['capabilities']['room_monitoring'])
	print json.text

	if len(json.json()['resources'][0]['capabilities']['room_monitoring']) < 10:
		break

	data = json.json()['resources'][0]['capabilities']['room_monitoring']

	for d in data[1:]:
		sensor1.append(calc(d['ads_0_1'] * scales[gainIdx]) + d['temperature'])
		sensor2.append(calc(d['ads_2_3'] * scales[gainIdx]) + d['temperature'])
		dht.append(d['temperature'])
		ldr.append(d['ldr'])
		lux.append(d['lux'])
		humidity.append(d['humidity'])
		t = d['date']

		datetime_object = datetime.datetime.strptime(t[:len(t)-1], '%Y-%m-%dT%H:%M:%S.%f') - datetime.timedelta(hours=3)

		labels.append(datetime_object.strftime('%H:%M:%S'))

	i += 1000 * 2

plt.ylim(20, 45)

plt.xticks(range(0, len(dht), 4000), [ labels[i] for i in range(0, len(dht), 4000)], rotation = 30)

#ax = plt.add_subplot(111)

#ax.set_xlabel('xlabel')
plt.title('Sensor 1')
plt.ylabel('Temperature (Celsius)')


sensor1_line, = plt.plot(sensor1)
DHT_line, = plt.plot(dht)

plt.legend([sensor1_line, DHT_line], ['Sensor 1', 'DHT'])

###

plt.figure()

plt.ylim(20, 45)

plt.xticks(range(0, len(dht), 4000), [ labels[i] for i in range(0, len(dht), 4000)], rotation = 30)

#ax = plt.add_subplot(111)

#ax.set_xlabel('xlabel')
plt.title('Sensor 2')
plt.ylabel('Temperature (Celsius)')


sensor2_line, = plt.plot(sensor2)
DHT_line, = plt.plot(dht)

plt.legend([sensor2_line, DHT_line], ['Sensor 2', 'DHT'])

#####

plt.figure()

plt.ylim(0, 100)

plt.xticks(range(0, len(dht), 4000), [ labels[i] for i in range(0, len(dht), 4000)], rotation = 30)

#ax = plt.add_subplot(111)

#ax.set_xlabel('xlabel')
plt.title('Humidity')
plt.ylabel('%')


humidity_line, = plt.plot(humidity)

plt.legend([humidity_line], ['Humidity'])

#####


# plt.figure()
#
# plt.xticks(range(0, len(dht), 4000), [ labels[i] for i in range(0, len(dht), 4000)], rotation = 30)
#
# #ax = plt.add_subplot(111)
#
# #ax.set_xlabel('xlabel')
# plt.title('Lux')
# #plt.ylabel('%')
#
#
# lux_line, = plt.plot(lux)
#
# plt.legend([humidity_line], ['Lux'])

##############

plt.figure()

plt.ylim(950, 1024)

plt.xticks(range(0, len(dht), 4000), [ labels[i] for i in range(0, len(dht), 4000)], rotation = 30)

#ax = plt.add_subplot(111)

#ax.set_xlabel('xlabel')
plt.title('LDR')
#plt.ylabel('%')


ldr_line, = plt.plot(ldr)

plt.legend([ldr_line], ['LDR'])

#####

plt.show()
