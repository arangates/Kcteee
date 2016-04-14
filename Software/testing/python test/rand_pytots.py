#This python script generates random data and updates fields of Thingspeak channel;

import httplib, urllib
import random
from time import localtime, strftime

import time																					

def doit():
	# voltage
	a= random.randint(210, 220) #v1
	b= random.randint(210, 230) #v2
	c= random.randint(210, 240) #v3

	#current
	d= random.randint(5,7) #c1
	e= random.randint(5,17) #c2
	f= random.randint(5,27) #c3

	#speed
	g= random.randint(1100,1200) 

	#temperature
	h= random.randint(38, 40) 
	
	params = urllib.urlencode({'field1': a ,'field2': b,'field3': c ,'field4': d,'field5': e ,'field6': f,'field7': g ,'field8': h,'key':'ETWI0YZCDOHZA6BN'})
	headers = {"Content-type": "application/x-www-form-urlencoded","Accept": "text/plain"} #HHTP HEADERS FOR THEIR SERVER
	conn = httplib.HTTPConnection("api.thingspeak.com:80") #opening a connection in port 80
	
	try:
		conn.request("POST", "/update", params, headers)
		response = conn.getresponse()
		print a
		print b
		print c
		print d
		print e
		print f
		print g
		print h
		#print strftime("%a, %d %b %Y %H:%M:%S", localtime())
		print response.status, response.reason
		data = response.read()
		conn.close()
	except:
		print "connection failed"	

#sleep for 16 seconds (api limit of 15 secs)
if __name__ == "__main__":
	while True:
		doit()
		time.sleep(15) 

