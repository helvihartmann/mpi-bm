# 12.11.13 can ready output from generation 503 on (128B-1MiB)
# version and pfad need to be specified to find the out files
# creates csv files into CBM/tex/version
# reads every .out file in a directory and gets iterations to calculate total data sent from file
# and gets byte number from string (no more need to define size of int)
# and gets iterations 2 from outfile....CAUTION: not all outfiles adopted (only 503 I,II, 503r)
import math
import os
import re
l=0;
version = str(503)
pfad = version+'/r1024'
filenameList = os.listdir(os.path.join(pfad))
print(filenameList)
for n in filenameList:
    l=l+1;
    if n.find('.out')!=-1 and n.find('._')==-1:# returns index if found and -1 otherwise
        print(n +' check')
        list_of_lines = []
        list_of_sendtimes = []
        list_of_recvtimes = []
    
# OUTPUT
        # create output files
        outname = version
        outname1=outname+"timeout.csv"
        outname1 = os.path.abspath(os.path.join('..','tex',version,outname1))
        outname2=outname+"loadout.csv"
        outname2 = os.path.abspath(os.path.join('..','tex',version,outname2))
        outname3=outname+"rateout.csv"
        outname3 = os.path.abspath(os.path.join('..','tex',version,outname3))
        output_time_file = open(outname1,'a')
        output_load_file = open(outname2,'a')
        output_rate_file = open(outname3,'a')
    
# INPUT
        #filename=os.path.join(pfad,filename)
        filename=os.path.join(pfad,n)
        input_file = open(filename,'r')
        
        # collect all lines in input files
        for line in input_file:
            list_of_lines.append(str(line.replace("\n","")))


        # now since the time is the first 8 chars of a line we delete the rest
        for i in range(len(list_of_lines)):
            if i!=0:
                list_of_lines[i] = list_of_lines[i][:8]


# CALCULATION: first 6 lines are information...skip over them to calculate send/recv average time
        # write time for send/recv
        # output_file.write('"send","receive"\n') #Header line
        linesofinfo = 7
        iterations = (len(list_of_lines)-linesofinfo)/2
        k=0
        send = 0;
        recve = 0;
        for i in range(linesofinfo,iterations-1+linesofinfo):
            list_of_sendtimes.append(list_of_lines[i])
            list_of_recvtimes.append(list_of_lines[i+iterations-1])
            send_tmp = float(list_of_sendtimes[k])
            recve_tmp = float(list_of_recvtimes[k])
            #print(send_tmp)
            send = send + send_tmp
            recve = recve + recve_tmp
            k = k+1
        send_meantime = str(send/len(list_of_sendtimes))
        recv_meantime = str(recve/len(list_of_recvtimes))

        # calculate varianz
        send_varianz = 0
        recv_varianz = 0
        for k in range(0,len(list_of_sendtimes)):
            send = float(list_of_sendtimes[k])
            recve = float(list_of_recvtimes[k])
            send_diff = ((float(send_meantime)-send)**2)
            recv_diff = ((float(recv_meantime)-recve)**2)
            send_varianz = send_varianz + send_diff
            recv_varianz = recv_varianz+recv_diff

        send_varianz = math.sqrt(send_varianz)/k
        recv_varianz = math.sqrt(recv_varianz)/k


        iterations2 = int(re.match(r'\d+', list_of_lines[2]).group())
        byte = re.match(r'\d+', list_of_lines[1]).group()
        timestemp = list_of_lines[0]
        send_total = iterations2*int(byte)

        # calculate rate
        send_rate = send_total/float(send_meantime)
        recv_rate = send_total/float(recv_meantime)
        send_rate_varianz = (send_varianz/float(send_meantime))*send_rate
        recv_rate_varianz = (recv_varianz/float(recv_meantime))*recv_rate

# WRITE
    # Key
        if l == 1:
            output_time_file.write(('#' + timestemp+'\n# send total package size  send meantime  sigma   recv meantime   sigma  \n'))
            output_load_file.write(('#' + timestemp+'\n #Byte load average last minute   last 5 min      last 15 min\n'))
            output_rate_file.write(('#'+ timestemp+'\n # Byte Datenrate sent sigma Datenrate received sigma\n'))
    # Data
        meantime = str(send_total) +" "+byte +" " + send_meantime +" "+ str(send_varianz) +" "+ recv_meantime +" "+ str(recv_varianz) + '\n'
        loadtime = byte + "              " + list_of_lines[2] + "     "+list_of_lines[3] + "       "+list_of_lines[4] + '\n'
        rate = byte + " " + str(send_rate)+ "  "+ str(send_rate_varianz) +" " +str(recv_rate) +" " + str(recv_rate_varianz)+ '\n'
        output_time_file.write(meantime)
        output_load_file.write(loadtime)
        output_rate_file.write(rate)


#CLOSE
        input_file.close()
output_time_file.write(('\n'+'\n'))
output_load_file.write(('\n'+'\n'))
output_rate_file.write(('\n'+'\n'))
output_time_file.close()
output_load_file.close()