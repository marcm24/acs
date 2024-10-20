# Project 3: SSD Performance Profiling

## Introduction
This project details the use of the [Flexible I/O Tester](https://github.com/axboe/fio) (FIO) to experiment on the performance of an SSD under varying conditions such as different access sizes, different read/write ratios, and different I/O queue depth. <br />

*Note: FIO Produces bandwidth data in MiB/s. To convert to MB/s we multiply the result by 1.049*

## System Specifications
Partitioned 5GB on a 1TB NVMe SSD (WD Black SN750 SE)  for FIO Testing: <br />
![image](https://github.com/user-attachments/assets/ec2787d0-0135-4a58-af2c-71be8c7cbe28) <br />

- Sequential read speeds up to 3600 MB/s 
- Sequential write speeds up to 2830 MB/s
- Random read performance of 525000 4KB IOPS
- Random write performance of 640000 4KB IOPS
- Utilizes PCIe Gen4 interconnect <br />

[Device Specification Source](https://www.westerndigital.com/en-ae/products/internal-drives/wd-black-sn750-se-nvme-ssd?sku=WDS100T1B0E)

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
The results obtained in the following tests were parsed and organized from the [python script](https://github.com/marcm24/acs/blob/main/project3/results.py) in the repository. It organizes the most critical information we want to obtain from the FIO tests including latency, bandwidth, and throughput. For all tests the runtime is held constant at 60 seconds. Other parameters that are held constant and/or changed are detailed within the test section. <br />

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
### Data Access Size Analysis
We can analyze the effect on latency through the following plot: <br />
![image](https://github.com/user-attachments/assets/e6ddc8f2-6a46-4a40-a4e9-24b4de07a202) <br />

We can analyze the effect on bandwidth through the following plot: <br />
![image](https://github.com/user-attachments/assets/b47578d4-5bb4-420f-8609-46c8bb88ac51) <br />

We can analyze the effect on throughput through the following plot: <br />
![image](https://github.com/user-attachments/assets/bd620090-79d2-4d4c-a3f6-a0a2053fb554) <br />

From these results we can see that as we increase the data access size the latency and bandwidth increase. This lines up as for the latency, it takes longer for a single I/O operation to occur with more data being transferred. Since we are transferring more data the bandwidth will increase to reflect that, as it is a measurement of the amount of data we transfer per second. Despite the increase in amount of data we are transferring, because there is an increase in latency we can see that there is a decrease in the throughput as we increase the data access size. <br />

### Read/Write Ratio Analysis
We can analyze the effect on latency through the following plot: <br />
![image](https://github.com/user-attachments/assets/a7067f01-d16e-4f85-9330-61d80c7afb80) <br />

We can analyze the effect on bandwidth through the following plot: <br />
![image](https://github.com/user-attachments/assets/235be05a-1850-4bea-9873-ba3db8e100fa) <br />

We can analyze the effect on throughput through the following plot: <br />
![image](https://github.com/user-attachments/assets/c79386d7-5e87-412a-a553-95012e114cd7) <br />

From these results we can see that as we decrease the read ratio the read latency decreases, and as we increase the write ratio the write latency increases. As we increase the amount of write operations we are performing it takes longer for them to occur, similarly with the read operations. The same applies to  the bandwidth, as we have a higher read ratio the read bandwidth increases, and as we have a higher write ratio the write bandwidth increases. From both of these results we can gather that read operations are easier to perform as the read latency is less than that of the write latency, and the read bandwidth is higher than that of the write bandwidth. As a result of this we find that the throughput is maximized at read only operations, where it drastically decreases when performing a majority read and write (70:30 R/W). From that point it increases as we move towards a write only operation, but the throughput at a write only operation is still less than that of a read only. <br />
### I/O Queue Depth Analysis
We can analyze the effect on latency through the following plot: <br />
![image](https://github.com/user-attachments/assets/e30d33ca-a60c-4896-afc4-8281b34e0b83) <br />

We can analyze the effect on bandwidth through the following plot: <br />
![image](https://github.com/user-attachments/assets/e58d1d8c-f43b-46be-8306-f8202eac986e) <br />

We can analyze the effect on throughput through the following plot: <br />
![image](https://github.com/user-attachments/assets/9eba0e36-6efa-409d-b5fe-13d9cdbbadc9) <br />

From these results we can see that as we increase the I/O queue depth the read and write latency largely remain the same. As we were able to analyze from the previous results, the write latency is higher than the read latency. The queue depth of 16 is the only point in which the latency for both reads and writes increase. This point is an outlier for the bandwidth and throughput tests as well. For the bandwidth, as we increase the queue depth we can see an overall decrease. As we increase from a queue depth of 16 the bandwidth increases slightly which lines up as there are more operations queued and more operations to occur per second. The overall decrease comes from the test time of 60 seconds being uniform throughout the tests, and there being more operations waiting to occur and ultimately not being able to occur in that time. The throughput follows the same trend as the bandwidth, with a drastic decrease at a queue depth of 16 and slight increase past that point. Overall, the larger the queue depth the bandwidth and throughput are still less than that of a queue depth of 1. <br />
