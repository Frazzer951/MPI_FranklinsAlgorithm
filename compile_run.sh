n=$1
if [ -z ${n} ]; then
    echo "set the number of nodes to use './compile_run.sh n'"
    exit
fi

mpic++ ring_election.cpp -o ring_election

mpirun -n ${n} ./ring_election
