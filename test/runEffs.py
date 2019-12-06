import sys, os
import time
import ROOT as r
from ROOT import gSystem
from copy import deepcopy
import CMS_lumi
import myPlotter_input as effplot 
r.gROOT.SetBatch(True)

################################# CHANGE BEFORE RUNNING #######################################

files = []
#files.append('DM_NOPU_pt10-30')
files.append('nopu_noage_norpc')
files.append('PU200_range3')
files.append('pu200_age_norpc_youngseg_muonage_norpcage_fail_3000')

#qualities = ['']
qualities = ['','nothreehits','index0','index01','index012','index0123']
legends = ['All','Quality>2','index0','index01','index012','index0123']


##############################################################################################

if len(sys.argv) > 2 :
  if sys.argv[1] == 'yes' :
    print ("Starting ntuplizer for every sample in input")
    time.sleep(2)
    r.gInterpreter.ProcessLine(".x loadTPGSimAnalysis.C")
    gSystem.Load("/afs/cern.ch/user/j/jleonhol/calcEffs/CMSSW_10_6_0/src/DTDPGAnalysis/DTNtuples/test/./DTNtupleBaseAnalyzer_C.so")
    gSystem.Load("/afs/cern.ch/user/j/jleonhol/calcEffs/CMSSW_10_6_0/src/DTDPGAnalysis/DTNtuples/test/./DTNtupleTPGSimAnalyzer_Efficiency_C.so")
    from ROOT import DTNtupleTPGSimAnalyzer
elif len(sys.argv)==1 or (len(sys.argv)!=1 and sys.argv[1]!='yes'): 
  print("Not making ntuples. If you want to make them, restart with 'yes' as first argument ")
  time.sleep(2)

path = '/afs/cern.ch/work/j/jleonhol/public/'
effPath = "./plotsEff/"
outputPath = './ntuples/'



for fil in files :
  if len(sys.argv)==2 :
    if sys.argv[1] == 'yes' :
      for quality in qualities :
        print ('Obtaining efficiency ntuples for ' + fil + ' with quality type ' + quality )
        time.sleep(2) 
        analysis = DTNtupleTPGSimAnalyzer(path + fil + '.root', outputPath + 'results_' +fil + '_' + quality + '.root', quality)
        analysis.Loop()
   

  plottingStuff = { 'lowlimityaxis': 0.9,
		    'highlimityaxis': 1,
		    'markersize': 1,
		    'yaxistitle' : 'Efficiency (adim.)',
		    'yaxistitleoffset': 1.5,
		    'xaxistitle': "Wheel",
		    'legxlow' : 0.3075 + 2 * 0.1975,
		    'legylow': 0.3,
		    'legxhigh': 0.9,
		    'legyhigh': 0.5,
		    'markertypedir':{},
		    'markercolordir':{}  
	  	  }   


  markerColors = [r.kBlue, r.kRed, r.kGreen, r.kOrange, r.kBlack, r.kMagenta]
  print "\nBeginning plotting\n"

  plotscaffold = "hEff_{st}_{al}_{ty}"
  savescaffold = "hEff_" + fil 

  listofplots = []     
  
  for i in range (len(qualities)) : 
    plottingStuff['markertypedir']["hEff_" + "AM" + "_" + qualities[i]] = 29
    plottingStuff['markercolordir']["hEff_" + "AM" + "_" + qualities[i]] = markerColors[i]
    effplot.makeresplot(listofplots, "AM", qualities[i], outputPath + 'results_' + fil + '_' + qualities[i] + '.root', plotscaffold)


  print "\nCombining and saving\n"
  effplot.combineresplots(listofplots, legends, plottingStuff, path,  savescaffold )



