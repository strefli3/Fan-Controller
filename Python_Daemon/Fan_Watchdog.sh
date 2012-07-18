#!/bin/sh

file=/dev/shm/duty.log

TO=300

Current_time=$(date +%s)

if [ -f $file ];
	then

		echo "The log file exists..."

		File_time=$(stat -c %Z $file)
		delta=$((Current_time-File_time))
		echo $delta
		if (( "$delta" > "$TO" ));
			then
				echo "Looks like Python crashed, restarting..."
				py_pid=$(ps -Alf | grep HDD_Temp_Push.py | grep python |awk '{print $4}')
				kill -9 $py_pid
				#python /home/strefli3/Fan-Controller/Python_Daemon/HDD_Temp_Push.py &
				python /home/strefli3/Fan-Controller/Python_Daemon/HDD_Temp_Push.py > /tmp/MRAC_"$Current_time".log &
				echo "The Cron job has found a problem with python. I've restarted it,  but you should check the log. The file delta was $delta." | mail -s "[FBOX] USART Failure on FC!" strefli3@gmail.com
			else
				echo "Python looks stable"
			fi
	else
		echo "File does not exist... Starting python..."
		py_pid=$(ps -Alf | grep HDD_Temp_Push.py | grep python |awk '{print $4}')
		kill -9 $py_pid
		#python /home/strefli3/Fan-Controller/Python_Daemon/HDD_Temp_Push.py &
		python /home/strefli3/Fan-Controller/Python_Daemon/HDD_Temp_Push.py > /tmp/MRAC_"$Current_time".log &
		echo "The Cron job has found a problem with python. I've restarted it,  but you should check the log. The duty file was missing!" | mail -s "[FBOX] USART Failure on FC!" strefli3@gmail.com

fi

