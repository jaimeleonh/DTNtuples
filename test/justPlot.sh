root -b << EOF
.x loadTPGSimAnalysis.C
DTNtupleTPGSimAnalyzer analysis("/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2021_data/ntuples/ST/DTDPGNtuple_run$1.root", "results_run$1.root", false);
analysis.Loop()
EOF
