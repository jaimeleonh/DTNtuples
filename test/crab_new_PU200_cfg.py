from WMCore.Configuration import Configuration
config = Configuration()
from samples import samples


################### MODIFY THESE TWO PARAMETERS ###############
bkgUsed = "0" ## This one just sets filenames, need to modify it in plugins/DTRandomDigiGenerator.cc and recompile
sampleUsed = "nu_PU200" ## This alias can be found in test/samples.py





##### Configuration parameters ################################
inputDataset = samples[sampleUsed]

# These are the cfg parameters used to configure the 
# dtDpgNtuples_slicetest_cfg.py configuration file
configParams = ['ntupleName=DTDPGNtuple.root', 'applyRandomBkg=True' ] 
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
config.General.requestName = 'DTDPGNtuples_' + sampleUsed + '_bkg' + bkgUsed
config.General.transferOutputs = True

config.section_('JobType')
config.JobType.pluginName  = 'Analysis'
config.JobType.psetName    = 'dtDpgNtuples_phase2_cfg.py'
config.JobType.allowUndistributedCMSSW = True

config.JobType.pyCfgParams = configParams
config.JobType.inputFiles  = inputFiles

config.section_('Data')
config.Data.inputDataset = inputDataset

config.Data.splitting    = 'LumiBased'
config.Data.unitsPerJob  = 10  
#config.Data.inputDBS = 'phys03'
#config.Data.inputDBS     = 'https://cmsweb.cern.ch/dbs/prod/global/DBSReader/'
config.Data.outLFNDirBase  = '/store/user/jleonhol/' + sampleUsed +'_bkg' + bkgUsed

config.section_('Site')
config.Site.storageSite = 'T2_ES_CIEMAT'
#config.Site.whitelist = ['T2_PL_Warsaw']

