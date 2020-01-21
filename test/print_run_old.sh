mkdir /eos/home-j/jleonhol/www/SliceTest/run$1/
mkdir /eos/home-j/jleonhol/www/SliceTest/run$1/plots
rm /eos/home-j/jleonhol/www/SliceTest/run$1/plots/*
cp ./run$1/* /eos/home-j/jleonhol/www/SliceTest/run$1/plots/
cp results_run$1.root /eos/home-j/jleonhol/www/SliceTest/run$1/
rm -rf ./run$1
cp /eos/home-j/jleonhol/backup/index_withFolders_php /eos/home-j/jleonhol/www/SliceTest/run$1/index.php 
cp /eos/home-j/jleonhol/backup/index_noFolders_php /eos/home-j/jleonhol/www/SliceTest/run$1/*/index.php 
