file_open = open('/mnt/f/fio_ssd_test.txt', encoding = 'utf8')

file_list = []
for line in file_open:
    file_list.append(line)

for line in file_list:
    if "read" in line: # read information
        print(line)
    if "write" in line: # write information
        print(line)
    if "lat (usec)" in line: # latency
        print(line)
    if "bw" in line: # bandwidth
        print(line)
    if "iops" in line: # throughput
        print(line)
