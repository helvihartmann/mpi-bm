for n in range(0, 12):
    data = 2**n
    data = str(data)
    filename = "502_"
    filename+= data
    outname = filename
    outname+="kB.csv"
    filename+="kB.out"
    print(filename)
    #print(outname)
    input_file = open(filename,'r')
    output_file = open('502out.csv','a')
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
    linesofinfo = 5
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
    sendmeantime = str(send/len(list_of_sendtimes))
    recvmeantime = str(recve/len(list_of_recvtimes))

    # write out file
    if n == 0:
        output_file.write('Byte   send meantime     recv meantime    load average last minute   last 5 min      last 15 min\n')
    meantime = list_of_lines[0][:5] +" " + sendmeantime +" "+ recvmeantime
    text = meantime+"              " + list_of_lines[1] + "     "+list_of_lines[2] + "       "+list_of_lines[3] + '\n'
    output_file.write(text)


    # now that everything is done we close the files
    input_file.close()
    output_file.close()
