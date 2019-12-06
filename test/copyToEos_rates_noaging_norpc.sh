rm /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_norpc/*
root -b << EOF
.x plotAll_noaging_norpc.C
EOF
python myPlotter_rings.py noaging norpc
python myPlotter_rings_ratios.py
cp /eos/home-j/jleonhol/www/rateStudies/index_php /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_norpc/index.php
cp ./ratePlots/h* /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_norpc/ 
python myPlotter_alot.py
cp ./plotsEff_rates/hEff.* /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_norpc/ 
cp results_nu_pu250_noage_norpc.root /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_norpc/ 
cp outPlots_ratios.root /eos/home-j/jleonhol/www/rateStudies/nu_pu250_noage_norpc/ 
