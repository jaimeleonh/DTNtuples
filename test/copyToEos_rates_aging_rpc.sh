# rm /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_norpc
root -b << EOF
.x plotAll_aging_rpc.C
EOF
python myPlotter_rings.py aging rpc
python myPlotter_rings_ratios.py
mkdir /eos/home-j/jleonhol/www/rateStudies/nu_pu250_age_withrpc_youngseg_muonage_norpcage_fail_3000/
cp /eos/home-j/jleonhol/www/rateStudies/index_php /eos/home-j/jleonhol/www/rateStudies/nu_pu250_age_withrpc_youngseg_muonage_norpcage_fail_3000/index.php
cp ./ratePlots/h* /eos/home-j/jleonhol/www/rateStudies/nu_pu250_age_withrpc_youngseg_muonage_norpcage_fail_3000/ 
python myPlotter_alot_withRPC_aged.py
cp ./plotsEff_rates/hEff_* /eos/home-j/jleonhol/www/rateStudies/nu_pu250_age_withrpc_youngseg_muonage_norpcage_fail_3000/ 
cp results_nu_pu250_noage_withrpc.root /eos/home-j/jleonhol/www/rateStudies/nu_pu250_age_withrpc_youngseg_muonage_norpcage_fail_3000/
cp outPlots_ratios.root /eos/home-j/jleonhol/www/rateStudies/nu_pu250_age_withrpc_youngseg_muonage_norpcage_fail_3000/
