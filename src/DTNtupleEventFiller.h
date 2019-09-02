#ifndef DTNtuple_DTNtupleEventFiller_h
#define DTNtuple_DTNtupleEventFiller_h

/** \class DTNtupleEventFiller DTNtupleEventFiller.h DTDPGAnalysis/DTNtuples/src/DTNtupleEventFiller.h
 *  
 * Helper class : the digi filler for Phase-1 / Phase2 digis (the DataFormat is the same)
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleBaseFiller.h"
#include <DataFormats/FEDRawData/interface/FEDRawData.h>
#include "FWCore/Framework/interface/ConsumesCollector.h"
#include "DataFormats/Common/interface/Handle.h"
#include "FWCore/Framework/interface/ESHandle.h"
#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/EventSetup.h"

#include <DataFormats/FEDRawData/interface/FEDRawDataCollection.h>
#include <FWCore/Framework/interface/EDProducer.h>
#include <FWCore/ParameterSet/interface/ParameterSet.h>

#include <vector>
#include <cstdint>

class DTNtupleEventFiller : public DTNtupleBaseFiller
{

 public:

  /// Constructor
  DTNtupleEventFiller(edm::ConsumesCollector && collector,
		      const std::shared_ptr<DTNtupleConfig> config, 
		      std::shared_ptr<TTree> tree, const std::string & label);
  //DTNtupleEventFiller(const std::shared_ptr<DTNtupleConfig> config, 
		     // std::shared_ptr<TTree> tree, const std::string & label);

  ///Destructor
  virtual ~DTNtupleEventFiller();
 
  /// Intialize function : setup tree branches etc ... 
  virtual void initialize() final;
  
  /// Clear branches before event filling 
  virtual void clear() final;

  /// Fill tree branches for a given events
  virtual void fill(const edm::Event & ev) final;    

 private :

//  edm::InputTag DTAB7InputTag_;
  edm::EDGetTokenT<FEDRawDataCollection>  rawToken_;
  
  int  m_runNumber;
  int  m_lumiBlock;
  int64_t m_eventNumber;

  uint64_t m_timeStamp;
  
  int  m_bunchCrossing;
  int64_t m_orbitNumber;
 
  unsigned char* fedLinePointer_;
  
  inline void readLine (long *dataWord) {
    (*dataWord)= *((long*)fedLinePointer_);
    fedLinePointer_+=8;
  }

 
};
  
#endif

