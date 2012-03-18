import traceback
import serial
import sys
import time
import math
import datetime
import urllib
from subprocess import Popen, PIPE
from hdd_temp import hdd_temp

log = open("/tmp/MRAC.log",'w')

hddt=hdd_temp()

#Comms Parameters
SEND_RATE = 5  # secs

Td=[36,36,36,36]
#Td=[30,30,30,30]

Error=0

i = 1

def init_ser():

	S=0
	try:
	        x = serial.Serial('/dev/ttyUSB0',38400,timeout=.1)
		x.flushInput()
	except:
	        print "USB0 Failed... Trying USB1\n"
	        S=1
	
	if S==1:
	        try:
	                x = serial.Serial('/dev/ttyUSB1',38400,timeout=.1)
			x.flushInput()
	        except:
	                print "USB1 Failed... Disabaling Serial Output\n"
	                S=2
	return x

print "Opening Serial Port..."
x=init_ser()

while i:

	try:

		rx = x.readlines()  #or x.read() for one byte
		if rx:
 			print rx

			
		t=(math.sin(i*3.1415/360)+1)*40
		i+=1

		#Temps=hddt.get_Temps()
		Temps=[[t,t,t,t],[t,t,t,t],[t,t,t,t],[t,t,t,t]]
                pstr="%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d,%02d\0" % (Temps[0][0],Temps[0][1],Temps[0][2],Temps[0][3],Temps[1][0],Temps[1][1],Temps[1][2],Temps[1][3],Temps[2][0],Temps[2][1],Temps[2][2],Temps[2][3],Temps[3][0],Temps[3][1],Temps[3][2],Temps[3][3],Td[0],Td[1],Td[2],Td[3])
		x.flushInput()
		print pstr
		tx = x.write(pstr)
		# Get output!
		duty=[-1,-1,-1,-1]

		try:
			for n in range(0, 4):
				time.sleep(1.00/100)
				rx = x.readline()  #or x.read() for one byte
				print rx
				sys.stdout.flush()
				out1=rx.split(' ')
				j=int(out1[1][0:-1])
				duty[j-1]=float(out1[2][0:-1])/255.0*100.0
			x.flushInput()
			dstr="F1:%3.2f F2:%3.2f F3:%3.2f F4:%3.2f" %(duty[0],duty[1],duty[2],duty[3])
			print dstr

			if min(duty) > -1:
				f = open("/dev/shm/duty.log", 'w')
				f.write(dstr)
				f.close()
				Error = 0
			else:
				print "min(duty) > -1 was not satisfied"
		except:
			print "Failed to read output..."
			Error=Error+1;
			if Error >= 4:
				Error = 0
				print "Restarting Serial..."
				cmd='echo "There has been a communications error on the USART. Please check the log." | mail -s "[FBOX] USART Failure on FC!" strefli3@gmail.com'
				O=Popen(cmd, shell=True, stdout=PIPE, stderr=PIPE)
				x=init_ser()
		
		time.sleep(SEND_RATE)


	except (KeyboardInterrupt, SystemExit):

		print 'Caught Ctrl+C...'
		print 'Closing the Logfile...'
		f.close()
		print 'Killing Python...'
		sys.exit()

	except:
	        # report error and proceed
		sys.exit()	
		traceback.print_exc()
		print 'Something went very wrong'
