import httplib, urllib
import random
from time import localtime, strftime
# download from http://code.google.com/p/psutil/
#import psutil
import time

def doit():
	a=random.randint(210, 230)
	b= random.randint(4, 10)
	c= random.randint(1100,1250)
	d= random.randint(25, 30)
	# e= random.randint(500, 1070045)
	# f= random.randint(560, 670560)
	# g= random.randint(5090, 10000)
	# h= random.randint(800, 2208)
	
	params = urllib.urlencode({'field4': a ,'field5': b,'field6': c ,'field7': d,'key':'ETWI0YZCDOHZA6BN'})
	headers = {"Content-type": "application/x-www-form-urlencoded","Accept": "text/plain"}
	conn = httplib.HTTPConnection("api.thingspeak.com:80")
	
	try:
		conn.request("POST", "/update", params, headers)
		response = conn.getresponse()
		print a
		print b
		print c
		print d
		# print e
		# print f
		# print g
		# print h
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
		time.sleep(16) 

