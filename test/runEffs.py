import sys, os
import time
import ROOT as r
from ROOT import gSystem
from copy import deepcopy
import CMS_lumi
import myPlotter_input as effplot 
r.gROOT.SetBatch(True)
from allLegends import legends
from subprocess import call
from bcolors import bcolors
from markerColors import markerColors

import argparse
parser = argparse.ArgumentParser(description='Plotter options')
parser.add_argument('-n','--ntuples', action='store_true', default = False)
parser.add_argument('-c','--compiler', action='store_true', default = False)
my_namespace = parser.parse_args()
################################# CHANGE BEFORE RUNNING #######################################

categories = ['norpc', 'rpc']
files = {'norpc':[], 'rpc':[], 'DM':[]}
files['DM'].append('PU0_DM_PT10-30_mod_2')
files['DM'].append('DM_NOPU_10-30')
#files['norpc'].append('nopu_noage_norpc')
#files['norpc'].append('PU200_mu_bkg7p5')
#files['norpc'].append('pu200_noage_norpc')
#files['norpc'].append('nopu_noage_norpc')
#files['norpc'].append('PU200_range3')
#files['norpc'].append('pu200_age_norpc_youngseg_muonage_norpcage_fail_3000')
#files['rpc'].append('pu200_noage_withrpc')
#files['rpc'].append('pu200_age_withrpc_youngseg_muonage_norpcage_fail_3000')

'''
Possible efficiency categories:
    - 'All' -> Every quality
    - 'correlated' -> Qualities 6,8 and 9
    - 'legacy' -> Qualities 3,4,6,7,8 and 9
    - 'nothreehits -> Quality > 2
    - 'index0' -> Only index 0
    - 'index01' -> Only indexes 0 and 1
    - 'index012' -> Only indexes 0, 1 and 2
    - 'index0123' -> Only indexes 0, 1, 2 and 3
    
    With useRPC = True : 
    - 'withmatchedthreehits' -> Quality > 2 + quality <= 2 matched with an RPC cluster or segment
    - 'qualityORSegs' -> Every DT quality + RPC segments
    - 'qualityORSegsClus' -> Every DT quality + RPC segments and clusters
    - 'qualityMatchedORSegs' -> Quality > 2 + quality <= 2 matched with an RPC cluster or segment + RPC segments
    - 'qualityMatchedORSegsClus' -> Quality > 2 + quality <= 2 matched with an RPC cluster or segment + RPC segments and clusters
'''

possibleQualities = ['All','correlated', 'legacy', 'index0', 'index01', 'index012', 'index0123', 'nothreehits', 'withmatchedthreehits' ,'qualityORSegs','qualityORSegsClus','qualityMatchedORSegs','qualityMatchedORSegsClus']




#qualities = ['']
qualities = {'norpc':[],'rpc':[], 'DM':[]}
#qualities['norpc'] = ['A']
qualities['norpc'] = ['All','nothreehits','correlated','legacy']
#qualities['norpc'] = ['All']
#qualities['norpc'] = ['','nothreehits']
#qualities['norpc'] = ['legacy']
#qualities['rpc'] = ['qualityMatchedORSegs','qualityMatchedORSegsClus']
qualities['rpc'] = ['All','nothreehits', 'withmatchedthreehits' ,'qualityORSegs','qualityORSegsClus','qualityMatchedORSegs','qualityMatchedORSegsClus']
qualities['DM'] = ['All']


##############################################################################################

if my_namespace.compiler == True :
  print ("Starting ntuplizer for every sample in input")
  time.sleep(2)
  r.gInterpreter.ProcessLine(".x loadTPGSimAnalysis_Effs.C")
if my_namespace.ntuples == True : 
  gSystem.Load("/afs/cern.ch/user/j/jleonhol/calcEffs/CMSSW_10_6_0/src/DTDPGAnalysis/DTNtuples/test/./DTNtupleBaseAnalyzer_C.so")
  gSystem.Load("/afs/cern.ch/user/j/jleonhol/calcEffs/CMSSW_10_6_0/src/DTDPGAnalysis/DTNtuples/test/./DTNtupleTPGSimAnalyzer_Efficiency_C.so")
  from ROOT import DTNtupleTPGSimAnalyzer
else :  
  print("Not making ntuples. If you want to make them, restart with 'yes' as first argument ")
  time.sleep(2)

path = '/afs/cern.ch/work/j/jleonhol/public/'
effPath = "./plotsEff/"
#outputPath = './ntuples/'
outputPath = '/eos/home-j/jleonhol/ntuplesResults/'

if not os.path.isdir(effPath) : rc = call('mkdir ' + effPath, shell=True)

for cat in files :  
  for fil in files[cat] :
    if my_namespace.ntuples == True :    
      for quality in qualities[cat] :
        print ('Obtaining efficiency ntuples for ' + fil + ' with quality type ' + quality )
        if quality not in possibleQualities :
          #print (  '\033[1;31m\033[91m' + 'ERROR: quality category not possible. It will not get considered in the ntuple production' + '\033[0m')
          print (  bcolors.red + 'ERROR: quality category does not exist. It will not get considered in the ntuple production' + bcolors.reset)
          continue
        time.sleep(2) 
        analysis = DTNtupleTPGSimAnalyzer(path + fil + '.root', outputPath + 'results_effis_' +fil + '_' + quality + '.root', quality)
        analysis.Loop()
   

    plottingStuff = { 'lowlimityaxis': 0.7,
		      'highlimityaxis': 1,
		      'markersize': 1,
		      'yaxistitle' : 'Efficiency (adim.)',
		      'yaxistitleoffset': 1.5,
		      'xaxistitle': "Wheel",
		      #'legxlow' : 0.5,
		      'legxlow' : 0.3075 + 2 * 0.1975,
		      #'legylow': 0.2,
		      'legylow': 0.4,
		      'legxhigh': 0.9,
		      'legyhigh': 0.6,
		      'markertypedir':{},
		      'markercolordir':{}, 
          'printPU':True
   		    }   

    for plot in ["Eff"] :
      plotscaffold = "h" + plot + "_{st}_{al}_{ty}"
      savescaffold = "h" + plot + "_" + fil 

      listofplots = [] 
      myLegends = []
 
      for i in range (len(qualities[cat])) : 
        if not os.path.isfile(outputPath + 'results_effis_' + fil + '_' + qualities[cat][i] + '.root') :
          if not qualities[cat][i] in legends :
            print (bcolors.red + "ERROR: '" +  qualities[cat][i]  + "' is not one of the possible qualities" + bcolors.reset)
          else :
            print (bcolors.red + 'ERROR: ' + outputPath + 'results_effis_' + fil + '_' + qualities[cat][i] + '.root + does not exist, maybe running the ntuple production helps') 
          continue 
        myLegends.append(legends[qualities[cat][i]])
        plottingStuff['markertypedir']["hEff_" + "AM" + "_" + qualities[cat][i]] = 20
        plottingStuff['markercolordir']["hEff_" + "AM" + "_" + qualities[cat][i]] = i+1
        effplot.makeresplot(listofplots, "AM", qualities[cat][i], outputPath + 'results_effis_' + fil + '_' + qualities[cat][i] + '.root', plotscaffold)


      print "\nCombining and saving\n"
      effplot.combineresplots(listofplots, myLegends, plottingStuff, effPath,  savescaffold+'_0' )
      #effplot.combineresplots(listofplots, legends[cat], plottingStuff, effPath,  savescaffold+'zoomIn' )


#if True : sys.exit(1)






#markerColors = []
print "\nBeginning plotting\n"





plottingStuff2 = {}
  
plottingStuff2['qualities1'] = { 'lowlimityaxis': 0.2,
		      'highlimityaxis': 1,
		      'markersize': 1,
		      'yaxistitle' : 'Efficiency (adim.)',
		      'yaxistitleoffset': 1.5,
		      'xaxistitle': "#eta",
		      'legxlow' : 0.84,
		      #'legxlow' : 0.3075 + 2 * 0.1975,
		      #'legylow': 0.2,
		      'legylow': 0.84,
		      'legxhigh': .99,
		      'legyhigh': .99,
		      'markertypedir':{},
		      'markercolordir':{}, 
          'printPU':True
   		    }   
plottingStuff2['qualities2'] = { 'lowlimityaxis': 0.5,
		      'highlimityaxis': 1,
		      'markersize': 1,
		      'yaxistitle' : 'Efficiency (adim.)',
		      'yaxistitleoffset': 1.5,
		      'xaxistitle': "#eta",
		      #'legxlow' : 0.5,
		      #'legxlow' : 0.3075 + 2 * 0.1975,
		      'legxlow' : 0.84,
		      #'legylow': 0.2,
		      'legylow': 0.84,
		      'legxhigh': .99,
		      'legyhigh': .99,
		      'markertypedir':{},
		      'markercolordir':{}, 
          'printPU':True
   		    }   
plottingStuff2['qualities3'] = { 'lowlimityaxis': 0.9,
		      'highlimityaxis': 1,
		      'markersize': 1,
		      'yaxistitle' : 'Efficiency (adim.)',
		      'yaxistitleoffset': 1.5,
		      'xaxistitle': "#eta",
		      #'legxlow' : 0.5,
		      #'legxlow' : 0.3075 + 2 * 0.1975,
		      'legxlow' : 0.84,
		      #'legylow': 0.2,
		      'legylow': 0.84,
		      'legxhigh': .99,
		      'legyhigh': .99,
		      'markertypedir':{},
		      'markercolordir':{}, 
          'PU': 200
   		    }   

    
#markerColors = [r.kBlue, r.kRed, r.kGreen, r.kOrange, r.kBlack, r.kMagenta]
chambTag = ["MB1", "MB2", "MB3", "MB4"]

Qualities = {'qualities1':[] , 'qualities2':[], 'qualities3':[] }
    
Qualities['qualities1'] = ['All', 'correlated']
Qualities['qualities2'] = ['nothreehits', 'legacy']
#Qualities['qualities3'] = ['All', 'nothreehits']
    


for ch in chambTag :
  for plot in ["EffEta"] :
    for key in ['qualities1','qualities2'] :
      listofplots = []   
      myLegends = []
      a=0
      for fil in files['norpc'] :
        plotscaffold = "h" + plot + "_" + ch +"_{al}_{ty}"
        savescaffold = "h" + plot + "_" + key + "_" + ch 

        for i in range (len(Qualities[key])) : 
          if not os.path.isfile(outputPath + 'results_effis_' + fil + '_' + Qualities[key][i] + '.root') :
            if not Qualities[key][i] in legends :
              print (bcolors.red + "ERROR: '" +  Qualities[key][i]  + "' is not one of the possible qualities" + bcolors.reset)
            else :
              print (bcolors.red + 'ERROR: ' + outputPath + 'results_effis_' + fil + '_' + Qualities[key][i] + '.root + does not exist, maybe running the ntuple production helps') 
            continue 
          myLegends.append(legends[Qualities[key][i]])
          plottingStuff2[key]['markertypedir']["hEff_" + "AM" + "_" + fil+Qualities[key][i]] = 20 + a*6
          plottingStuff2[key]['markercolordir']["hEff_" + "AM" + "_" + fil+Qualities[key][i]] = markerColors[i]
          effplot.makeWhateverResplot(listofplots, "AM", fil+Qualities[key][i], outputPath + 'results_effis_' + fil + '_' + Qualities[key][i] + '.root', plotscaffold)
        a+=1

      if (len (files['norpc']) > 0 ) :
        print "\nCombining and saving\n"
        effplot.combineEffPlots(listofplots, myLegends, plottingStuff2[key], effPath,  savescaffold+'_0' )


filesPU = ['nopu_noage_norpc','pu200_noage_norpc']

for ch in chambTag :
  for plot in ["EffEta"] :
    for key in ['qualities3'] :
      listofplots = []   
      myLegends = []
      a = 0
      for fil in filesPU :
        plotscaffold2 = "h" + plot + "_PU_" + ch +"_{al}_{ty}"
        savescaffold2 = "h" + plot + "_PU_" + key + "_" + ch 
        for i in range (len(Qualities[key])) :       
          if not os.path.isfile(outputPath + 'results_effis_' + fil + '_' + Qualities[key][i] + '.root') :
            if not Qualities[key][i] in legends :
              print (bcolors.red + "ERROR: '" +  Qualities[key][i]  + "' is not one of the possible qualities" + bcolors.reset)
            else :
              print (bcolors.red + 'ERROR: ' + outputPath + 'results_effis_' + fil + '_' + Qualities[key][i] + '.root + does not exist, maybe running the ntuple production helps') 
            continue 
          myLegends.append(legends[Qualities[key][i]])
          plottingStuff2[key]['markertypedir']["hEff_" + "AM" + "_" + fil+Qualities[key][i]] = 20 + a*6
          plottingStuff2[key]['markercolordir']["hEff_" + "AM" + "_" + fil+Qualities[key][i]] = markerColors[i]
          effplot.makeWhateverResplot(listofplots, "AM", fil+Qualities[key][i], outputPath + 'results_effis_' + fil + '_' + Qualities[key][i] + '.root', plotscaffold2)
        a+=1

      if (len(listofplots) > 0 ) :
        print "\nCombining and saving\n"
        effplot.combineEffPlots(listofplots, myLegends, plottingStuff2[key], effPath,  savescaffold2+'_0' )



filesDM = []
#filesDM = ['PU0_DM_PT10-30_mod_2_']
DMPath = './DMplots/'

chambTags = [ "MB1", "MB2", "MB3", "MB4"]
whTags    = [ "Wh.-2", "Wh.-1", "Wh.0", "Wh.+1", "Wh.+2"]



#for File in filesDM : 
for File in files['DM'] : 
  res = r.TFile.Open(outputPath + 'results_effis_' + File + '_All' + '.root')
  plottingStuff = { 'lowlimityaxis': 0,
 	            'highlimityaxis': 1,
	            'markersize': 1,
	            'yaxistitle' : 'Efficiency (adim.)',
	            'yaxistitleoffset': 1.5,
	            'xaxistitle': "Local Direction",
	            'legxlow' : 0.5,
	            'legylow': 0.3,
	            'legxhigh': 0.9,
	            'legyhigh': 0.35,
	            'markertypedir':{},
	            'markercolordir':{'AM':r.kRed, 'HB':r.kBlue}  
   		    }   

  legendsDM = ['AM']
  #legendsDM = ['AM', 'HB']
  plotList = []
  i = 0
  for algo in legendsDM :
    plotscaffold2 = "hEff_{al}_{ty}"
    #h_matched2 = res.Get('hEff_'  + algo +  '_matched') 
    #h_total2 = res.Get('hEff_' + algo + '_total')
    #h_Eff2 = r.TEfficiency(h_matched2, h_total2) 
    ##h_Eff2.SetMarkerColor(plottingStuff['markercolordir'][algo])
    #h_Eff2.SetLineColor(plottingStuff['markercolordir'][algo])
    #h_Eff2.SetMarkerSize(plottingStuff['markersize'])
    #plotList.append(h_Eff2)
    plottingStuff['markertypedir']["hEff_" + algo + "_" + File ] = 20
    plottingStuff['markercolordir']["hEff_" + algo + "_" + File ] = markerColors[i]
    i+=1
    effplot.makeWhateverResplot(plotList, algo, File, outputPath + 'results_effis_' + File + '_All'  + '.root', plotscaffold2)
  effplot.combineEffPlots(plotList, legendsDM, plottingStuff, DMPath, 'hEffVsSlope_' + File)
  
  for ch in chambTags :
    for wh in whTags : 
      plotList = []
      i=0
      for algo in legendsDM :
        plotscaffold2 = "hEff_" + wh + "_" + ch + "_{al}_{ty}"
        plottingStuff['markertypedir']["hEff_" + algo + "_" + File ] = 20
        plottingStuff['markercolordir']["hEff_" + algo + "_" + File ] = markerColors[i]
        i+=1
        effplot.makeWhateverResplot(plotList, algo, File, outputPath + 'results_effis_' + File + '_All'  + '.root', plotscaffold2)
      effplot.combineEffPlots(plotList, legendsDM, plottingStuff, DMPath, 'hEffVsSlope_' + wh + '_' + ch + '_' + File )
	 

  plottingStuff2 = { 'lowlimityaxis': 0,
 	            'highlimityaxis': 1,
	            'markersize': 1,
	            'yaxistitle' : 'Efficiency (adim.)',
	            'yaxistitleoffset': 1.5,
	            'xaxistitle': "Gen Part Lxy",
	            'legxlow' : 0.5,
	            'legylow': 0.3,
	            'legxhigh': 0.9,
	            'legyhigh': 0.35,
	            'markertypedir':{},
	            'markercolordir':{'AM':r.kRed, 'HB':r.kBlue}  
   		    }   
  
  for ch in chambTags :
    for wh in whTags : 
      plotList = []
      i=0
      for algo in legendsDM :
        plotscaffold2 = "hEffLxy_" + wh + "_" + ch + "_{al}_{ty}"
        plottingStuff2['markertypedir']["hEff_" + algo + "_" + File ] = 20
        plottingStuff2['markercolordir']["hEff_" + algo + "_" + File ] = markerColors[i]
        i+=1
        effplot.makeWhateverResplot(plotList, algo, File, outputPath + 'results_effis_' + File + '_All'  + '.root', plotscaffold2)
      effplot.combineEffPlots(plotList, legendsDM, plottingStuff, DMPath, 'hEffVsLxy_' + wh + '_' + ch + "_" + File )


#
