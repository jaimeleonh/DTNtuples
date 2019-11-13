# DTNtuples
Ntuples for the analysis of the CMS drift tubes detector performance

## Preliminary instructions
**Note**: 
In the present days this code is evolving fast, hence the installation recipe may change often. Please keep an eye on this page to check for updates.

### Installation:
```
cmsrel CMSSW_10_6_1_patch2
cd CMSSW_10_6_1_patch2/src/
cmsenv
git cms-merge-topic jaimeleonh:newUnpacker # phase-2 unpacker
git cms-merge-topic -u dtp2-tpg-am:AM_106X_dev # AM emulator
git clone --single-branch --branch correctL1 https://github.com/jaimeleonh/DTNtuples.git DTDPGAnalysis/DTNtuples 
scramv1 b -j 5
```

####With last emulator version
```
cmsrel CMSSW_10_6_1_patch2
cd CMSSW_10_6_1_patch2/src/
cmsenv
git cms-merge-topic jaimeleonh:newUnpacker # phase-2 unpacker
git cms-merge-topic -u jaimeleonh:EmulFW # AM emulator
git clone --single-branch --branch correctL1 https://github.com/jaimeleonh/DTNtuples.git DTDPGAnalysis/DTNtuples 
scramv1 b -j 5
```

##### Changing to ssh (easier to do PR)
```
git remote set-url origin git@github.com:jaimeleonh/DTNtuples.git
```


### Ntuple prduction:
```
cd DTDPGAnalysis/DTNtuples/test/
cmsRun dtDpgNtuples_slicetest_prueba_cfg.py nEvents=10000 correctL1A=True runNumber=330848
```
By default, ```correctL1A=False```, so it DOES NOT correct the BX and the t0 with the L1A BX. 


### Analysis:
#### To run over a ntuple w/o L1A substraction (when choosing ```correctL1A=False```):
Fill the plots, print in png and copy to eos: 
```
./plotPrintAndMove.sh runNumber
```
W/o copying to eos
```
./plotAndPrint.sh runNumber
```
#### Or, to run over a ntuple w/ L1A substraction (when choosing ```correctL1A=True```):
Fill the plots, print in png and copy to eos: 
```
./plotPrintAndMove_cor.sh  runNumber
```
W/o copying to eos
```
./plotAndPrint_cor.sh runNumber
```

