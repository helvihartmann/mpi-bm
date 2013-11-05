for n in range(0, 12):
    data = 2**n
    data = str(data)
    filename = "502_"
    filename+= data
    outname = filename
    outname+="kB.csv"
    filename+="kB.out"
    print(filename)
    print(outname)
    input_file = open(filename,'r')
    output_file = open(outname,'a')
    list_of_lines = []
    list_of_times = []
    for line in input_file:
        list_of_lines.append(str(line.replace("\n","")))
    
    #read data amount from first lines (first 8 characters)
#list_of_lines = list_of_lines[1:8]
    # first 18 lines are not useful so they will be deleted
#   list_of_lines = list_of_lines[5:]

    # now since the time is the first 8 chars of a line we delete the rest
    for i in range(len(list_of_lines)):
        list_of_lines[i] = list_of_lines[i] #[:8]

    # now that we have the bare number we generate our output file.
    # write information about sended data and load average first
    linesofinfo = 5
    for i in range(0,linesofinfo):
        info = '"'+list_of_lines[i] +'"\n'

        output_file.write(info)

    # write time for send/recv
    output_file.write('"send","receive"\n') #Header line
    iterations = (len(list_of_lines)-linesofinfo)/2
    for i in range(linesofinfo,iterations-1):
        list_of_times.append(list_of_lines[i][:8])
        string = '"' + list_of_lines[i][:8] + '","' + list_of_lines[i+iterations-1][:8] + '"\n'
        output_file.write(string)
#import scipy
#   meantime= scipy.mean(list_of_times)
#meantime = (sum(list_of_times)/len(list_of_times))
#output_file.write('\n')
#output_file.write('\n',meantime)


    # now that everything is done we close the files
    input_file.close()
    output_file.close()
