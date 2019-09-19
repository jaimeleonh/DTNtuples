rm ./run$1/*
root -b << EOF 
.x printPlots_run.C ("$1")
EOF
./print_run.sh $1
