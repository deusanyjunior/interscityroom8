# read dataLDR.csv and plot data
import matplotlib
import matplotlib.pyplot as plt
import numpy as np
from scipy import stats
from datetime import datetime

csvfile = open("sensorsRoom8-2018-06-11T17_38_46.876341Z.csv")

datax = []
dataRound = []
dataLDR = []
dataLux = []
dataHum = []
dataTem = []
dataAds01 = []

for line in csvfile:

    data = line.split(',')
    if len(data) < 6:
        continue;
    if data[2].startswith("sensor"):
        continue;
    datax.append(datetime.strptime(data[0],'%Y-%m-%dT%H:%M:%S.%fZ'))
    dataRound.append(int(data[1])/4)
    dataLDR.append(int(data[2]))
    dataLux.append(int(data[3]))
    dataHum.append(float(data[4]))
    # dataHum.append((float(data[4])-55)*40)
    dataTem.append(float(data[5]))
    dataAds01.append(float(data[6]))

# print "timestamps"
# print datax

print "LDR"
# print dataLDR
print stats.describe(dataLDR)

# print "Lux"
# print dataLux
# print stats.describe(dataLux)

print "Humidity"
# print dataHum
print stats.describe(dataHum)

print "Temperature"
# print dataTem
print stats.describe(dataTem)

print "data read"

print "plotting data.."

# plt.plot(datax,dataLDR,'g-')
# plt.plot(datax,dataLux,'r-')
# plt.plot(datax,dataHum,'g-')
# plt.plot(datax,dataTem,'r-')
plt.plot(datax,dataAds01,'r-',datax,dataRound,'g-')

# plt.plot(datax,dataLDR,'r-',datax,dataLux,'g-',datax,dataHum,'b-',datax,dataTem,'y-')

# plt.ylabel('Sensors values: LDR(r),Lux(g),Hum(b),Tem(y)')

plt.show()

print "done!"
