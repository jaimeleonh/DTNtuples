echo $dir
rm -r ./run$1/*
root -b << EOF
.x loadTPGSimAnalysis.C
DTNtupleTPGSimAnalyzer analysis("/eos/home-j/jleonhol/ntuplesST/DTDPGNtuple_run$1.root","results_run$1.root",true);
analysis.Loop()
EOF

./printAndMove.sh $1
