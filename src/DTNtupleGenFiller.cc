/** \class DTNtupleGenFiller DTNtupleGenFiller.cc DTDPGAnalysis/DTNtuples/src/DTNtupleGenFiller.cc
 *  
 * Helper class : the gen particles filler
 *
 * \author C. Battilana (INFN BO)
 *
 *
 */

#include "DTDPGAnalysis/DTNtuples/src/DTNtupleGenFiller.h"

#include "FWCore/Framework/interface/Event.h"
#include "FWCore/Framework/interface/LuminosityBlock.h"

#include "FWCore/Framework/interface/EDAnalyzer.h"
#include "FWCore/Framework/interface/ESHandle.h"

DTNtupleGenFiller::DTNtupleGenFiller(edm::ConsumesCollector && collector,
				     const std::shared_ptr<DTNtupleConfig> config, 
				     std::shared_ptr<TTree> tree, const std::string & label) : 
  DTNtupleBaseFiller(config, tree, label)
{

  edm::InputTag & iTag = m_config->m_inputTags["genPartTag"];
  if (iTag.label() != "none") m_genPartToken = collector.consumes<reco::GenParticleCollection>(iTag);

}

DTNtupleGenFiller::~DTNtupleGenFiller() 
{ 

};

void DTNtupleGenFiller::initialize()
{
  
  m_tree->Branch((m_label + "_nGenParts").c_str(), &m_nGenParts, (m_label + "_nGenParts/i").c_str());

  m_tree->Branch((m_label + "_pdgId").c_str(), &m_gen_pdgId);

  m_tree->Branch((m_label + "_pt").c_str(),     &m_gen_pt);
  m_tree->Branch((m_label + "_phi").c_str(),    &m_gen_phi);
  m_tree->Branch((m_label + "_eta").c_str(),    &m_gen_eta);
  m_tree->Branch((m_label + "_charge").c_str(), &m_gen_charge);
  m_tree->Branch((m_label + "_lxy").c_str(),    &m_gen_lxy);
  m_tree->Branch((m_label + "_dxy").c_str(),    &m_gen_dxy);
  
}

void DTNtupleGenFiller::clear()
{

  m_nGenParts = 0;

  m_gen_pdgId.clear();

  m_gen_pt.clear();
  m_gen_phi.clear();
  m_gen_eta.clear();
  m_gen_charge.clear();
  m_gen_lxy.clear();
  m_gen_dxy.clear();

}

void DTNtupleGenFiller::fill(const edm::Event & ev)
{

  clear();

  auto genParts = conditionalGet<reco::GenParticleCollection>(ev, m_genPartToken,"GenParticleCollection");

  if (genParts.isValid()) 
    {

      for (const auto & genPart : (*genParts))
	{

	  if(genPart.status() != 1)
	    continue;
      
	  m_gen_pdgId.push_back(genPart.pdgId());

	  m_gen_pt.push_back(genPart.pt());
	  m_gen_eta.push_back(genPart.eta());
	  m_gen_phi.push_back(genPart.phi());
	  m_gen_charge.push_back(genPart.charge());

          double LXY = sqrt( genPart.vertex().x()*genPart.vertex().x() + genPart.vertex().y()*genPart.vertex().y() );
          double DXY = -genPart.vertex().x()*sin(genPart.phi()) + genPart.vertex().y()*cos(genPart.phi()) ;
	  m_gen_lxy.push_back(LXY);
	  m_gen_dxy.push_back(DXY);

	  
	  ++m_nGenParts;
	}
    }

  return;

}
