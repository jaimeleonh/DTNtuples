mkdir /eos/home-j/jleonhol/www/resolutionsAMall
mkdir /eos/home-j/jleonhol/www/resolutionsHBall
mkdir /eos/home-j/jleonhol/www/resolutionsAMAllST
rm /eos/home-j/jleonhol/www/resolutionsAMall/*
rm /eos/home-j/jleonhol/www/resolutionsHBall/*
rm /eos/home-j/jleonhol/www/resolutionsAMST/*
cp /eos/home-j/jleonhol/backup/index_resol_php /eos/home-j/jleonhol/www/resolutionsAMall/index.php
cp /eos/home-j/jleonhol/backup/index_resol_php /eos/home-j/jleonhol/www/resolutionsAMST/index.php
cp /eos/home-j/jleonhol/backup/index_resol_php /eos/home-j/jleonhol/www/resolutionsHBall/index.php
cp ./plotsPU0ST/*AM*.png /eos/home-j/jleonhol/www/resolutionsAMST 
cp ./plots/*AM*.png /eos/home-j/jleonhol/www/resolutionsAMall 
cp ./plots/*HB*.png /eos/home-j/jleonhol/www/resolutionsHBall
