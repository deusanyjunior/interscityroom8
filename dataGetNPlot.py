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

    t = v - v0;

    for a in range(0, 4):
        d1 += t * p[a]
        t = t * (v - v0);

    t = v - v0;

    for b in range(0, 3):
        d2 += t * q[b]
        t = t * (v - v0);

    return (t0 + d1/d2);

#today = datetime.datetime.now()

sensor1 = [ ]
sensor2 = [ ]

humidity = [ ]
lux = [ ]
ldr = [ ]
dht = [ ]

heating = [ ]
round = [ ]

#i = time.time() - 3600 * (72)
time_step = 512
#end = time.time() - 1000 * 2 - 3600 * ( 12 - 6 )


#initial = datetime.datetime.fromtimestamp(i)
#t = initial.strftime('%Y-%m-%dT%H:%M:%S')
t = '2018-06-04T00:00:00' # start data
t2 = '2018-06-05T23:00:00' # end date

# t = datetime.datetime.fromtimestamp('2018-03-06T09:30:30')
labels = []

while True:

    #if len(dht) >= 1000:
    #	break

    print t
    #
    # json = requests.post(
    # 'http://143.107.45.126:30134/collector/resources/' + uuid + '/data',
    # json={'start_date': t, 'end_date': '2018-06-06T16:30:00' })

    json = requests.post(
    'http://143.107.45.126:30134/collector/resources/' + uuid + '/data',
    json={'start_date': t, 'end_date': t2, 'limit': '1000' })

    # json = requests.post(
    # 	'http://143.107.45.126:30134/collector/resources/' + uuid + '/data',
    # 	json={'start_date': t })

    #print 'curl -H "Content-Type: application/json" -X POST -d \'{"start_date":"%s", "end_date": "%s"}\' http://143.107.45.126:30134/collector/resources/e8dcf7f8-4977-4295-a955-3ee3daf18714/data' % (t, endTime)

    #
    # if len(json.json()['resources'][0]['capabilities']['room_monitoring']) < 1000:
    #     break
    try:
        data = json.json()['resources'][0]['capabilities']['room_monitoring']
    except:
        print "len(data) == 0"
        break

    print len(data)
<<<<<<< HEAD
    # print "data: "
    # print data
=======
    print "data: "
    print data
>>>>>>> atualizando o plot de dados para um período com visualização de fluxo no sensor2

    # if len(data) < 1000:
    #     breakrange(0, len(dht), 4000)


    if t == data[-1]['date']:
        break;

    for d in data[1:]:
        try:
            sensor1.append(calc(d['ads_0_1'] * scales[gainIdx]) + d['temperature'])
            sensor2.append(calc(d['ads_2_3'] * scales[gainIdx]) + d['temperature'])
            dht.append(d['temperature'])
            ldr.append(d['ldr'])
            lux.append(d['lux'])
            humidity.append(d['humidity'])
            t = d['date']
            heating.append(d['heating']+24)
            round.append(d['round'])

            datetime_object = datetime.datetime.strptime(t[:len(t)-1], '%Y-%m-%dT%H:%M:%S.%f') - datetime.timedelta(hours=3)
            labels.append(datetime_object.strftime('%m%d%H%M%S'))

        except KeyError, e:
            print "KeyError: " + str(e)
            print d
            datetime_object = datetime.datetime.strptime(t[:len(t)-1], '%Y-%m-%dT%H:%M:%S.%f') - datetime.timedelta(hours=3)
            labels.append(datetime_object.strftime('%m%d%H%M%S'))
            continue;
            # break;

    #i += 1000 * 2

plt.ylim(15, 30)

plt.xticks(range(0, len(dht), time_step), [ labels[j] for j in range(0, len(dht), time_step)], rotation = 30)
#plt.xticks(range(0,len(dht)),labels,rotation='vertical')
#ax = plt.add_subplot(111)

#ax.set_xlabel('xlabel')
plt.title('Sensor 1')
plt.ylabel('Temperature (Celsius)')


sensor1_line, = plt.plot(sensor1)
DHT_line, = plt.plot(dht)
heating_line, = plt.plot(heating)
# round_line, = plt.plot(round)

plt.legend([sensor1_line, DHT_line, heating_line], ['Sensor 1', 'DHT','heating'])

###

plt.figure()

plt.ylim(15, 30)

plt.xticks(range(0, len(dht), time_step), [ labels[j] for j in range(0, len(dht), time_step)], rotation = 30)

#ax = plt.add_subplot(111)

#ax.set_xlabel('xlabel')
plt.title('Sensor 2')
plt.ylabel('Temperature (Celsius)')


sensor2_line, = plt.plot(sensor2)
DHT_line, = plt.plot(dht)
heating_line, = plt.plot(heating)

plt.legend([sensor2_line, DHT_line, heating_line], ['Sensor 2', 'DHT','heating'])

#####

plt.figure()

plt.ylim(40, 100)

plt.xticks(range(0, len(dht), time_step), [ labels[j] for j in range(0, len(dht), time_step)], rotation = 30)

#ax = plt.add_subplot(111)

#ax.set_xlabel('xlabel')
plt.title('Humidity')
plt.ylabel('%')


humidity_line, = plt.plot(humidity)

plt.legend([humidity_line], ['Humidity'])

#####


# plt.figure()
#
# plt.xticks(range(0, len(dht), time_step), [ labels[j] for j in range(0, len(dht), time_step)], rotation = 30)
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
plt.ylim(450, 850)
plt.xticks(range(0, len(dht), time_step), [ labels[j] for j in range(0, len(dht), time_step)], rotation = 30)
#ax = plt.add_subplot(111)
#ax.set_xlabel('xlabel')
plt.title('LDR')
#plt.ylabel('%')
ldr_line, = plt.plot(ldr)
plt.legend([ldr_line], ['LDR'])

#####

plt.show()
