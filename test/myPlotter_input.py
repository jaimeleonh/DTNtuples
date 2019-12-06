# -*- coding: utf-8 -*-
import ROOT as r
from copy import deepcopy
import CMS_lumi
import sys
r.gROOT.SetBatch(True)

############################################# CHANGE IF NECESSARY ###########################################################

def main() :

  path = "./plotsEff/"
  outputPath = './ntuples/'

  File = sys.argv[1]
  qualities = ['','nothreehits','index0','index01','index012','index0123']

  prefixes = "results_"
  suffixes = ".root"
  legends = ['All','Quality>2','index0','index01','index012','index0123']

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
		    'markerColors':[r.kBlue, r.kRed, r.kGreen, r.kOrange, r.kBlack, r.kMagenta],
		    'markertypedir':{},
		    'markercolordir':{}  
	  	  }   


  markerColors = [r.kBlue, r.kRed, r.kGreen, r.kOrange, r.kBlack, r.kMagenta]
  print "\nBeginning plotting\n"

  plotscaffold = "hEff_{st}_{al}_{ty}"
  savescaffold = "hEff_" + File 

  listofplots = []     
  
  for i in range (len(qualities)) : 
    plottingStuff['markertypedir']["hEff_" + "AM" + "_" + qualities[i]] = 29
    plottingStuff['markercolordir']["hEff_" + "AM" + "_" + qualities[i]] = markerColors[i]
    makeresplot(listofplots, "AM", qualities[i], outputPath + prefixes + File + '_' + qualities[i] + suffixes, plotscaffold)


  print "\nCombining and saving\n"
  combineresplots(listofplots, legends, plottingStuff, path,  savescaffold )


#############################################################################################################################



def makeresplot(hlist, algo, suffix, fileName, plotscaffold):
    chambTag = ["MB1", "MB2", "MB3", "MB4"]
    print "Obtaining intermediate plot for algo ", algo
    res = r.TFile.Open(fileName)
    hmatched = [res.Get(plotscaffold.format(al = algo, st = chambTag[ich], ty = "matched")) for ich in range(4)]
    htotal   = [res.Get(plotscaffold.format(al = algo, st = chambTag[ich], ty = "total")) for ich in range(4)]

    resplot = r.TH1F("hEff_{al}_{su}".format(al = algo, su = suffix), "", 20, -0.5, 19.5)
    
    ibin = 1
    for ich in range(4):
        for iwh in range(1, 6):
            resplot.SetBinContent(ibin, hmatched[ich].GetBinContent(iwh) / htotal[ich].GetBinContent(iwh))
            eff = r.TEfficiency('kk','',1,-0.5,0.5)
            eff.SetTotalEvents(1, int(htotal[ich].GetBinContent(iwh)))
            eff.SetPassedEvents(1,int(hmatched[ich].GetBinContent(iwh)))
            if (eff.GetEfficiencyErrorLow(1)-eff.GetEfficiencyErrorUp(1)) > 0.05: print 'warning, bin asymmetric'
            resplot.SetBinError( ibin, max(eff.GetEfficiencyErrorLow(1),eff.GetEfficiencyErrorUp(1)))
            del eff
            ibin += 1

    hlist.append(deepcopy(resplot))
    res.Close(); del hmatched, htotal, res, resplot
    return




def combineresplots(hlist, legends, plottingStuff, path, savescaffold):
    chambTag = ["MB1", "MB2", "MB3", "MB4"]
    print "Combining list of plots"
    if len(hlist) == 0: raise RuntimeError("Empty list of plots")
    c   = r.TCanvas("c", "c", 800, 800)
    c.SetLeftMargin(0.11)
    c.SetGrid()
    leg = r.TLegend(plottingStuff['legxlow'], plottingStuff['legylow'], plottingStuff['legxhigh'], plottingStuff['legyhigh'])
    hlist[0].SetStats(False)
    #hlist[0].SetTitle("L1 DT Phase 2 algorithm efficiency comparison")
    hlist[0].GetYaxis().SetRangeUser(plottingStuff['lowlimityaxis'], plottingStuff['highlimityaxis'])
    hlist[0].GetYaxis().SetTitleOffset(plottingStuff['yaxistitleoffset'])
    hlist[0].GetYaxis().SetTitle(plottingStuff['yaxistitle'])
    hlist[0].GetXaxis().SetTitle(plottingStuff['xaxistitle'])
    hlist[0].GetXaxis().SetNdivisions(120)
    ilabel = 1
    for ich in range(4):
        for iwh in range(-2, 3):
            hlist[0].GetXaxis().ChangeLabel(ilabel, -1, -1, -1, -1, -1, (iwh > 0) * "+" + str(iwh))
            ilabel += 1

   
    for iplot in range(len(hlist)):
        hlist[iplot].SetMarkerSize(plottingStuff['markersize'])
        hlist[iplot].SetMarkerStyle(plottingStuff['markertypedir'][hlist[iplot].GetName()])
        hlist[iplot].SetMarkerColor(plottingStuff['markercolordir'][hlist[iplot].GetName()])
        leg.AddEntry(hlist[iplot], legends[iplot], "P")
        hlist[iplot].Draw("P,hist" + (iplot != 0) * "same")

    leg.Draw()

    textlist = []
    linelist = []
    for ich in range(4):
        textlist.append(r.TText(.17 + ich * 0.1975, 0.20, chambTag[ich]))
        textlist[-1].SetNDC(True)
        textlist[-1].Draw("same")
        if ich != 3:
            linelist.append(r.TLine(0.3075 + ich * 0.1975, 0.1, 0.3075 + ich * 0.1975, 0.9))
            linelist[-1].SetNDC(True)
            linelist[-1].Draw("same")

    #cmslat = r.TLatex()
    #cmslat.SetTextSize(0.03);
    #cmslat.DrawLatexNDC(0.11, 0.91, "#scale[1.5]{CMS}");
    #cmslat.Draw("same");

    CMS_lumi.lumi_13TeV = ""
    CMS_lumi.extraText  = 'Simulation - No ageing'
    CMS_lumi.cmsTextSize= 0.5
    CMS_lumi.lumi_sqrtS = ''
    CMS_lumi.CMS_lumi(r.gPad, 0, 0, 0.07)


    #c.SetLogy()
    c.SaveAs(path + savescaffold + ".png")
    c.SaveAs(path + savescaffold + ".pdf")
    c.SaveAs(path + savescaffold + ".root")
    c.Close(); del c
    return


if __name__ == '__main__':
  main()



