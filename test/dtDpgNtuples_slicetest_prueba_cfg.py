import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
from Configuration.StandardSequences.Eras import eras

import subprocess
import sys
import os

options = VarParsing.VarParsing()

options.register('globalTag',
                 '106X_dataRun3_Express_v2', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Global Tag")

options.register('correctL1A',
                 False, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "Correct L1A")

options.register('nEvents',
                 -1, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.int,
                 "Maximum number of processed events")

options.register('runNumber',
                 '330998', #default value
                 #'330848', #default value
                 #'330792', #default value
                 #'330675', #default value
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.int,
                 "Run number to be looked for in either 'inputFolderCentral' or 'inputFolderDT' folders")

options.register('inputFile',
                 '', 
		#'/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/root/run330228_streamA_fu-c2f13-09-03.root', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "The input file to be processed, if non null overrides runNumber based input file selection")

options.register('inputFolderCentral',
		 #'',
                 '/eos/cms/store/data/Commissioning2019/MiniDaq/RAW/v1/', #default value
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                 "Base EOS folder with input files from MiniDAQ runs with central tier0 transfer")

options.register('inputFolderDT',
                 '/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/root/', #default value
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                 "Base EOS folder with input files from MiniDAQ runs with DT 'private' tier0 transfer")

options.register('tTrigFile',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "File with customised DT tTrigs, used only if non ''")

options.register('t0File',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "File with customised DT t0is, used only if non ''")

options.register('vDriftFile',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "File with customised DT vDrifts, used only if non ''")

options.register('ntupleName',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Folder and name ame for output ntuple, if non null overrides 'standard' naming based on runNumber option")


options.parseArguments()

process = cms.Process("DTNTUPLES",eras.Run2_2018)

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.nEvents))

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.GlobalTag.globaltag = cms.string(options.globalTag)

if options.tTrigFile  != '' or \
   options.vDriftFile != '' or \
   options.t0File != '' :
    process.GlobalTag.toGet = cms.VPSet()

if options.tTrigFile != '' :
    process.GlobalTag.toGet.append(cms.PSet(record = cms.string("DTTtrigRcd"),
                                            tag = cms.string("ttrig"),
                                            connect = cms.string("sqlite_file:" + options.tTrigFile),
                                            label = cms.untracked.string("cosmics")
                                        )
                               )

if options.vDriftFile != '' :
    process.GlobalTag.toGet.append(cms.PSet(record = cms.string("DTMtimeRcd"),
                                            tag = cms.string("vDrift"),
                                            connect = cms.string("sqlite_file:" + options.vDriftFile)
                                        )
                                   )

if options.t0File != '' :
    process.GlobalTag.toGet.append(cms.PSet(record = cms.string("DTT0Rcd"),
                                            tag = cms.string("t0"),
                                            connect = cms.string("sqlite_file:" + options.t0File)
                                        )
                                   )
    

process.source = cms.Source("PoolSource",
                            
        fileNames = cms.untracked.vstring(),
        secondaryFileNames = cms.untracked.vstring()

)

if options.inputFile != '' :

    print "[dtDpgNtuples_slicetest_cfg.py]: inputFile parameter is non-null running on file:\n\t\t\t" + options.inputFile
    #process.source.fileNames = cms.untracked.vstring("file://" + options.inputFile)
    #process.source.fileNames = cms.untracked.vstring('file:/eos/cms/store/data/Commissioning2019/MiniDaq/RAW/v1/000/330/675/00000/5F07BFDB-0A8C-3B44-8CB8-E569A44B6374.root')
    process.source.fileNames = cms.untracked.vstring('file:/eos/cms/store/data/Commissioning2019/MiniDaq/RAW/v1/000/330/792/00000/BBF3AF70-11CD-6C4F-ACD0-4B559D57C90A.root')

else :

    runStr = str(options.runNumber).zfill(9)
    runFolder = options.inputFolderCentral + "/" + runStr[0:3] + "/" + runStr[3:6] + "/" + runStr[6:] + "/00000"

    print "[dtDpgNtuples_slicetest_cfg.py]: looking for files under:\n\t\t\t" + runFolder
    
    if os.path.exists(runFolder) :
        files = subprocess.check_output(["ls", runFolder])
        process.source.fileNames = ["file://" + runFolder + "/" + f for f in files.split()]

    else :
        print "[dtDpgNtuples_slicetest_cfg.py]: files not found there, looking under:\n\t\t\t" + options.inputFolderDT

        files = subprocess.check_output(["ls", options.inputFolderDT])
        filesFromRun = []

        for f in files.split() :
            if f.find(runStr[3:]) > -1 :
                filesFromRun.append(f)

        if len(filesFromRun) == 1 :
            process.source.fileNames.append("file://" + options.inputFolderDT + "/" + filesFromRun[0])

        else :
            print "[dtDpgNtuples_slicetest_cfg.py]: " + str(len(filesFromRun)) + " files found, can't run!"
            sys.exit(999)

print process.source.fileNames

if options.ntupleName == '' :
    if options.correctL1A == False :
      ntupleName = "/afs/cern.ch/work/j/jleonhol/public/DTDPGNtuple_run" + str(options.runNumber) + ".root"
    else :
      ntupleName = "/afs/cern.ch/work/j/jleonhol/public/DTDPGNtuple_run" + str(options.runNumber) + "_corrected.root"
    #ntupleName = "./DTDPGNtuple_run" + str(options.runNumber) + ".root"
    #ntupleName = "./DTDPGNtuple.root"
else :
    ntupleName = options.ntupleName

process.TFileService = cms.Service('TFileService',
        fileName = cms.string(ntupleName)
    )

process.load('Configuration/StandardSequences/GeometryRecoDB_cff')
process.load("Configuration.StandardSequences.MagneticField_cff")

process.load('Configuration.StandardSequences.RawToDigi_Data_cff')
process.load('EventFilter.DTRawToDigi.dtab7unpacker_cfi')

process.dtAB7unpacker.channelMapping = cms.untracked.string("july2019")
process.dtAB7unpacker.correctTPTimeToL1A = options.correctL1A;

process.load('RecoLocalMuon.Configuration.RecoLocalMuonCosmics_cff')

process.load('DTDPGAnalysis.DTNtuples.dtNtupleProducer_slicetest_cfi')


process.load("Phase2L1Trigger.CalibratedDigis.CalibratedDigis_cfi")
process.CalibratedDigis.dtDigiTag = cms.InputTag('dtAB7unpacker')
#process.CalibratedDigis.timeOffset = 350 # 325+25 is no offset with the SX5/Cosmics runs (BX is -1)
process.CalibratedDigis.scenario = 2
process.load("L1Trigger.DTPhase2Trigger.dtTriggerPhase2PrimitiveDigis_cfi")
process.dtTriggerPhase2PrimitiveDigis.scenario = 2
process.dtTriggerPhase2PrimitiveDigis.dTanPsi_correlate_TP = cms.untracked.double(620./4096.)
process.dtTriggerPhase2PrimitiveDigis.chi2corTh = cms.untracked.double(100000) #in cm^2
process.dtTriggerPhase2PrimitiveDigis.useBX_correlation = cms.untracked.bool(True)
process.dtTriggerPhase2PrimitiveDigis.dT0_correlate_TP = cms.untracked.double(25.) 
process.dtTriggerPhase2PrimitiveDigis.dBX_correlate_TP = cms.untracked.int32(0) 
#process.dtTriggerPhase2PrimitiveDigis.debug = True
#process.dtTriggerPhase2PrimitiveDigis.dump = True



process.p = cms.Path(process.muonDTDigis
                     + process.dtAB7unpacker
                     + process.twinMuxStage2Digis
                     + process.bmtfDigis
                     + process.dtlocalrecoT0Seg
		     + process.CalibratedDigis
		     + process.dtTriggerPhase2PrimitiveDigis
                     + process.dtNtupleProducer)
