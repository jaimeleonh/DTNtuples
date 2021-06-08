import FWCore.ParameterSet.Config as cms

dtNtupleProducer = cms.EDAnalyzer("DTNtupleProducer",
                                  genPartTag = cms.untracked.InputTag("none"),

                                  puInfoTag = cms.untracked.InputTag("none"),
                                  lumiScalerTag = cms.untracked.InputTag("scalersRawToDigi"),
                                  primaryVerticesTag = cms.untracked.InputTag("offlinePrimaryVertices"),

                                  ph1DtDigiTag = cms.untracked.InputTag("muonDTDigis"),
                                  ph2DtDigiTag = cms.untracked.InputTag("muonDTDigis"),

                                  ph1TwinMuxInTag  = cms.untracked.InputTag("twinMuxStage2Digis","PhIn"),
                                  ph1TwinMuxOutTag = cms.untracked.InputTag("twinMuxStage2Digis","PhOut"),
                                  ph1BmtfInTag  = cms.untracked.InputTag("bmtfDigis"),

                                  ph1TwinMuxInThTag = cms.untracked.InputTag("twinMuxStage2Digis","ThIn"),
                                  ph1BmtfInThTag  = cms.untracked.InputTag("bmtfDigis"),

                                  ph2TPGPhiHwTag  = cms.untracked.InputTag("none"),
                                  ph2TPGPhiEmuHbTag  = cms.untracked.InputTag("none"),
                                  ph2TPGPhiEmuAmTag  = cms.untracked.InputTag("dtTriggerPhase2AmPrimitiveDigis"),

                                  ph1DtSegmentTag = cms.untracked.InputTag("dt4DSegments"),        
                                  ph2DtSegmentTag = cms.untracked.InputTag("none"),

                                  tTrigMode = cms.untracked.string('DTTTrigSyncFromDB'),
                                  tTrigModeConfig = cms.untracked.PSet(vPropWire = cms.double(24.4),
                                    doTOFCorrection = cms.bool(False),
                                    tofCorrType = cms.int32(2),
                                    wirePropCorrType = cms.int32(0),
                                    doWirePropCorrection = cms.bool(False),
                                    doT0Correction = cms.bool(True),
                                    tTrigLabel = cms.string(''),
                                    debug = cms.untracked.bool(False),
                                    t0Label = cms.string('')
                                  ),
)

