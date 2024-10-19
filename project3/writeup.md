# Project 3: SSD Performance Profiling

## Introduction
This project details the use of the [Flexible I/O Tester](https://github.com/axboe/fio) (FIO)

## System Specifications
Partitioned 5GB on a 1TB NVMe SSD (WD Black SN750 SE)  for FIO Testing: <br />
![image](https://github.com/user-attachments/assets/ec2787d0-0135-4a58-af2c-71be8c7cbe28) <br />

- Sequential read speeds up to 3600 MB/s 
- Utilizes PCIe Gen4 interconnect

## FLexible I/O Tester (FIO) Parameters
To conduct tests with Flexible I/O Tester (FIO) we can utilzie the following command: <br />
```
 fio --name=ssd_test --directory=/mnt/f --ioengine=sync --rw=OPERATION,ratio=RATIO --bs=BLOCK_SIZE --size=READ_WRITE_SIZE --numjobs=1 --runtime=60 --time_based --iodepth=IO_QUEUE_DEPTH --output-format=normal --output=fio_ssd_test.txt
``` 
Where we can define: <br />
`OPERATION = randread, randwrite` To specify a random read or random write and `RATIO = 50:50, 70:30` to specify the read/write ratio. The ratio can be omitted if we are performing a read or write only operation. <br />

`BLOCK_SIZE = 4k, 16k, 32k, 128k` To specify the size of data being read/written for each I/O operation. <br />

`READ_WRITE_SIZE = 1G` To specify the size of data being read or written during the job for the experiment. <br />

`IO_QUEUE_DEPTH = 1, 16, 64, 256` To specify the amount of I/O operations queued at a time. <br />

This command will perform 1 job called "ssd_test" that will run for 60 seconds and provide  bandwidth, latency, and throughput numbers. <br />

## Latency Tests
### Varying Data Access Sizes
### Varying Read/Write Ratios
### Varying I/O Queue Depth

## Bandwidth Tests
### Varying Data Access Sizes
### Varying Read/Write Ratios
### Varying I/O Queue Depth

## Analysis
