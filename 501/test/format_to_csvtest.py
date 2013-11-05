input_file = open("501_1kB.out",'r')
output_file = open("501_1kB.csv",'a')
list_of_lines = []
for line in input_file:
    list_of_lines.append(str(line.replace("\n","")))
# first 18 lines are not useful so they will be deleted
list_of_lines = list_of_lines[1:]
# now since the time is the first 8 chars of a line we delete the rest
for i in range(len(list_of_lines)):
    list_of_lines[i] = list_of_lines[i][:8]

# now that we have the bare number we generate our output file.
output_file.write('"send","receive"\n') #Header line
iterations = len(list_of_lines)/2
for i in range(0,iterations-1):
    string = '"' + list_of_lines[i] + '","' + list_of_lines[i+iterations-1] + '"\n'
    output_file.write(string)

# now that everything is done we close the files
input_file.close()
output_file.close()
