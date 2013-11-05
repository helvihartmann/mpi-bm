import math
import os

r = 3
iterations2 = 10000000

for n in range(7, 20):
    data = 2**n
    data = str(data)
    version = str(503)
    filename=version
    filename+= "_"
    filename+= data
    #outname = "/Desktop/CBM/myfiles/"+version +"/"+version
    outname = version

    outname1=outname+"timeout.csv"
    outname2=outname+"loadout.csv"
    outname3=outname+"rateout.csv"
    filename+="B.out"
    print(filename)
    #print(outname)
    input_file = open(filename,'r')
    output_time_file = open(os.path.abspath(os.path.join('..','..','..','tex',version,outname1)),'wb')
    output_load_file = open(outname2,'w+')
    output_rate_file = open(outname3,'w+')
    list_of_lines = []
    list_of_sendtimes = []
    list_of_recvtimes = []
    for line in input_file:
        list_of_lines.append(str(line.replace("\n","")))


    # now since the time is the first 8 chars of a line we delete the rest
    for i in range(len(list_of_lines)):
        list_of_lines[i] = list_of_lines[i][:8]



    # first 5 lines are Bytes send and load average....skip over them to calculate send/recv average time
    # write time for send/recv
    # output_file.write('"send","receive"\n') #Header line
    linesofinfo = 6
    iterations = (len(list_of_lines)-linesofinfo)/2
    k=0
    for i in range(linesofinfo,iterations-1):
        
        list_of_sendtimes.append(list_of_lines[i])
        list_of_recvtimes.append(list_of_lines[i+iterations-1])
        
        # if send/recv want to be printed
        #string = '"' + list_of_sendtimes[k] + '","' + list_of_recvtimes[k] + '"\n'
        #output_file.write(string)
        
        # sum over all times
        send = float(list_of_sendtimes[k])
        recve = float(list_of_recvtimes[k])
        send = send + send
        recve = recve + recve
        k = k+1

    #calculate mean
    send_meantime = str(send/len(list_of_sendtimes))
#import scipy
#sendmeantime = str(scipy.mean(list_of_sendtimes))

    recv_meantime = str(recve/len(list_of_recvtimes))

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

    # write out file
    if n == 7:
        output_time_file.write('# send total package size   send meantime  sigma   recv meantime   sigma  \n')
        output_load_file.write('# Byte load average last minute   last 5 min      last 15 min\n')
        output_rate_file.write('# Byte Datenrate sent sigma Datenrate received sigma\n')

    if n == 8 or n==12 or n==15 or n==18:
        r = r + 1

    iterations2 = iterations2/2
    byte = int(list_of_lines[0][:r])
    send_total = iterations2*byte
    send_rate = send_total/float(send_meantime)
    recv_rate = send_total/float(recv_meantime)
    send_rate_varianz = send_varianz*send_total
    recv_rate_varianz = recv_varianz*send_total

    meantime = str(send_total) +" "+list_of_lines[0][:r] +" " + send_meantime +" "+ str(send_varianz) +" "+ recv_meantime +" "+ str(recv_varianz) + '\n'
    loadtime = list_of_lines[0][:r] + "              " + list_of_lines[1] + "     "+list_of_lines[2] + "       "+list_of_lines[3] + '\n'
    rate = list_of_lines[0][:r] + " " + str(send_rate)+ "  "+ str(send_rate_varianz) +" " +str(recv_rate) +" " + str(recv_rate_varianz)+ '\n'
    output_time_file.write(meantime)
    output_load_file.write(loadtime)
    output_rate_file.write(rate)


    # now that everything is done we close the files
    input_file.close()
    output_time_file.close()
    output_load_file.close()
