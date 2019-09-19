rm ./run$1/*
root -b << EOF
.x loadTPGSimAnalysis.C
DTNtupleTPGSimAnalyzer analysis("/afs/cern.ch/work/j/jleonhol/public/DTDPGNtuple_run$1.root","results_run$1.root")
analysis.Loop()
EOF

root -b << EOF 
.x printPlots_run.C ("$1")
EOF
./print_run.sh $1
