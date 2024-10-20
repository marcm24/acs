# Project 3: SSD Performance Profiling

## Introduction
This project details the use of the [Flexible I/O Tester](https://github.com/axboe/fio) (FIO) to experiment on the performance of an SSD under varying conditions such as different access sizes, different read/write ratios, and different I/O queue depth. <br />

*Note: FIO Produces bandwidth data in MiB/s. To convert to MB/s we multiply the result by 1.049*

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

## Tests
The results obtained in the following tests were parsed and organized from the [python script](https://github.com/marcm24/acs/blob/main/project3/results.py) in the repository. It organizes the most critical information we want to obtain from the FIO tests including latency, bandwidth, and throughput. <br />

### Varying Data Access Sizes
During this experiment the parameters held constant are the read/write ratio at 50:50, and I/O queue depth of 1. <br />

The following data was obtained for an access size of 4KB: <br />
![image](https://github.com/user-attachments/assets/8c9ef416-5a33-4936-845f-f554e4a47d91) <br />
We can see for this access size we have an average read latency of 3.26μs and average write latency of 3.54μs. The average read and write bandwidth is 533MB/s. The throughput is 130,248 I/O per second (IOPS). <br />

For an access size of 16KB we find: <br />
![image](https://github.com/user-attachments/assets/83f405ed-a6fb-4595-a7a8-7c6c448a610c) <br />
We can see for this access size we have an average read latency of 5.14μs and average write latency of 5.81μs. The average read bandwidth is 1,377MB/s and average write bandwidth is 1,376MB/s. The throughput is 84,072 IOPS. <br />

For an access size of 32KB we find: <br />
![image](https://github.com/user-attachments/assets/4e99a5b6-febb-40b8-94a1-78b9dfbb49cf) <br />
We can see for this access size we have an average read latency of 7.57μs and average write latency of 8.78μs. The average read bandwidth is 1,888MB/s and average write bandwidth is 1,887MB/s. The throughput is 57,635 IOPS. <br />

For an access size of 128KB we find: <br />
![image](https://github.com/user-attachments/assets/694cc451-0488-4da9-a1c4-84dbf4f1f899) <br />
We can see for this access size we have an average read latency of 23.17μs and average write latency of 26.67μs. The average read bandwidth is 2,513MB/s and average write bandwidth is 2,512MB/s. The throughput is 19,193 IOPS. <br />

### Varying Read/Write Ratios
During this experiment the parameters held constant are the data access size at 32KB, and I/O queue depth at 1. <br />

For a read only operation we find: <br />
![image](https://github.com/user-attachments/assets/6dccb8cb-1f5b-495d-972e-5811ca6ce013) <br />
We can see for this operation we have an average read latency of 8.84μs. The average read bandwidth is 3,391MB/s. The throughput is 103,560 IOPS. <br />

For a read/write ratio of 70:30 we find: <br />
![image](https://github.com/user-attachments/assets/8e3feaac-287c-4d08-b329-95198895c530) <br />
We can see for this operation we have an average read latency of 7.61μs and write latency of 9.14μs. The average read bandwidth is 2,684MB/s and average write bandwidth is 1,151MB/s. The throughput is 35,153 IOPS. <br />

For a read/write ratio of 50:50 we find: <br />
![image](https://github.com/user-attachments/assets/1e279ddf-320f-4a35-8322-c603a69eedf7) <br />
We can see for this operation we have an average read latency of 7.62μs and write latency of 8.81μs. The average read bandwidth is 1,885MB/s and average write bandwidth is 1,884MB/s. The throughput is 57,558 IOPS. <br />

For a write only operation we find: <br />
![image](https://github.com/user-attachments/assets/bcc73870-279f-4abb-92c8-34da79242d22) <br />
We can see for this operation we have an average write latency of 10.4μs. The average write bandwidth is 2,929MB/s. The throughput is 89,473 IOPS. <br />

### Varying I/O Queue Depth
During this experiment the parameters held constant are the data access size at 32KB, and a read/write ratio of 50:50. <br />

For a queue depth of 1 we find: <br />
![image](https://github.com/user-attachments/assets/1e279ddf-320f-4a35-8322-c603a69eedf7) <br />
We can see for this queue depth we have an average read latency of 7.62μs and write latency of 8.81μs. The average read bandwidth is 1,885MB/s and average write bandwidth 1,884MB/s. The throughput is 57,558 IOPS. <br />

For a queue depth of 16 we find: <br />
![image](https://github.com/user-attachments/assets/76f97a5f-4bae-404f-87aa-9a0fd993f070) <br />
We can see for this operation we have an average read latency of 7.72μs and write latency of 8.87μs. The average read bandwidth is 1,859MB/s and average write bandwidth is 1,858MB/s. The throughput is 56,784 IOPS. <br />

For a queue depth of 64 we find: <br />
![image](https://github.com/user-attachments/assets/2039a163-5c07-438d-a0c4-dc7260ff639d) <br />
We can see for this queue depth we have an average read latency of 7.65μs and write latency of 8.84μs. The average read bandwidth is 1,872MB/s and average write bandwidth is 1,871MB/s. The throughput is 57,197 IOPS. <br />

For a queue depth of 256 we find: <br />
![image](https://github.com/user-attachments/assets/83ce8049-75de-4e96-9c7b-884d79dc8c29) <br />
We can see for this queue depth we have an average read latency of 7.65μs and write latency of 8.84μs. The average read bandwidth is 1,877MB/s and average write bandwidth is 1,876MB/s. The throughput is 57,301 IOPS. <br />


## Analysis
Organizing the data obtained in the previous section we can analyze the results:
