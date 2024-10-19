file_open = open('/mnt/f/fio_ssd_test.txt', encoding = 'utf8')

file_list = []
for line in file_open:
    file_list.append(line)

for line in file_list:
    if "bw" in line:
        print(line)
    if "iops" in line:
        print(line)
    if "latency" in line:
        print(line)
