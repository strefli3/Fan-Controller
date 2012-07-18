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

                        # Fan Bank 0
                        FBA=[0,0,0,0]
                        print self.Temps
                        try:
                                FBA[0]=min([x for x in self.Temps[1] if x !=0])
                        except:
                                FBA[0]=0

                        # Fan Bank 1 and 2

                        try:
                                FBA[1]=min([x for x in sum([self.Temps[0],self.Temps[2]],[]) if x != 0])
                        except:
                                FBA[1]=0

                        try:
                                FBA[2]=min([x for x in sum([self.Temps[1],self.Temps[3]],[]) if x != 0])
                        except:
                                FBA[2]=0

			 # Fan Bank 3
                        try:
                                FBA[3]=min([x for x in self.Temps[2] if x !=0])
                        except:
                                FBA[3]=0

                        print FBA

                        #if self.Temps[0][0] > FBA[0] or self.Temps[0][0] == 0:
                        #       self.Temps[0][0] = FBA[0]
                        #if self.Temps[1][0] > FBA[1] or self.Temps[1][0] == 0:
                        #       self.Temps[1][0] = FBA[1]
                        #if self.Temps[2][0] > FBA[2] or self.Temps[2][0] == 0:
                        #       self.Temps[2][0] = FBA[2]
                        #if self.Temps[3][0] > FBA[3] or self.Temps[3][0] == 0:
                        #       self.Temps[3][0] = FBA[3]

                        if self.Temps[0][0] < FBA[0]:
                                self.Temps[0][0] = FBA[0]
                        if self.Temps[1][0] < FBA[1]:
                                self.Temps[1][0] = FBA[1]
                        if self.Temps[2][0] < FBA[2]:
                                self.Temps[2][0] = FBA[2]
                        if self.Temps[3][0] < FBA[3]:
                                self.Temps[3][0] = FBA[3]


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
		




