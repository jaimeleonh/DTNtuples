import FWCore.ParameterSet.Config as cms
import FWCore.ParameterSet.VarParsing as VarParsing
from Configuration.StandardSequences.Eras import eras

def appendToGlobalTag(process, rcd, tag, fileName, label) :

    if  not fileName :
        return process

    if not hasattr(process.GlobalTag,"toGet") :
        process.GlobalTag.toGet = cms.VPSet()

    process.GlobalTag.toGet.append(
        cms.PSet(tag = cms.string(tag),
                 record = cms.string(rcd),
                 connect = cms.string("sqlite_file:" + fileName),
                 label = cms.untracked.string(label)
             )
    )

    return process

import subprocess
import sys
import os

options = VarParsing.VarParsing()

options.register('globalTag',
                 '111X_dataRun3_Prompt_v2', #default value
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
                 '/eos/cms/store/data/Commissioning2020/MiniDaq/RAW/v1/', #default value
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                 "Base EOS folder with input files from MiniDAQ runs with central tier0 transfer")

options.register('inputFolderDT',
                 '/eos/cms/store/t0streamer/Minidaq/A/', #default value
                 #'/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/root/', #default value
                  VarParsing.VarParsing.multiplicity.singleton,
                  VarParsing.VarParsing.varType.string,
                 "Base EOS folder with input files from MiniDAQ runs with DT 'private' tier0 transfer")

options.register('inputGlobal',
                 '/eos/cms/store/data/Commissioning2020/Cosmics/RAW/v1/', #default value
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

options.register('tTrigFilePh2',
                 '', #default value
                 #'/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/calib/ttrig_phase2_Run333369.db', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "File with customised DT phase-2 tTrigs, used only if non ''")

options.register('t0FilePh2',
                 '', #default value
                 #'/eos/cms/store/group/dpg_dt/comm_dt/commissioning_2019_data/calib/t0_phase2_Run333364.db', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "File with customised DT phase-2 t0is, used only if non ''")


options.register('vDriftFile',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "File with customised DT vDrifts, used only if non ''")

options.register('runOnDat',
                 False, #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.bool,
                 "If set to True switches source from 'PoolSource' to 'NewEventStreamFileReader'")
options.register('ntupleName',
                 '', #default value
                 VarParsing.VarParsing.multiplicity.singleton,
                 VarParsing.VarParsing.varType.string,
                 "Folder and name ame for output ntuple, if non null overrides 'standard' naming based on runNumber option")


options.parseArguments()

if options.runOnDat :
    inputSourceType = "NewEventStreamFileReader"
else:
    inputSourceType = "PoolSource"
     
process = cms.Process("DTNTUPLES",eras.Run2_2018)

process.load('Configuration.StandardSequences.Services_cff')
process.load('FWCore.MessageService.MessageLogger_cfi')

process.options   = cms.untracked.PSet( wantSummary = cms.untracked.bool(True) )
process.MessageLogger.cerr.FwkReport.reportEvery = 100
process.maxEvents = cms.untracked.PSet(input = cms.untracked.int32(options.nEvents))

process.load('Configuration.StandardSequences.FrontierConditions_GlobalTag_condDBv2_cff')

process.GlobalTag.globaltag = cms.string(options.globalTag)

process = appendToGlobalTag(process, "DTTtrigRcd", "ttrig", options.tTrigFile, "cosmics")
process = appendToGlobalTag(process, "DTT0Rcd", "t0", options.t0File, "")

process = appendToGlobalTag(process, "DTTtrigRcd", "ttrig", options.tTrigFilePh2, "cosmics_ph2")
process = appendToGlobalTag(process, "DTT0Rcd", "t0", options.t0FilePh2, "ph2")

process = appendToGlobalTag(process, "DTMtimeRcd", "vDrift", options.vDriftFile, "")

if options.tTrigFile    != '' or \
   options.t0File       != '' or \
   options.tTrigFilePh2 != '' or \
   options.t0FilePh2    != '' or \
   options.vDriftFile   != '' :
    process.GlobalTag.toGet = cms.VPSet()

if options.tTrigFile != '' :
    process.GlobalTag.toGet.append(cms.PSet(record = cms.string("DTTtrigRcd"),
                                            tag = cms.string("ttrig"),
                                            connect = cms.string("sqlite_file:" + options.tTrigFile),
                                            label = cms.untracked.string("cosmics")
                                        )
                               )
                               
if options.t0File != '' :
    process.GlobalTag.toGet.append(cms.PSet(record = cms.string("DTT0Rcd"),
                                            tag = cms.string("t0"),
                                            connect = cms.string("sqlite_file:" + options.t0File)
                                        )
                                   )

if options.tTrigFilePh2 != '' :
    process.GlobalTag.toGet.append(cms.PSet(record = cms.string("DTTtrigRcd"),
                                            tag = cms.string("ttrig"),
                                            connect = cms.string("sqlite_file:" + options.tTrigFilePh2),
                                            label = cms.untracked.string("cosmics_ph2")
                                        )
                               )

if options.t0FilePh2 != '' :
    process.GlobalTag.toGet.append(cms.PSet(record = cms.string("DTT0Rcd"),
                                            tag = cms.string("t0"),
                                            connect = cms.string("sqlite_file:" + options.t0FilePh2),
                                            label = cms.untracked.string("ph2")
                                        )
                                   )


if options.vDriftFile != '' :
    process.GlobalTag.toGet.append(cms.PSet(record = cms.string("DTMtimeRcd"),
                                            tag = cms.string("vDrift"),
                                            connect = cms.string("sqlite_file:" + options.vDriftFile)
                                        )
                                   )


    

process.source = cms.Source(inputSourceType,
#process.source = cms.Source("PoolSource",
                            
        fileNames = cms.untracked.vstring(),
#        secondaryFileNames = cms.untracked.vstring(),
#	skipEvents=cms.untracked.uint32(0)

)

if options.inputFile != '' :

    print "[dtDpgNtuples_slicetest_cfg.py]: inputFile parameter is non-null running on file:\n\t\t\t" + options.inputFile
    #process.source.fileNames = cms.untracked.vstring("file://" + options.inputFile)
    #process.source.fileNames = cms.untracked.vstring('file:/eos/cms/store/data/Commissioning2019/MiniDaq/RAW/v1/000/330/675/00000/5F07BFDB-0A8C-3B44-8CB8-E569A44B6374.root')
    process.source.fileNames = cms.untracked.vstring('file:/eos/cms/store/data/Commissioning2019/MiniDaq/RAW/v1/000/330/792/00000/BBF3AF70-11CD-6C4F-ACD0-4B559D57C90A.root')

else :

    runStr = str(options.runNumber).zfill(9)
    runFolder = options.inputFolderCentral + "/" + runStr[0:3] + "/" + runStr[3:6] + "/" + runStr[6:] 
    #runFolder = options.inputFolderCentral + "/" + runStr[0:3] + "/" + runStr[3:6] + "/" + runStr[6:] + "/00000"
    if not options.runOnDat:
        runFolder = runFolder + "/00000"                  
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
            runFolderGlobal = options.inputGlobal + "/" + runStr[0:3] + "/" + runStr[3:6] + "/" + runStr[6:] + "/00000"
	    
	    print "[dtDpgNtuples_slicetest_cfg.py]: files not found there, looking under:\n\t\t\t" + runFolderGlobal
            if os.path.exists(runFolderGlobal) :
               files = subprocess.check_output(["ls", runFolderGlobal])
	       process.source.fileNames = ["file://" + runFolderGlobal + "/" + f for f in files.split()]


	    else :
               print "[dtDpgNtuples_slicetest_cfg.py]: " + str(len(filesFromRun)) + " files found, can't run!"
               sys.exit(999)

print process.source.fileNames

if options.ntupleName == '' :
    if options.correctL1A == False :
      ntupleName = "/eos/home-j/jleonhol/ntuplesST/DTDPGNtuple_run" + str(options.runNumber) + ".root"
    else :
      ntupleName = "/eos/home-j/jleonhol/ntuplesST/DTDPGNtuple_run" + str(options.runNumber) + "_corrected.root"
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
process.dtAB7unpacker.correctTPTimeToL1A = cms.untracked.bool(True)

process.dtAB7unpackerNoCor = process.dtAB7unpacker.clone()
process.dtAB7unpackerNoCor.correctTPTimeToL1A = options.correctL1A;
process.dtAB7unpackerNoCor.print_prims = True
prims_debug = "/eos/home-j/jleonhol/ntuplesST/run" + str(options.runNumber) + "_fw.txt"
if os.path.isfile(prims_debug):
    os.remove(prims_debug)
process.dtAB7unpackerNoCor.file_to_print = cms.untracked.string(prims_debug)

process.load('RecoLocalMuon.Configuration.RecoLocalMuonCosmics_cff')

process.load('DTDPGAnalysis.DTNtuples.dtNtupleProducer_slicetest_cfi')


process.load("L1Trigger.DTTriggerPhase2.CalibratedDigis_cfi")
#process.CalibratedDigis.dtDigiTag = cms.InputTag('dtAB7unpacker')
process.CalibratedDigis.dtDigiTag = cms.InputTag('dtAB7unpackerNoCor')

process.load("L1Trigger.DTTriggerPhase2.dtTriggerPhase2PrimitiveDigis_cfi")
process.dtTriggerPhase2PrimitiveDigis.debug = cms.untracked.bool(False)
process.dtTriggerPhase2PrimitiveDigis.dump = cms.untracked.bool(False)
process.dtTriggerPhase2PrimitiveDigis.printPython = cms.untracked.bool(True)
process.dtTriggerPhase2PrimitiveDigis.print_prims = True
prims_debug = "/eos/home-j/jleonhol/ntuplesST/run" + str(options.runNumber) + "_emu.txt"
if os.path.isfile(prims_debug):
    os.remove(prims_debug)
process.dtTriggerPhase2PrimitiveDigis.file_to_print = cms.untracked.string(prims_debug)

####################### SliceTest specials ##############################

#Scenario -> 2 (SliceTest only)
process.CalibratedDigis.scenario = 2
process.dtTriggerPhase2PrimitiveDigis.scenario = 2

#SL TanPhi cut
process.dtTriggerPhase2PrimitiveDigis.tanPhiTh = cms.untracked.double(1.5)
#process.dtTriggerPhase2PrimitiveDigis.tanPhiTh = cms.untracked.double(1.)

#Chi2 -> Changing a lot lately
process.dtTriggerPhase2PrimitiveDigis.chi2Th = cms.untracked.double(0.01)
process.dtTriggerPhase2PrimitiveDigis.chi2corTh = cms.untracked.double(0.1) #in cm^2

#LSB -> Position 0.025 cm instead of 0.004 cm
process.dtTriggerPhase2PrimitiveDigis.use_LSB = True
process.dtTriggerPhase2PrimitiveDigis.x_precision = cms.untracked.double(1./160.)
process.dtTriggerPhase2PrimitiveDigis.tanPsi_precision = cms.untracked.double(1./4096.)

#Correlate with BX
process.dtTriggerPhase2PrimitiveDigis.useBX_correlation = True
process.dtTriggerPhase2PrimitiveDigis.dBX_correlate_TP = 1

#Correlate with tanPsi
process.dtTriggerPhase2PrimitiveDigis.dTanPsi_correlate_TP = cms.untracked.double(620./4096.)
#process.dtTriggerPhase2PrimitiveDigis.dTanPsi_correlate_TP = cms.untracked.double(99999./4096.)

#Confirmation forbidden
process.dtTriggerPhase2PrimitiveDigis.allow_confirmation = False


process.p = cms.Path(process.muonDTDigis
                     + process.dtAB7unpacker
                     + process.dtAB7unpackerNoCor
                     + process.twinMuxStage2Digis
                     + process.bmtfDigis
                     + process.dtlocalrecoT0Seg
		                 + process.CalibratedDigis
		                 + process.dtTriggerPhase2PrimitiveDigis
                     + process.dtNtupleProducer)

if options.tTrigFilePh2 != '' and options.t0FilePh2 != '':
    from DTDPGAnalysis.DTNtuples.customiseDtPhase2Reco_cff import customiseForPhase2Reco
    process = customiseForPhase2Reco(process,"p", options.tTrigFilePh2, options.t0FilePh2)
