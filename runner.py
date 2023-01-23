import subprocess
import statistics
import math

no_of_samples = 100

def compileAll():
    subprocess.call(['gcc', '-g', '-Wall', '-o', 'linked_list_serial', 'linked_list_serial.c'])
    subprocess.call(['gcc', '-g', '-Wall', '-o', 'linked_list_mutex', 'linked_list_mutex.c', '-lm', '-lpthread'])
    subprocess.call(['gcc', '-g', '-Wall', '-o', 'linked_list_read_write_locks', 'linked_list_read_write_locks.c', '-lm', '-lpthread'])


#execute the program no_of_samples times and return the average time and standard deviation
def execute(command):
    times = []
    for i in range(no_of_samples):
        time = subprocess.Popen(command, stdout=subprocess.PIPE).communicate()[0]
        elapsedTimes.append(float(time))
    
    average = statistics.mean(times)
    standardDeviation = statistics.stdev(times)
    samples = math.ceil(math.pow((1.96 * standardDeviation) / (0.05 * average), 2))

    print("Average time: ", average)
    print("Standard Deviation: ", standardDeviation)
    print("No of samples: ", samples)

def executeCommands(commandList):
    for command in commandList:
        print("Executing: ", command)
        print('No of threads: ', command[6])
        execute(command)

serial_cases = [['./linked_list_serial', '1000', '10000', '0.99', '0.005', '0.005'], ['./linked_list_serial', '1000', '10000', '0.90', '0.05', '0.05'], ['./linked_list_serial', '1000', '10000', '0.50', '0.25', '0.25']]
mutex_case1 = [['./linked_list_mutex', '1000', '10000', '0.99', '0.005', '0.005', '1'], ['./linked_list_mutex', '1000', '10000', '0.99', '0.005', '0.005', '2'], ['./linked_list_mutex', '1000', '10000', '0.99', '0.005', '0.005', '4'], ['./linked_list_mutex', '1000', '10000', '0.99', '0.005', '0.005', '8']]
mutex_case2 = [['./linked_list_mutex', '1000', '10000', '0.90', '0.05', '0.05', '1'], ['./linked_list_mutex', '1000', '10000', '0.90', '0.05', '0.05', '2'], ['./linked_list_mutex', '1000', '10000', '0.90', '0.05', '0.05', '4'], ['./linked_list_mutex', '1000', '10000', '0.90', '0.05', '0.05', '8']]
mutex_case3 = [['./linked_list_mutex', '1000', '10000', '0.50', '0.25', '0.25', '1'], ['./linked_list_mutex', '1000', '10000', '0.50', '0.25', '0.25', '2'], ['./linked_list_mutex', '1000', '10000', '0.50', '0.25', '0.25', '4'], ['./linked_list_mutex', '1000', '10000', '0.50', '0.25', '0.25', '8']]

read_write_case1 = [['./linked_list_read_write_locks', '1000', '10000', '0.99', '0.005', '0.005', '1'], ['./linked_list_read_write_locks', '1000', '10000', '0.99', '0.005', '0.005', '2'], ['./linked_list_read_write_locks', '1000', '10000', '0.99', '0.005', '0.005', '4'], ['./linked_list_read_write_locks', '1000', '10000', '0.99', '0.005', '0.005', '8']]
read_write_case2 = [['./linked_list_read_write_locks', '1000', '10000', '0.90', '0.05', '0.05', '1'], ['./linked_list_read_write_locks', '1000', '10000', '0.90', '0.05', '0.05', '2'], ['./linked_list_read_write_locks', '1000', '10000', '0.90', '0.05', '0.05', '4'], ['./linked_list_read_write_locks', '1000', '10000', '0.90', '0.05', '0.05', '8']]
read_write_case3 = [['./linked_list_read_write_locks', '1000', '10000', '0.50', '0.25', '0.25', '1'], ['./linked_list_read_write_locks', '1000', '10000', '0.50', '0.25', '0.25', '2'], ['./linked_list_read_write_locks', '1000', '10000', '0.50', '0.25', '0.25', '4'], ['./linked_list_read_write_locks', '1000', '10000', '0.50', '0.25', '0.25', '8']]

mutex = [mutex_case1, mutex_case2, mutex_case3]
read_write = [read_write_case1, read_write_case2, read_write_case3]

compileAll()

print('')
print('**************************************************')
print('                   Test Cases')
print('**************************************************')

for i in range(3):
    print('')
    print('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
    print('                     Case ' + str(i + 1))
    print('~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~')
    print('')
    print('Serial program :')
    print('')
    execute(serial_cases[i])
    print('')
    print('Parallel program (based on Pthreads) with one mutex :')
    print('')
    executeCommands(mutex[i])
    print('')
    print('Parallel program (based on Pthreads) with read-write locks :')
    print('')
    executeCommands(read_write[i])