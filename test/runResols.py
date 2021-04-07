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
from allLegends import legends

import argparse
parser = argparse.ArgumentParser(description='Plotter options')
parser.add_argument('-n','--ntuples', action='store_true', default = False)
parser.add_argument('-r','--redoPlots', action='store_true', default = False)
my_namespace = parser.parse_args()

################################# CHANGE BEFORE RUNNING #######################################

categories = ['norpc', 'rpc']
files = {'norpc':[], 'rpc':[], 'DM':[]}
#files['norpc'].append('3h4h') 
#files['norpc'].append('nopu_noage_norpc') 
#files['norpc'].append('mu_pu200') 
#files['norpc'].append('DTDPGNtuple_11_1_0_patch2_Phase2_Simulation_8muInBarrel_woRPC')
#files['norpc'].append('mu_PU200_noRPC_noAgeing_111X_1_0')
#files['norpc'].append('mu_PU200_noRPC_noAgeing_20210223')
files['norpc'].append('mu_PU200_noRPC_noAgeing_20210308')
files['norpc'].append('mu_PU200_noRPC_noAgeing_20210315')
files['norpc'].append('mu_PU200_noRPC_noAgeing_20210315_cmssw')
#files['norpc'].append('mu_pu200_newest_analyzer')
#files['norpc'].append('mu_PU200_withRPC_noAgeing')
#files['norpc'].append('DTDPGNtuple_11_1_0_patch2_Phase2_Simulation_8muInBarrel')
#files['norpc'].append('mu_PU200_noRPC_noAgeing_3h4h')
#files['norpc'].append('mu_PU200_noRPC_noAgeing_grouping2')

#qualities = ['']
qualities = {'norpc':[],'rpc':[], 'DM':[]}
qualities['norpc'].append('All')
qualities['norpc'].append('Correlated')
qualities['norpc'].append('Legacy')
#qualities['norpc'].append('4h')
#qualities['norpc'].append('3h')

##############################################################################################

print "GOT IN!"

if my_namespace.ntuples == True: 
    print ("Starting ntuplizer for every sample in input")
    time.sleep(2)
    r.gInterpreter.ProcessLine(".x loadTPGSimAnalysis_Res_All.C")
    gSystem.Load("/afs/cern.ch/user/j/jleonhol/newResol/CMSSW_10_6_0/src/DTDPGAnalysis/DTNtuples/test/./DTNtupleBaseAnalyzer_C.so")
    gSystem.Load("/afs/cern.ch/user/j/jleonhol/newResol/CMSSW_10_6_0/src/DTDPGAnalysis/DTNtuples/test/./DTNtupleTPGSimAnalyzer_Resolution_All_C.so")
    from ROOT import DTNtupleTPGSimAnalyzer
else :
  print("Not making ntuples. If you want to make them, restart with 'yes' as first argument ")
  time.sleep(2)

path = '/eos/home-j/jleonhol/simulationSamples/'
plotsPath = "./summaryPlots/"
#outputPath = './ntuples/'
outputPath = '/eos/home-j/jleonhol/ntuplesResults/'
eosPath='/eos/home-j/jleonhol/www/resolutionsNote/'


chambTag = ["MB1", "MB2", "MB3", "MB4"]
wheelTag    = [ "Wh-2", "Wh-1", "Wh0", "Wh+1", "Wh+2"];
magnitude = ["Time", "Phi", "PhiB", "TanPsi", "x"]

plottingStuff = { 'lowlimityaxis': 0,
		      'highlimityaxis': {},
		      'markersize': 1,
              'yaxistitle' : {"Time":"Time resolution (ns)", "Phi":"Global Phi resolution (mrad)", "PhiB":"Bending Phi resolution (mrad)", "TanPsi":"Local direction resolution (mrad)", "x":"Position resolution (cm)"}, 
		      'yaxistitleoffset': 1.5,
		      'xaxistitle': "Wheel",
		      'legxlow' : 0.7,
		      'legylow': 0.15,
		      'legxhigh': 0.9,
		      'legyhigh': 0.25,
		      'markertypedir':{},
		      'markercolordir':{}  
   		    }

plottingStuff['highlimityaxis']['Time'] = {'3h': 10, '4h': 10, 'All':5, 'Correlated':5, 'Legacy':5}
plottingStuff['highlimityaxis']['Phi'] = {'3h': 50E-3, '4h':50E-3,'All':50E-3,'Correlated':50E-3, 'Legacy':50E-3}
plottingStuff['highlimityaxis']['PhiB'] = {'3h': 15,  '4h': 10, 'All':1, 'Correlated':1, 'Legacy':1}
plottingStuff['highlimityaxis']['TanPsi'] = {'3h': 15, '4h': 10, 'All':1, 'Correlated':1, 'Legacy':1}
plottingStuff['highlimityaxis']['x'] = {'3h': 0.02, '4h': 0.02, 'All': 0.02, 'Correlated': 0.02, 'Legacy':0.02}

markerColors = [r.kBlue, r.kRed, r.kGreen, r.kOrange, r.kBlack, r.kMagenta]



for cat in files :  
  for fil in files[cat] :
    if my_namespace.ntuples == True:
      print ('Obtaining resolution ntuples for ' + fil )
      time.sleep(2) 
      analysis = DTNtupleTPGSimAnalyzer(path + fil + '.root', outputPath + 'results_resols_' +fil + '_.root')
      analysis.Loop()

    if my_namespace.ntuples == True or my_namespace.redoPlots == True : 
      rc = call ('./runPlots.sh ' + fil, shell=True) 
    
    
    for mag in magnitude :
      for qual in qualities[cat] : 
        listofplots = []
        plotscaffold = "h" + mag + "Res_{al}_" + qual + "_{wh}"
        savescaffold = "h" + mag + "Res_{al}_" + qual

        plottingStuff['markertypedir']["h_" + "AM" + "_" + fil+qual] = 20
        plottingStuff['markercolordir']["h_" + "AM" + "_" + fil+qual] = markerColors[0]
        effplot.makeResolPlot(listofplots, "AM", fil+qual, plotsPath + fil + '/' +  'outPlots.root', plotscaffold)

        print "\nCombining and saving\n"
        effplot.combineResolPlots(listofplots, mag, qual, [], plottingStuff, plotsPath + fil + '/' + qual  + '/', savescaffold.format(al='AM') )
           
   # rc = call('cp -r ' + plotsPath + fil + ' ' + eosPath , shell=True)
   # rc = call('cp -r /eos/home-j/jleonhol/backup/index_resol_php ' + eosPath + fil + "/index.php" , shell=True)
   # for qual in qualities[cat] : rc = call('cp -r /eos/home-j/jleonhol/backup/index_resol_php ' + eosPath + fil + "/" + qual + "/index.php" , shell=True)
     
for cat in files :
  if not files[cat] : continue
  for mag in magnitude :
    for fil in files[cat] :
      listofplots = []
      num = 0
      for qual in qualities[cat] : 
        plotscaffold = "h" + mag + "Res_{al}_" + qual + "_{wh}"
        savescaffold = "h" + mag + "Res_{al}" 

        plottingStuff['markertypedir']["h_" + "AM" + "_" + fil+qual] = 20
        plottingStuff['markercolordir']["h_" + "AM" + "_" + fil+qual] = markerColors[num]
        num+=1
        effplot.makeResolPlot(listofplots, "AM", fil+qual, plotsPath + fil + '/' +  'outPlots.root', plotscaffold)

      print "\nCombining and saving\n"
      if not os.path.isdir(plotsPath + fil + '/mixed/') : os.mkdir(plotsPath + fil + '/mixed/')
      effplot.combineResolPlots(listofplots, mag, qual, qualities[cat], plottingStuff, plotsPath + fil + '/mixed/', savescaffold.format(al='AM') )


for cat in files :
    if not files[cat] : continue
    dirname = "{}/{}/".format(plotsPath, cat)
    if not os.path.isdir(dirname):
        os.mkdir(dirname)
    for mag in magnitude:
        for qual in qualities[cat] : 
            listofplots = []
            mylegends = []
            num = 0
            for fil in files[cat]:
                plotscaffold = "h" + mag + "Res_{al}_" + qual + "_{wh}"
                savescaffold = "h" + mag + "Res_{al}_" + qual 

                plottingStuff['markertypedir']["h_" + "AM" + "_" + fil+qual] = 20
                plottingStuff['markercolordir']["h_" + "AM" + "_" + fil+qual] = markerColors[num]
                num+=1
                effplot.makeResolPlot(listofplots, "AM", fil+qual, plotsPath + fil + '/' +  'outPlots.root', plotscaffold)
                print legends[fil]
                mylegends.append(legends[fil]) 

            print "\nCombining and saving\n"
            effplot.combineResolPlots(listofplots, mag, qual, mylegends, plottingStuff, dirname, savescaffold.format(al='AM') )



if True : sys.exit(1)






#markerColors = []
print "\nBeginning plotting\n"


###############################################################################################
###############################################################################################
###############################################################################################
###############################################################################################
###############################################################################################
###############################################################################################
###############################################################################################
#######################################     END     ###########################################
###############################################################################################
###############################################################################################
###############################################################################################
###############################################################################################
###############################################################################################
###############################################################################################
###############################################################################################
###############################################################################################


plottingStuff2 = {}
  
plottingStuff2['qualities1'] = { 'lowlimityaxis': 0.2,
		      'highlimityaxis': 1,
		      'markersize': 1,
		      'yaxistitle' : 'Efficiency (adim.)',
		      'yaxistitleoffset': 1.5,
		      'xaxistitle': "#eta",
		      'legxlow' : 0.85,
		      #'legxlow' : 0.3075 + 2 * 0.1975,
		      #'legylow': 0.2,
		      'legylow': 0.85,
		      'legxhigh': 1,
		      'legyhigh': 1,
		      'markertypedir':{},
		      'markercolordir':{}  
   		    }   
plottingStuff2['qualities2'] = { 'lowlimityaxis': 0.5,
		      'highlimityaxis': 1,
		      'markersize': 1,
		      'yaxistitle' : 'Efficiency (adim.)',
		      'yaxistitleoffset': 1.5,
		      'xaxistitle': "#eta",
		      #'legxlow' : 0.5,
		      #'legxlow' : 0.3075 + 2 * 0.1975,
		      'legxlow' : 0.85,
		      #'legylow': 0.2,
		      'legylow': 0.85,
		      'legxhigh': 1,
		      'legyhigh': 1,
		      'markertypedir':{},
		      'markercolordir':{}  
   		    }   
plottingStuff2['qualities3'] = { 'lowlimityaxis': 0.9,
		      'highlimityaxis': 1,
		      'markersize': 1,
		      'yaxistitle' : 'Efficiency (adim.)',
		      'yaxistitleoffset': 1.5,
		      'xaxistitle': "#eta",
		      #'legxlow' : 0.5,
		      #'legxlow' : 0.3075 + 2 * 0.1975,
		      'legxlow' : 0.85,
		      #'legylow': 0.2,
		      'legylow': 0.85,
		      'legxhigh': 1,
		      'legyhigh': 1,
		      'markertypedir':{},
		      'markercolordir':{}  
   		    }   

    
markerColors = [r.kBlue, r.kRed, r.kGreen, r.kOrange, r.kBlack, r.kMagenta]
chambTag = ["MB1", "MB2", "MB3", "MB4"]

Qualities = {'qualities1':[] , 'qualities2':[], 'qualities3':[] }
Legends = {'qualities1':[] , 'qualities2':[], 'qualities3':[] }
    
Qualities['qualities1'] = ['', 'correlated']
Qualities['qualities2'] = ['nothreehits', 'legacy']
Qualities['qualities3'] = ['', 'nothreehits']
    
Legends['qualities1'] = ['All', 'Correlated Only', 'All - Aged 3000fb^{-1}', 'Correlated Only - Aged 3000fb^{-1}']
#Legends['qualities1'] = ['All', 'Correlated Only']
Legends['qualities2'] = ['Quality > 2', 'Legacy', 'Quality > 2 - Aged 3000fb^{-1}', 'Legacy - Aged 3000fb^{-1}']
Legends['qualities3'] = ['No PU - All Qualities', 'No PU - Quality > 2', 'PU200 - All Qualities', 'PU200 - Quality > 2']
#Legends['qualities2'] = ['Quality > 2', 'Legacy qualities']


for ch in chambTag :
  for plot in ["EffEta"] :
    for key in ['qualities1','qualities2'] :
      listofplots = []   
      a = 0
      for fil in files['norpc'] :
        plotscaffold = "h" + plot + "_" + ch +"_{al}_{ty}"
        savescaffold = "h" + plot + "_" + key + "_" + ch 

        for i in range (len(Qualities[key])) : 
          plottingStuff2[key]['markertypedir']["hEff_" + "AM" + "_" + fil+Qualities[key][i]] = 20
          plottingStuff2[key]['markercolordir']["hEff_" + "AM" + "_" + fil+Qualities[key][i]] = markerColors[a]
          effplot.makeWhateverResplot(listofplots, "AM", fil+Qualities[key][i], outputPath + 'results_resols_' + fil + '_' + Qualities[key][i] + '.root', plotscaffold)
          a+=1

      print "\nCombining and saving\n"
      effplot.combineEffPlots(listofplots, Legends[key], plottingStuff2[key], effPath,  savescaffold+'_0' )


filesPU = ['nopu_noage_norpc','pu200_noage_norpc']

for ch in chambTag :
  for plot in ["EffEta"] :
    for key in ['qualities3'] :
      listofplots = []   
      a = 0
      for fil in filesPU :
        plotscaffold = "h" + plot + "_" + ch +"_{al}_{ty}"
        savescaffold = "h" + plot + "_" + key + "_" + ch 

        for i in range (len(Qualities[key])) : 
          plottingStuff2[key]['markertypedir']["hEff_" + "AM" + "_" + fil+Qualities[key][i]] = 20
          plottingStuff2[key]['markercolordir']["hEff_" + "AM" + "_" + fil+Qualities[key][i]] = markerColors[a]
          effplot.makeWhateverResplot(listofplots, "AM", fil+Qualities[key][i], outputPath + 'results_resols_' + fil + '_' + Qualities[key][i] + '.root', plotscaffold)
          a+=1

      print "\nCombining and saving\n"
      effplot.combineEffPlots(listofplots, Legends[key], plottingStuff2[key], effPath,  savescaffold+'_0' )



filesDM = []
#filesDM = ['PU0_DM_PT10-30_mod_2_']
DMPath = './DMplots/'

chambTags = [ "MB1", "MB2", "MB3", "MB4"]
whTags    = [ "Wh.-2", "Wh.-1", "Wh.0", "Wh.+1", "Wh.+2"]



for File in filesDM : 
  res = r.TFile.Open('./ntuples/results_resols_' + File + '.root')
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

  #legendsDM = ['AM']
  legendsDM = ['AM', 'HB']
  plotList = []
  for algo in legendsDM :
    h_matched2 = res.Get('hEff_'  + algo +  '_matched') 
    h_total2 = res.Get('hEff_' + algo + '_total')
    h_Eff2 = r.TEfficiency(h_matched2, h_total2) 
    h_Eff2.SetMarkerColor(plottingStuff['markercolordir'][algo])
    h_Eff2.SetLineColor(plottingStuff['markercolordir'][algo])
    h_Eff2.SetMarkerSize(plottingStuff['markersize'])
    plotList.append(h_Eff2)
  effplot.combineEffPlots(plotList, legendsDM, plottingStuff, DMPath, 'hEffVsSlope')
  
  for ch in chambTags :
    for wh in whTags : 
      plotList = []
      for algo in legendsDM :
        h_matched = res.Get('hEff_' + wh + "_" + ch + "_" + algo +  "_matched") 
        h_total = res.Get('hEff_' + wh + "_" + ch + "_" + algo + "_total")
        h_Eff = r.TEfficiency(h_matched, h_total) 
 	h_Eff.SetMarkerColor(plottingStuff['markercolordir'][algo])
 	h_Eff.SetLineColor(plottingStuff['markercolordir'][algo])
 	h_Eff.SetMarkerSize(plottingStuff['markersize'])
        plotList.append(h_Eff)
      effplot.combineEffPlots(plotList, legendsDM, plottingStuff, DMPath, 'hEffVsSlope_' + wh + '_' + ch )
	 


#
