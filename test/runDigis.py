import sys, os
import time
import ROOT as r
from ROOT import gSystem
from copy import deepcopy
import CMS_lumi
#import myPlotter_input as effplot 
r.gROOT.SetBatch(True)
from subprocess import call
import myPlotter_input as effplot
from markerColors import markerColors

################################# CHANGE BEFORE RUNNING #######################################

categories = ['norpc', 'rpc']
files = {'norpc':[], 'rpc':[], 'DM':[]}
files['norpc'].append('PU200_bkgHits') 

#qualities = ['']
qualities = {'norpc':[],'rpc':[], 'DM':[]}
qualities['norpc'].append('4h')
qualities['norpc'].append('3h')

##############################################################################################

if len(sys.argv) >= 3 :
  if sys.argv[2] == 'yes' :
    print ("Starting ntuplizer for every sample in input")
    time.sleep(2)
    r.gInterpreter.ProcessLine(".x loadTPGSimAnalysis_Digis.C")
    gSystem.Load("/afs/cern.ch/user/j/jleonhol/newResol/CMSSW_10_6_0/src/DTDPGAnalysis/DTNtuples/test/./DTNtupleBaseAnalyzer_C.so")
    gSystem.Load("/afs/cern.ch/user/j/jleonhol/newResol/CMSSW_10_6_0/src/DTDPGAnalysis/DTNtuples/test/./DTNtupleTPGSimAnalyzer_Digis_C.so")
    from ROOT import DTNtupleTPGSimAnalyzer
elif len(sys.argv)==1 or (len(sys.argv)!=1 and sys.argv[1]!='yes'): 
  print("Not making ntuples. If you want to make them, restart with 'yes' as first argument ")
  time.sleep(2)

path = '/afs/cern.ch/work/j/jleonhol/public/'
plotsPath = "./digiPlots/"
#outputPath = './ntuples/'
outputPath = '/eos/home-j/jleonhol/ntuplesResults/'
eosPath='/eos/home-j/jleonhol/www/resolutionsNote/'

if not os.path.isdir(plotsPath) : rc = call('mkdir ' + plotsPath, shell=True)

chambTag = ["MB1", "MB2", "MB3", "MB4"]
wheelTag    = [ "Wh-2", "Wh-1", "Wh0", "Wh+1", "Wh+2"];
magnitude = ["Time", "Phi", "PhiB", "TanPsi", "x"]

plottingStuff = { 'lowlimityaxis': 0,
		      'highlimityaxis': {},
		      'markersize': 1,
              'yaxistitle' : {"Time":"Time resolution (ns)", "Phi":"Global Phi resolution (mrad)", "PhiB":"Bending Phi resolution (mrad)", "TanPsi":"Local direction resolution (mrad)", "x":"Position resolution (cm)"}, 
		      'yaxistitleoffset': 1.5,
		      'xaxistitle': "Wheel",
		      'legxlow' : 0.85,
		      'legylow': 0.85,
		      'legxhigh': 0.99,
		      'legyhigh': 0.99,
		      'markertypedir':{},
		      'markercolordir':{}  
   		    }

plottingStuff['highlimityaxis']['Time'] = {'3h': 5, '4h': 5}
plottingStuff['highlimityaxis']['Phi'] = {'3h': 50E-3, '4h':50E-3}
plottingStuff['highlimityaxis']['PhiB'] = {'3h': 15,  '4h': 10}
plottingStuff['highlimityaxis']['TanPsi'] = {'3h': 15, '4h': 10}
plottingStuff['highlimityaxis']['x'] = {'3h': 0.02, '4h': 0.02}

#markerColors = [r.kBlue, r.kRed, r.kGreen, r.kOrange, r.kBlack, r.kMagenta]



for cat in files :  
  for fil in files[cat] :
    if len(sys.argv)>=3 :
      if sys.argv[2] == 'yes' :
        print ('Obtaining resolution ntuples for ' + fil )
        time.sleep(2) 
        analysis = DTNtupleTPGSimAnalyzer(path + fil + '.root', outputPath + 'results_digis_' +fil + '_.root')
        analysis.Loop()
   
