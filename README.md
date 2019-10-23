# DTNtuples
Ntuples for the analysis of the CMS drift tubes detector performance

## Preliminary instructions
**Note**: 
In the present days this code is evolving fast, hence the installation recipe may change often. Please keep an eye on this page to check for updates.

### Installation:
```
cmsrel CMSSW_10_6_0
cd CMSSW_10_6_0/src/
cmsenv
git cms-merge-topic jaimeleonh:newUnpacker # phase-2 unpacker
git cms-merge-topic -u dtp2-tpg-am:AM_106X_dev # AM emulator
git clone --single-branch --branch correctL1 https://github.com/jaimeleonh/DTNtuples.git DTDPGAnalysis/DTNtuples
scramv1 b -j 5
```

### Ntuple prduction:
```
cd DTDPGAnalysis/DTNtuples/test/
cmsRun dtDpgNtuples_slicetest_prueba_cfg.py nEvents=10000 correctL1A=True runNumber=330848
```

### Analysis:
```
./plotPrintAndMove.sh runNumber
```
to run over a ntuple w/o L1A substraction or 
```
./plotPrintAndMove_cor.sh  runNumber
```
to run over a ntuple w/ L1A substraction
