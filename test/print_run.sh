mkdir /eos/home-j/jleonhol/www/SliceTest/run$1/
rm -r /eos/home-j/jleonhol/www/SliceTest/run$1/*
echo "INFO: Deleted old files"
#cp -r ./run$1/* /eos/home-j/jleonhol/www/SliceTest/run$1/
cp results_run$1.root /eos/home-j/jleonhol/www/SliceTest/run$1/
echo "INFO: Ntuple copied"
#rm -rf ./run$1
cp /eos/home-j/jleonhol/backup/index_withFolders_php /eos/home-j/jleonhol/www/SliceTest/run$1/index.php 
for i in ./run$1/*
do
  if [ -d "$i" ]
  then
    echo "Copying files in " $i
    mkdir /eos/home-j/jleonhol/www/SliceTest/$i
    cp $i/* /eos/home-j/jleonhol/www/SliceTest/$i
    cp /eos/home-j/jleonhol/backup/index_noFolders_php /eos/home-j/jleonhol/www/SliceTest/$i/index.php 
  fi
done

