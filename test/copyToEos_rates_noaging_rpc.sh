# rm /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_norpc
root -b << EOF
.x plotAll_noaging_rpc.C
EOF
python myPlotter_rings.py noaging norpc
python myPlotter_rings_ratios.py
cp /eos/home-j/jleonhol/www/rateStudies/index_php /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_withrpc/index.php
cp ./ratePlots/h* /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_withrpc/ 
cp hEff.* /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_withrpc/ 
cp results_nu_pu250_noage_withrpc.root /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_withrpc/ 
cp outPlots_ratios.root /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_withrpc/ 
