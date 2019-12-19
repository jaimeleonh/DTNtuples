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
#files['norpc'].append('nu_pu250_noage_norpc')
files['norpc'].append('nu_pu250_noage_norpc')
files['norpc'].append('nu_pu250_age_norpc_youngseg_muonage_norpcage_fail_3000')
files['norpc'].append('norpc')
files['rpc'].append('nu_pu250_noage_withrpc')
files['rpc'].append('nu_pu250_age_withrpc_youngseg_muonage_norpcage_fail_3000')
files['rpc'].append('rpc')

eospath = "/eos/home-j/jleonhol/www/"

for cat in files : 
  for fil in files[cat] :
    rc = call('cp -r plotsRates/' + fil + ' ' + eospath + 'rateStudies/' , shell=True)
    rc = call('cp -r ' + eospath + 'rateStudies/index_php ' + eospath + 'rateStudies/' + fil + "/index.php" , shell=True)



effFiles = {'norpc':[], 'rpc':[], 'DM':[]}
effFiles['norpc'].append('PU200_range3')
effFiles['norpc'].append('pu200_age_norpc_youngseg_muonage_norpcage_fail_3000')
effFiles['rpc'].append('pu200_noage_withrpc')
effFiles['rpc'].append('pu200_age_withrpc_youngseg_muonage_norpcage_fail_3000')





