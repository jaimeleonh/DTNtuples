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
git cms-merge-topic oglez:Phase2_CMSSW_10_6_0_pre4_Summer2019 # phase-2 unpacker
git cms-merge-topic -u pozzobon:DTHough_NP_20190619_106X_noL1T # MTT-CHT emulator
git cms-merge-topic -u jaimeleonh:AM_106X_SliceTest # AM emulator
git clone --single-branch --branch newBranch https://github.com/jaimeleonh/DTNtuples.git DTDPGAnalysis/DTNtuples
scramv1 b -j 5
```

### Ntuple prduction:
```
cd DTDPGAnalysis/DTNtuples/test/
cmsRun dtDpgNtuples_slicetest_prueba_cfg.py nEvents=10000
```

### Analysis:
```
./plotPrintAndMove.sh runNumber
