from subprocess import Popen, PIPE

class hdd_temp:

        def __init__(self):

		self.Temps=[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]

	def hddget(self):


		cmd='hddtemp /dev/sd[abcdefg]'	
	        hddCMD=Popen(cmd, shell=True, stdout=PIPE, stderr=PIPE)
     		stdout, stderr = hddCMD.communicate()

		
        	hdbuf=stdout+stderr
	        return hdbuf

	def hdd_split(self,str):

		self.Temps=[[0,0,0,0],[0,0,0,0],[0,0,0,0],[0,0,0,0]]

		TTable={}
		
	        sbuf=str.split('\n')
		sbuf.pop()	
	        for i in range(len(sbuf)):
        	        hbuf=sbuf[i].split(':')

			if hbuf[2][1:3].isdigit():
				temp=hbuf[2][1:3]
			else:
				temp=00

			TTable[hbuf[0]]=temp

	        if len(TTable) == 7:
        	        self.Temps[2][0]=int(TTable['/dev/sda'])
        	        self.Temps[2][1]=int(TTable['/dev/sdb'])
        	        self.Temps[2][2]=int(TTable['/dev/sdc'])

        	        self.Temps[1][0]=int(TTable['/dev/sdd'])
        	        self.Temps[1][1]=int(TTable['/dev/sde'])
        	        self.Temps[1][2]=int(TTable['/dev/sdf'])
        	        self.Temps[1][3]=int(TTable['/dev/sdg'])

			#Lets add some cross talk to see what happens...

			#self.Temps[0][0]=int(min(temp))
			#self.Temps[3][0]=int(min(temp))

		else:
			self.Temps=0

	        return self.Temps

	def get_Temps(self):
		self.Temps=0
		while self.Temps == 0:
			str=self.hddget()
        	        self.Temps=self.hdd_split(str)
			Temps=self.Temps
			print "Try"
		return Temps
		




