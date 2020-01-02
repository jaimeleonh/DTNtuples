# DTNtuples
Analysis tools to get the AM simulated performance plots. 

## Preliminary instructions
**Note**: 
In the present days this code is evolving fast, hence the installation recipe may change often. Please keep an eye on this page to check for updates.

### Installation:
```
cmsrel CMSSW_10_6_5_patch1
cd CMSSW_10_6_5_patch1/src/
cmsenv
#git cms-merge-topic battibass:phase2UnpackerFromOscar_10_6_X # phase-2 primitives data format and phase-2 unpacker
git cms-merge-topic -u pozzobon:DTHough_NP_20190619_106X_noL1T # MTT-CHT emulator
git cms-merge-topic -u dtp2-tpg-am:AM_106X_dev # AM emulator
git clone https://github.com/jaimeleonh/DTNtuples.git -b unifiedPerf DTDPGAnalysis/DTNtuples
scramv1 b -j 5
```
 -j 5
```

### Analysis:

This repository groups 4 types of plotting tools: 
- Efficiency plots w.r.t segments
- Resolution plots w.r.t segments
- Digi rates
- Primitive rates

