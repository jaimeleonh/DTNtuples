# rm /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_rpc
root -b << EOF
.x plotAll_noaging_rpc.C
EOF
python myPlotter_rings.py noaging rpc
python myPlotter_rings_ratios.py
mkdir /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_withrpc
cp /eos/home-j/jleonhol/www/rateStudies/index_php /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_withrpc/index.php
cp ./ratePlots/h* /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_withrpc/ 
python myPlotter_alot_withRPC.py
cp ./plotsEff_rates/hEff.* /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_withrpc/ 
cp results_nu_pu250_noage_withrpc.root /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_withrpc/ 
cp outPlots_ratios.root /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_withrpc/ 
