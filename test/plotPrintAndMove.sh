echo $dir
rm -r ./run$1/*
root -b << EOF
.x loadTPGSimAnalysis.C
DTNtupleTPGSimAnalyzer analysis("/eos/user/b/balvarez/DTNtuples/DTDPGNtuple_run$1.root","results_run$1.root",true);                             
#Using offline shifter ntyples
#DTNtupleTPGSimAnalyzer analysis("/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2021_data/ntuples/ST/DTDPGNtuple_run$1_calib_ph1_calib_ph2.root","results_run$1.root",false); 
    
        
analysis.Loop()
EOF

./printAndMove.sh $1
