# DTNtuples
Ntuples for the analysis of the CMS DT Detector Performance

## Preliminary instructions
**Note**: 
In the present days this code is evolving fast, hence the installation recipe may change often. Please keep an eye on this page to check for updates.

### Installation:

For simulation: 
```
cmsrel CMSSW_11_1_0_patch6
cd CMSSW_11_1_0_patch6/src/
cmsenv
git cms-merge-topic -u dtp2-tpg-am:AM_111X_dev # AM emulator
git clone https://github.com/dtp2-tpg-am/L1Trigger-DTPhase2Trigger.git L1Trigger/DTTriggerPhase2/data
git clone https://github.com/jaimeleonh/DTNtuples.git -b ntupleProduction_11_1_X DTDPGAnalysis/DTNtuples
scramv1 b -j 5
```


### Ntuple prduction:
```
cd DTDPGAnalysis/DTNtuples/test/
cmsRun dtDpgNtuples_phase2_cfg.py nEvents=10000
```
