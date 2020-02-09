rm -r ./run$1/*
root -b << EOF
.x loadTPGSimAnalysis.C
DTNtupleTPGSimAnalyzer analysis("/eos/home-j/jleonhol/ntuplesST/DTDPGNtuple_run$1_corrected.root","results_run$1.root",false)
analysis.Loop()
EOF

root -b << EOF 
.x printPlots_run.C ("$1")
EOF
./print_run.sh $1
