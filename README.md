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

    id = genRandomId()
    is_active = True

    q = r = 0

    left = rank-1
    right = (rank+1)%size

    if rank == 0:
        left = size-1

    while running:
        if is_active:
            send id to right
            receive q from left

            send id to left
            receive r from right

            max_v = max(q,r)
            if max_v > id:
                is_active = False
            else if max_v == id:
                break
        else:
            receive q from left
            send q to right

            receive r from right
            send r to left

    # leader breaks out of loop and kills rest of processes
```
