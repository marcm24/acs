# Project 3: SSD Performance Profiling

## Introduction
This project details the use of the [Flexible I/O Tester](https://github.com/axboe/fio) (FIO) to experiment on the performance of an SSD under varying conditions such as different access sizes, different read/write ratios, and different I/O queue depth. <br />

*Note: FIO Produces data in MiB/s. To convert to MB/s we multiply the result by 1.049*

## System Specifications
Partitioned 5GB on a 1TB NVMe SSD (WD Black SN750 SE)  for FIO Testing: <br />
![image](https://github.com/user-attachments/assets/ec2787d0-0135-4a58-af2c-71be8c7cbe28) <br />

- Sequential read speeds up to 3600 MB/s 
- Utilizes PCIe Gen4 interconnect

## FLexible I/O Tester (FIO) Parameters
To conduct tests with Flexible I/O Tester (FIO) we can utilzie the following command: <br />
```
 fio --name=ssd_test --directory=/mnt/f --ioengine=sync --rw=OPERATION --rwmixread=RATIO --bs=BLOCK_SIZE --size=READ_WRITE_SIZE --numjobs=1 --runtime=60 --time_based --iodepth=IO_QUEUE_DEPTH --output-format=normal --output=fio_ssd_test.txt
``` 
Where we can define: <br />
`OPERATION = randread, randwrite` To specify a random read or random write and `RATIO = 50, 70` to specify the read ratio. The ratio can be omitted if we are performing a read or write only operation. <br />

`BLOCK_SIZE = 4k, 16k, 32k, 128k` To specify the size of data being read/written for each I/O operation. <br />

`READ_WRITE_SIZE = 1G` To specify the size of data being read or written during the job for the experiment. <br />

`IO_QUEUE_DEPTH = 1, 16, 64, 256` To specify the amount of I/O operations queued at a time. <br />

This command will perform 1 job called "ssd_test" that will run for 60 seconds and provide  bandwidth, latency, and throughput numbers. <br />

## Latency Tests
### Varying Data Access Sizes
During this experiment the parameters held constant are the operation at 50:50, and I/O queue depth at 1. <br />

The following data was obtained for an access size of 4KB: <br />

For an access size of 16KB we find: <br />

For an access size of 32KB we find: <br />

For an access size of 128KB we find: <br />


*insert plot*
### Varying Read/Write Ratios
During this experiment the parameters held constant are the data access size at 32KB, and I/O queue depth at 1. <br />

For a read only operation we find: <br />
![image](https://github.com/user-attachments/assets/6dccb8cb-1f5b-495d-972e-5811ca6ce013) <br />
We can see for this operation we have an average latency of 8.84μs <br />

For a read/write ratio of 70:30 we find: <br />
![image](https://github.com/user-attachments/assets/8e3feaac-287c-4d08-b329-95198895c530) <br />
We can see for this operation we have an average read latency of 7.61μs and write latency of 9.14μs <br />

For a read/write ratio of 50:50 we find: <br />

We can see for this operation we have an average read latency of and write latency of <br />

*insert plot*
### Varying I/O Queue Depth

## Bandwidth Tests
### Varying Data Access Sizes
### Varying Read/Write Ratios
### Varying I/O Queue Depth

## Analysis
