# Philosophers
The school 42 project on the use of threads and processes

# Description
The program expects 5 or 4 parameters as arguments:
- Number of philosophers
- Time in milliseconds until a philosopher dies from starvation
- Time in milliseconds it takes for a philosopher to eat
- Time in milliseconds it takes for a philosopher to sleep
- (Optional) Number of meals before the program stops

**example:**  
```
./philosopher 4 600 200 200 2
```

# Output

Every philosopher should eat and sleep, if he can't then he thinks.  
In order to eat, a philosopher needs 2 forks, but every philosopher has
only one fork, but in for eating they need 2 forks, so the philosopher has to borrow a fork from a neighbor.  
If the philosopher has not had time to eat during his lifetime, then he dies and the simulation stops
```
./philosopher 4 600 200 200 1  
0 1 is thinking
0 2 has taken a fork
0 2 has taken a fork
0 2 is eating
0 3 is thinking
0 4 has taken a fork
0 4 has taken a fork
0 4 is eating
200 4 is sleeping
200 2 is sleeping
200 3 has taken a fork
200 3 has taken a fork
200 3 is eating
200 1 has taken a fork
200 1 has taken a fork
200 1 is eating
200 Done
```
