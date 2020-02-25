from WMCore.Configuration import Configuration
config = Configuration()

##### Configuration parameters ################################

#inputDataset = "/DisplacedMuons_Pt10to30_Dxy0to3000-pythia8-gun/PhaseIITDRSpring19DR-PU200_106X_upgrade2023_realistic_v3-v1/GEN-SIM-DIGI-RAW"
#inputDataset = "/Mu_FlatPt2to100-pythia8-gun/PhaseIITDRSpring19DR-NoPU_106X_upgrade2023_realistic_v3-v1/GEN-SIM-DIGI-RAW"
#inputDataset = "/Mu_FlatPt2to100-pythia8-gun/PhaseIITDRSpring19DR-PU200_106X_upgrade2023_realistic_v3-v2/GEN-SIM-DIGI-RAW"
inputDataset = "/DisplacedMuons_Pt10to30_Dxy0to3000-pythia8-gun/PhaseIITDRSpring19DR-NoPU_106X_upgrade2023_realistic_v3-v1/GEN-SIM-DIGI-RAW"
#inputDataset = "/HTo2LongLivedTo4mu_MH-200_MFF-50_CTau-2000mm_TuneCP5_13TeV_pythia8/escalant-MC2018_Benchmark_2mu2jets_June2019-GSR-testForL1-v2-5aa1749307f00d6302ec929df355f761/USER"
#inputDataset = "/DisplacedMuons_Pt30to100_Dxy0to3000-pythia8-gun/PhaseIITDRSpring19DR-NoPU_106X_upgrade2023_realistic_v3-v1/GEN-SIM-DIGI-RAW"

# These are the cfg parameters used to configure the 
# dtDpgNtuples_slicetest_cfg.py configuration file
configParams = ['ntupleName=DTDPGNtuple.root'] 
# E.g. use dedicated tTrigs
# configParams = ['ntupleName=DTDPGNtuple.root', \
#                 'tTrigFile=calib/TTrigDB_cosmics_ttrig.db'] 

# These are the additional input files (e.g. sqlite .db) 
# needed by dtDpgNtuples_slicetest_cfg.py to run
inputFiles = [] 
# E.g. use dedicated tTrigs
# inputFiles = ['./calib/TTrigDB_cosmics_ttrig.db'] 

###############################################################

config.section_('General')
config.General.workArea = 'crab_jobs'
#config.General.requestName = 'DTDPGNtuples_PU200_1.5'
#config.General.requestName = 'DTDPGNtuples_PU200_newCut'
config.General.requestName = 'DTDPGNtuples_DM_PU0_corSeg_new'
#config.General.requestName = 'DTDPGNtuples_PU0_DM_PT10-30_mod'
config.General.transferOutputs = True

config.section_('JobType')
config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = 'dtDpgNtuples_phase2_cfg.py'

config.JobType.pyCfgParams = configParams
config.JobType.inputFiles  = inputFiles

config.section_('Data')
config.Data.inputDataset = inputDataset

config.Data.splitting    = 'LumiBased'
config.Data.unitsPerJob  = 10  
#config.Data.inputDBS = 'phys03'
#config.Data.inputDBS     = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/'
#config.Data.outLFNDirBase  = '/store/user/jleonhol/PU200_newCut'
config.Data.outLFNDirBase  = '/store/user/jleonhol/PU0_DM_PT10-30_corSeg_new'

config.section_('Site')
config.Site.storageSite = 'T2_ES_CIEMAT'
#config.Site.whitelist = ['T2_PL_Warsaw']

