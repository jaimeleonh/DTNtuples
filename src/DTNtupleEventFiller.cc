/** \class DTNtupleEventFiller DTNtupleEventFiller.cc DTDPGAnalysis/DTNtuples/src/DTNtupleEventFiller.cc
 *  
 * Helper class : the digi filler for Phase-1 / Phase2 digis (the DataFormat is the same)
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleEventFiller.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

DTNtupleEventFiller::DTNtupleEventFiller(edm::ConsumesCollector && collector, 
					 const std::shared_ptr<DTNtupleConfig> config, 
					 std::shared_ptr<TTree> tree, const std::string & label) : 
  DTNtupleBaseFiller(config, tree, label)
{

  edm::InputTag iTag;
  iTag = m_config->m_inputTags["DTAB7_FED_Source"];
  //DTAB7InputTag_ = pset.getParameter<edm::InputTag>(cms.InputTag("rawDataCollector");
  rawToken_ = collector.consumes<FEDRawDataCollection>(iTag);
  

}

DTNtupleEventFiller::~DTNtupleEventFiller() 
{ 

};

void DTNtupleEventFiller::initialize()
{
  
  m_tree->Branch((m_label + "_runNumber").c_str(), &m_runNumber, (m_label + "_runNumber/I").c_str());
  m_tree->Branch((m_label + "_lumiBlock").c_str(), &m_lumiBlock, (m_label + "_lumiBlock/I").c_str());
  m_tree->Branch((m_label + "_eventNumber").c_str(), &m_eventNumber, (m_label + "_eventNumber/L").c_str());
  
  m_tree->Branch((m_label + "_timeStamp").c_str(), &m_timeStamp, (m_label + "_timeStamp/l").c_str());

  m_tree->Branch((m_label + "_bunchCrossing").c_str(), &m_bunchCrossing, (m_label + "_bunchCrossing/I").c_str());
  m_tree->Branch((m_label + "_orbitNumber").c_str(), &m_orbitNumber, (m_label + "_orbitNumber/L").c_str());
  
}

void DTNtupleEventFiller::clear()
{

  m_runNumber   = DTNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  m_lumiBlock   = DTNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  m_eventNumber = DTNtupleBaseFiller::DEFAULT_INT_VAL_POS;

  m_timeStamp   = 0;
  
  m_bunchCrossing = DTNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  m_orbitNumber   = DTNtupleBaseFiller::DEFAULT_INT_VAL_POS;
  
}

void DTNtupleEventFiller::fill(const edm::Event & ev)
{

  clear();
  // auto trigColl = conditionalGet<FEDRawDataCollection>(ev, rawToken_,"FEDRawDataCollection");
  
  m_runNumber   = ev.run();
  m_lumiBlock   = ev.getLuminosityBlock().luminosityBlock();
  m_eventNumber = ev.eventAuxiliary().event();

  m_timeStamp = ev.eventAuxiliary().time().value();

//  m_bunchCrossing = ev.eventAuxiliary().bunchCrossing();
  m_orbitNumber   = ev.eventAuxiliary().orbitNumber();

  //edm::EDGetTokenT<FEDRawDataCollection>  rawToken_ = consumes<FEDRawDataCollection>(DTAB7InputTag_);
  edm::Handle<FEDRawDataCollection> data = conditionalGet<FEDRawDataCollection>(ev, rawToken_,"FEDRawDataCollection"); 
  // ev.getByToken(rawToken_, data);

  FEDRawData dturosdata = data->FEDData(1368);
  if ( dturosdata.size() == 0 ) {
    return;
  }

  fedLinePointer_=dturosdata.data();
  long dataWord=0;


  // Reading the headers:
  
  readLine(&dataWord);  // Reading the word
  // calcCRC(dataWord);

  int bxCounter_=((dataWord>>20)&0xFFF);
  m_bunchCrossing = bxCounter_;

  return;

}
