# 474-Project2

Project 2: Implement a distributed algorithm using MPI

Ring Election with Franklin’s Algorithm

Group members:

Luke Eltiste lukeeltiste@csu.fullerton.edu

## How to run

Either

```shell
mpic++ ring_election.cpp -o ring_election

mpirun -n number_nodes ./ring_election
```

Or

```shell
 ./compile_run.sh n
```

## Pseudocode

```python
# Franklin’s Algorithm

def franklins_algorithm():
    rank = get_rank()
    size = get_size()
    is_active = True
    is_leader = False
    running = True

    q = [0] # id, round
    r = [0] # id, round

    left = rank-1
    right = (rank+1)%size

    if rank == 0:
        left = size-1

    while running:
        if is_active:
            send [rank] to left
            send [rank] to right

            receive q from left
            receive r from right

            max_v = max(q[0],r[0])
            if max_v > rank:
                is_active = False
            else if max_v == rank:
                is_leader = True
                running = False
        else:
            receive q from left
            receive r from right

            if q[0] == -1 or r[0] == -1:
                # leader found
                running = False
                print(f"{rank} is now leader")

            send q to right
            send r to left

    if is_leader:
        # send out message to inform leader has been found
        send [-1, 0] to left
        send [-1, 0] to right
```
