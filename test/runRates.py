import sys, os
import time
import ROOT as r
from ROOT import gSystem
from copy import deepcopy
import CMS_lumi
import myPlotter_input as effplot 
r.gROOT.SetBatch(True)
from subprocess import call

################################# CHANGE BEFORE RUNNING #######################################

categories = ['norpc', 'rpc']
files = {'norpc':[], 'rpc':[], 'DM':[]}
files['norpc'].append('nu_pu250_noage_norpc')
files['norpc'].append('nu_pu250_age_norpc_youngseg_muonage_norpcage_fail_3000')
files['rpc'].append('nu_pu250_noage_withrpc')
files['rpc'].append('nu_pu250_age_withrpc_youngseg_muonage_norpcage_fail_3000')
#files['rpc'].append('pu200_age_withrpc_youngseg_muonage_norpcage_fail_3000')

r.gROOT.ForceStyle()
r.TGaxis.SetMaxDigits( 2 )
r.gROOT.ForceStyle()


totalQualities = [ "AllBX", "GoodBX", "GoodBX+index0", "GoodBX+index01","GoodBX+index012","GoodBX+index0123","AllBX+qu>=3","GoodBX+qu>=3", "GoodBX+matchedqu<3", "GoodBX+qu>=3+RPCseg", "GodBX+qu>=3+RPCseg+clus","GoodBX+matchedqu<3+RPCseg", "GoodBX+matchedqu<3+RPCseg+clus" ];

qualities_dict = {}
for i in range(len(totalQualities)) :
  qualities_dict [totalQualities[i]] = i+1

legends = ['AM'] #FIXME if more than 1 algo is used (as a lot more stuff)

qualities = {}
qualities['norpc'] = ["GoodBX","GoodBX+qu>=3","GoodBX+index0", "GoodBX+index01","GoodBX+index012","GoodBX+index0123"]
qualities['rpc'] = ["GoodBX","GoodBX+qu>=3","GoodBX+matchedqu<3", "GoodBX+qu>=3+RPCseg", "GodBX+qu>=3+RPCseg+clus","GoodBX+matchedqu<3+RPCseg", "GoodBX+matchedqu<3+RPCseg+clus"]



plottingStuff = { 'lowlimityaxis' : 0,
		  'ranges' : {},
	          'markersize': 1,
	          'yaxistitleoffset': 1.5,
	          'xaxistitle': "Wheel",
	          'legxlow' : 0.3075 + 2 * 0.1975,
	          'legylow': 0.4,
	          'legxhigh': 0.9,
	          'legyhigh': 0.5,
	          'markertypedir':{},
	          'markercolordir':{}  
   	        }   
#plottingStuff['ranges'] = {"rates":[60E6,10E6,10E6,10E6,10E6,10E6,50E6,10E6], "bandwidths":[60E8,10E8,10E8,10E8,10E8,10E8,50E8,10E8] }
plottingStuff['ranges']['nu_pu250_noage_norpc'] = {"rates":[15E5,15E5,15E5,15E5,15E5,15E5], "bandwidths":[1E8,1E8,1E8,1E8,1E8,1E8] }
plottingStuff['ranges']['nu_pu250_age_norpc_youngseg_muonage_norpcage_fail_3000'] = {"rates":[15E5,15E5,15E5,15E5,15E5,15E5], "bandwidths":[1E8,1E8,1E8,1E8,1E8,1E8] }
plottingStuff['ranges']['nu_pu250_noage_withrpc'] = { "rates":[15E5,15E5,15E5,15E5,15E5,15E5,15E5], "bandwidths":[1E8,1E8,1E8,1E8,1E8,1E8,1E8] }
plottingStuff['ranges']['nu_pu250_age_withrpc_youngseg_muonage_norpcage_fail_3000'] = { "rates":[15E5,15E5,15E5,15E5,15E5,15E5,15E5], "bandwidths":[1E8,1E8,1E8,1E8,1E8,1E8,1E8] }

plottingStuffRat = { 'lowlimityaxis' : {},
	             'markersize': 1,
	             'yaxistitleoffset': 1.5,
	             'xaxistitle': "Wheel",
	             'legxlow' : 0.3075 + 2 * 0.1975,
	             'legylow': 0.4,
	             'legxhigh': 0.9,
	             'legyhigh': 0.5,
	             'markertypedir':{},
	             'markercolordir':{}  
   	           }   
plottingStuffRat['lowlimityaxis']['nu_pu250_noage_norpc'] = [0,0,0,0,0]
#plottingStuffRat['lowlimityaxis']['nu_pu250_noage_norpc'] = [0.4,0.4,0.4,0.4,0.4]
plottingStuffRat['lowlimityaxis']['nu_pu250_age_norpc_youngseg_muonage_norpcage_fail_3000'] = [0,0,0,0,0]
#plottingStuffRat['lowlimityaxis']['nu_pu250_age_norpc_youngseg_muonage_norpcage_fail_3000'] = [0.4,0.4,0.4,0.4,0.4]
plottingStuffRat['lowlimityaxis']['nu_pu250_noage_withrpc'] = [0,0,0,0,0,0,0]
#plottingStuffRat['lowlimityaxis']['nu_pu250_noage_withrpc'] = [0.5,0.5,0.5,0.5]
plottingStuffRat['lowlimityaxis']['nu_pu250_age_withrpc_youngseg_muonage_norpcage_fail_3000'] = [0,0,0,0,0,0,0]

#legends = {'norpc':[],'rpc':[], 'DM':[]}
#legends['norpc'] = ['All','Quality>2','index0','index01','index012','index0123']
#legends['rpc'] = ['All','Without Quality<3', 'With RPC-matched Quality<3' ,'Quality>2 + RPC hits','With RPC-matched Quality<3 + RPC hits']
#legends['DM'] = ['']


##############################################################################################

if len(sys.argv) >= 2 :
  if sys.argv[1] == 'yes' :
    print ("Starting ntuplizer for every sample in input")
    time.sleep(2)
    r.gInterpreter.ProcessLine(".x loadTPGSimAnalysis_Rates.C")
    gSystem.Load("/afs/cern.ch/user/j/jleonhol/calcEffs/CMSSW_10_6_0/src/DTDPGAnalysis/DTNtuples/test/./DTNtupleBaseAnalyzer_C.so")
    gSystem.Load("/afs/cern.ch/user/j/jleonhol/calcEffs/CMSSW_10_6_0/src/DTDPGAnalysis/DTNtuples/test/./DTNtupleTPGSimAnalyzer_Rates_C.so")
    from ROOT import DTNtupleTPGSimAnalyzer
elif len(sys.argv)==1 or (len(sys.argv)!=1 and sys.argv[1]!='yes'): 
  print("Not making ntuples. If you want to make them, restart with 'yes' as first argument ")
  time.sleep(2)

path = '/afs/cern.ch/work/j/jleonhol/public/'
ratePath = "./plotsRates/"
#outputPath = './ntuples/'
outputPath = '/eos/home-j/jleonhol/ntuplesResults/'
plotscaffold = { "rates": "ratePrims_{al}_{wh}_{se}_{st}", "bandwidths": "bandwidth_{al}_{wh}_{se}_{st}" }
savescaffold = { "rates": "hRates", "bandwidths": "hBandwidths" }
markerColors = [r.kBlue, r.kRed, r.kGreen, r.kOrange, r.kBlack, r.kMagenta]

for cat in files : 
  for fil in files[cat] :
    rc = call('mkdir ' + ratePath + fil, shell=True) 
    if len(sys.argv)==2 :
      if sys.argv[1] == 'yes' :
        print ('Obtaining rate ntuples for ' + fil)
        time.sleep(2) 
        analysis = DTNtupleTPGSimAnalyzer(path + fil + '.root', outputPath + 'results_rates_' + fil + '.root')
        analysis.Loop()

    print "\nBeginning plotting\n"


    for plot in ['bandwidths']: 
      for i in range (len(qualities[cat])) : 
        listofplots = []     
        plottingStuff['markertypedir']["h_" + "AM" + "_" + fil] = 20
        plottingStuff['markercolordir']["h_" + "AM" + "_" + fil] = markerColors[0]
        effplot.makeRatesPerRingplot(listofplots, "AM", fil, outputPath + 'results_rates_' + fil + '.root', qualities_dict[qualities[cat][i]], plotscaffold[plot])
        effplot.combineRatesPerRingplots(listofplots, i, legends, plot, plottingStuff, ratePath + fil, fil,  savescaffold[plot] )

        effplot.makeRatesPerSectorplot("AM", fil, outputPath + 'results_rates_' + fil + '.root', qualities_dict[qualities[cat][i]], plotscaffold[plot], plottingStuff, savescaffold[plot], plot, ratePath + fil, i)
        

    for plot in ['rates']: 
      listofplots = []     
      plottingStuff['markertypedir']["h_" + "AM" + "_" + fil] = 20
      plottingStuff['markercolordir']["h_" + "AM" + "_" + fil] = markerColors[0]
      effplot.makeRatesPerRingplot(listofplots, "AM", fil, outputPath + 'results_rates_' + fil + '.root', qualities_dict[qualities[cat][0]], plotscaffold[plot])
      effplot.combineRatesPerRingplots(listofplots, 0, legends, plot, plottingStuff, ratePath + fil, fil,  savescaffold[plot] )

      for i in range (1,len(qualities[cat])) : 
        listofplots2 = []     
        plottingStuff['markertypedir']["h_" + "AM" + "_" + fil] = 20
        plottingStuff['markercolordir']["h_" + "AM" + "_" + fil] = markerColors[0]
        effplot.makeRatesPerRingplot(listofplots2, "AM", fil, outputPath + 'results_rates_' + fil + '.root', qualities_dict[qualities[cat][i]], plotscaffold[plot])
        effplot.combineRatesPerRingplots(listofplots2, i, legends, plot, plottingStuff, ratePath + fil, fil,  savescaffold[plot] )
        effplot.combineRateRatiosPerRingplots(listofplots2, listofplots, ratePath + fil, fil, i, plottingStuffRat,legends)
      
for cat in files : 
  rc = call('mkdir ' + ratePath + cat, shell=True)
  if len(files[cat]) == 0 : continue 
  for plot in ['bandwidths']: 
    for i in range (len(qualities[cat])) : 
      listofplots = []     
      num=0
      for fil in files[cat] :
        plottingStuff['markertypedir']["h_" + "AM" + "_" + fil] = 20
        plottingStuff['markercolordir']["h_" + "AM" + "_" + fil] = markerColors[num]
        num+=1
        effplot.makeRatesPerRingplot(listofplots, "AM", fil, outputPath + 'results_rates_' + fil + '.root', qualities_dict[qualities[cat][i]], plotscaffold[plot])
      effplot.combineRatesPerRingplots(listofplots, i, ['No aging','Aging 3000fb^{-1}'], plot, plottingStuff, ratePath + cat, fil,  savescaffold[plot] )

        

  for plot in ['rates']: 
    listofplots2 = {}     
    for i in range (len(qualities[cat])) : 
      listofplots2[i] = []     
      num=0
      for fil in files[cat] :
        plottingStuff['markertypedir']["h_" + "AM" + "_" + fil] = 20
        plottingStuff['markercolordir']["h_" + "AM" + "_" + fil] = markerColors[num]
        num+=1
        effplot.makeRatesPerRingplot(listofplots2[i], "AM", fil, outputPath + 'results_rates_' + fil + '.root', qualities_dict[qualities[cat][i]], plotscaffold[plot])
      effplot.combineRatesPerRingplots(listofplots2[i], i, ['No aging','Aging 3000fb^{-1}'], plot, plottingStuff, ratePath + cat, fil,  savescaffold[plot] )
      effplot.combineRateRatiosPerRingplots(listofplots2[i], listofplots2[0], ratePath + cat, fil, i, plottingStuffRat, ['No aging','Aging 3000fb^{-1}'])





