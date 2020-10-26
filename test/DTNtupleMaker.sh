#TPrun=$1
#CSrun=$2
#GlobalRun=$3

#source configForSliceTestAnalysis.sh
# if running on 2019 runs, modify next line
export DTSAMPLEDIR=/eos/cms/store/data/Commissioning2020/MiniDaq/RAW/v1
cd ./calib/

let firstThreeTP=$1/1000
let lastThreeTP=$1%1000

if (( $lastThreeTP < 100 )) 
then
repackedDirTP=/eos/cms/store/data/Commissioning2020/MiniDaq/RAW/v1/000/$firstThreeTP/0$lastThreeTP
else
repackedDirTP=/eos/cms/store/data/Commissioning2020/MiniDaq/RAW/v1/000/$firstThreeTP/$lastThreeTP
fi

calibFile=t0i_run$1.txt


if [ -f "$calibFile" ] 
then
echo "Found t0i file. Remove file in order to rerun the T0 calibration"
else
  if [ -d "$repackedDirTP" ]
  then
    cmsRun dtT0WireCalibration_cfg.py runNumber=$1 > t0i_run$1.txt
  else 
    cmsRun dtT0WireCalibration_cfg.py runNumber=$1 runOnDat=True  inputFolderCentral=/eos/cms/store/t0streamer/Minidaq/A/  > t0i_run$1.txt
  fi
fi

ttrigFile=./Run$2-ttrig_timebox_v1/TimeBoxes/results/ttrig_timeboxes_Run$2_v1.db

if [ -f "$ttrigFile" ]
then 
echo "Found ttrig file. Remove file in order to rerun the ttrig calibration"
else
dtCalibration ttrig timeboxes all --run=$2 --trial=1 --label=ttrig_timebox --runselection=$2 --datasetpath=/Dummy/Dummy/RAW --globaltag=106X_dataRun3_Express_v2  --datasettype=Cosmics --run-on-RAW --phase2 --inputT0DB ./t0_run$1.db --input-files-local
fi

cd ..

ntupleFile=./DTDPGNtuple_run$2.root

let firstThree=$2/1000
let lastThree=$2%1000

if (( $lastThree < 100 )) 
then
repackedDir=/eos/cms/store/data/Commissioning2020/MiniDaq/RAW/v1/000/$firstThree/0$lastThree
else
repackedDir=/eos/cms/store/data/Commissioning2020/MiniDaq/RAW/v1/000/$firstThree/$lastThree
fi

echo $repackedDir

ph1_calib_folder=/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2020_data/calib/phase1

if [ -f "$ntupleFile" ]
then 
echo "Found ntuple. Remove file in order to rerun the ntuple production"
else
  if [ -d "$repackedDir" ]
  then
  echo "Running the ntuple production with the rootfile..."
  cmsRun dtDpgNtuples_slicetest_prueba_cfg.py nEvents=1000000 runNumber=$2 tTrigFilePh2=./calib/Run$2-ttrig_timebox_v1/TimeBoxes/results/ttrig_timeboxes_Run$2_v1.db t0FilePh2=./calib/t0_run$1.db tTrigFile=$ph1_calib_folder/ttrig_timeboxes_Run335018_MB3_techtrig_phase1_v1.db t0File=$ph1_calib_folder/t0_run335017_MB3_techtrig_phase1.db
  else 
  echo "Running the ntuple production with the datfiles..."
  cmsRun dtDpgNtuples_slicetest_prueba_cfg.py runOnDat=True  inputFolderCentral=/eos/cms/store/t0streamer/Minidaq/A/ nEvents=1000000 runNumber=$2 tTrigFilePh2=./calib/Run$2-ttrig_timebox_v1/TimeBoxes/results/ttrig_timeboxes_Run$2_v1.db t0FilePh2=./calib/t0_run$1.db tTrigFile=$ph1_calib_folder/ttrig_timeboxes_run337786_t0_run337772_v1.db t0File=$ph1_calib_folder/t0_run337772.db
  fi
fi

