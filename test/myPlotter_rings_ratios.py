# -*- coding: utf-8 -*-
import ROOT as r
from copy import deepcopy
import CMS_lumi
r.gROOT.SetBatch(True)

############################################# CHANGE IF NECESSARY ###########################################################


print "\nBeginning plotting\n"
#path = "./plotsEff_allThree/"
path = "./ratePlots/"
savescaffold = "h_ratios"
stuff = ["outPlots_ratios"]
#stuff = ["QualityFilter", "allThree"]
prefixes = [""]
suffixes = [".root"]
legends = ["AM"]
#legends.append["HB"]
#legends = ["Only Q5", "Q5+Q3 primos"]


#############################################################################################################################



plotscaffold = "ratePrims_{al}_{wh}_{st}"
chambTag = ["MB1", "MB2", "MB3", "MB4"]
wheelTag = [ "Wh-2", "Wh-1", "Wh0", "Wh+1", "Wh+2"];
#sectorTag = ["Sec1","Sec2","Sec3","Sec4","Sec5","Sec6","Sec7","Sec8","Sec9","Sec10","Sec11","Sec12"]
#ranges = [60E6,15E6,15E6,15E6,15E6,15E6,50E6]
suffix = ""

def makeresplot(hlist, algo, suffix, fileName,binToUse):
    print "Obtaining intermediate plot for algo ", algo
    res = r.TFile.Open(fileName)
    #hmatched = [res.Get(plotscaffold.format(mag = magnit + "Res", qu = quality, al = algo, wh = wheelTag[iwh])) for iwh in range(5)]
    hmatched = []
    for ich in range(4):
      for iwh in range(5):
        #print (plotscaffold.format(al = algo, wh = wheelTag[iwh], st=chambTag[ich]))
        hmatched.append(res.Get(plotscaffold.format(al = algo, wh = wheelTag[iwh], st=chambTag[ich]) ))
    resplot = r.TH1F("h_{al}_{su}".format(su = suffix, al = algo), "", 20, -0.5, 19.5)
    
    #resplot=r.TH1F("hEff_{al}_{su}".format(al = algo, su = suffix), "", 20, -0.5, 19.5)
        #resplot = r.TH1F("hEff_{al}_{su}".format(al = algo, su = suffix), "", 20, -0.5, 19.5)
    #resplot = r.TH1F("h_{al}_{su}".format(al = algo, su = suffix), "", 20, -0.5, 19.5)
    
    ibin = 1
    resplot.GetYaxis().SetTitle(hmatched[0].GetXaxis().GetBinLabel(binToUse))
    for ich in range(4):
      for iwh in range(5):
        resplot.SetBinContent(ibin, hmatched[ 5*ich + iwh ].GetBinContent(binToUse))
        ibin += 1

    hlist.append(deepcopy(resplot))
    res.Close(); del hmatched, res, resplot
    return


lowlimityaxis  = 0
highlimityaxis = 1
markersize     = 1
#yaxistitle     = "Rate ()"
#yaxistitleoffset= 1.5
yaxistitleoffset= 1.5 
#units = "(ns)"
xaxistitle     = "Wheel"
legxlow        = 0.55 
legylow        = 0.95
legxhigh       = 0.65
legyhigh       = 0.9


def combineresplots(hlist, binToUse, legends):
    print "Combining list of plots"
    if len(hlist) == 0: raise RuntimeError("Empty list of plots")
    c   = r.TCanvas("c", "c", 800, 800)
    c.SetLeftMargin(0.11)
    c.SetGrid()
    leg = r.TLegend(legxlow, legylow, legxhigh, legyhigh)
    hlist[0].SetStats(False)
    #hlist[0].SetTitle("L1 DT Phase 2 algorithm efficiency comparison")
    hlist[0].GetYaxis().SetRangeUser(lowlimityaxis, highlimityaxis)
    hlist[0].GetYaxis().SetTitleOffset(yaxistitleoffset)
    #hlist[0].GetYaxis().SetTitle(magnitude[index] + " resolution " + units[index])
    hlist[0].GetXaxis().SetTitle(xaxistitle)
    hlist[0].GetXaxis().SetNdivisions(120)
    ilabel = 1
    for ich in range(4):
        for iwh in range(-2, 3):
            hlist[0].GetXaxis().ChangeLabel(ilabel, -1, -1, -1, -1, -1, (iwh > 0) * "+" + str(iwh))
            ilabel += 1

    for iplot in range(len(hlist)):
        hlist[iplot].SetMarkerSize(markersize)
        hlist[iplot].SetMarkerStyle(markertypedir[hlist[iplot].GetName()])
        hlist[iplot].SetMarkerColor(markercolordir[hlist[iplot].GetName()])
        leg.AddEntry(hlist[iplot], legends[iplot], "P")
        hlist[iplot].Draw("P,hist" + (iplot != 0) * "same")

    leg.Draw()

    textlist = []
    linelist = []
    for ich in range(4):
        textlist.append(r.TText(.17 + ich * 0.1975, 0.30, chambTag[ich]))
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

    #CMS_lumi.lumi_13TeV = ""
    #CMS_lumi.extraText  = 'Simulation'
    #CMS_lumi.cmsTextSize= 0.5
    #CMS_lumi.lumi_sqrtS = ''
    #CMS_lumi.CMS_lumi(r.gPad, 0, 0, 0.07)

    firsttex = r.TLatex()
    firsttex.SetTextSize(0.03)
    firsttex.DrawLatexNDC(0.11,0.91,"#scale[1.5]{       CMS} Phase-2 Simulation")
    firsttex.Draw("same");

    secondtext = r.TLatex()
    toDisplay  = r.TString("14 TeV, 200 PU")
    secondtext.SetTextSize(0.035)
    secondtext.SetTextAlign(31)
    secondtext.DrawLatexNDC(0.90, 0.91, toDisplay.Data())
    secondtext.Draw("same")


    #c.SetLogy()
    c.SaveAs(path + savescaffold  + "_" + str(binToUse) + ".png")
    c.SaveAs(path + savescaffold  + "_" + str(binToUse) + ".pdf")
    c.SaveAs(path + savescaffold  + "_" + str(binToUse) + ".root")
    c.Close(); del c
    return



markertypedir  = {}
markertypedir["h_" + "AM" + "_" + stuff[0]] = 20
#markertypedir["h_" + "HB" + "_" + stuff[0]] = 20
##markertypedir["hEff_" + "AM" + "_" + stuff[1]] = 29
#markertypedir["AM+RPC_age"] = 29
#markertypedir["AM+RPC_noage"] = 29
#markertypedir["AM+RPC_age"] = 34
#markertypedir["AM+RPC_noage"] = 34
#markertypedir["HB_noage"] = 22
#markertypedir["HB_age"] = 22

markercolordir  = {}
markercolordir["h_" + "AM" + "_" + stuff[0]] = r.kBlue
#markercolordir["h_" + "HB" + "_" + stuff[0]] = r.kRed
#markercolordir["AM+RPC_noage"] = r.kBlue
#markercolordir["HB_noage"] = r.kBlue
#markercolordir["AM_noage"] = r.kBlue
#markercolordir["HB_age"] = r.kRed

listofplots1     = []
listofplots2     = []
listofplots3     = []
listofplots4     = []
listofplots5     = []
makeresplot(listofplots1, "AM", stuff[0], stuff[0]+suffixes[0],1)
makeresplot(listofplots2, "AM", stuff[0], stuff[0]+suffixes[0],2)
makeresplot(listofplots3, "AM", stuff[0], stuff[0]+suffixes[0],3)
makeresplot(listofplots4, "AM", stuff[0], stuff[0]+suffixes[0],4)
makeresplot(listofplots5, "AM", stuff[0], stuff[0]+suffixes[0],5)
#makeresplot(listofplots1, "HB", stuff[0], stuff[0]+suffixes[0],1)
#makeresplot(listofplots, False, "HB", False)
#makeresplot(listofplots, True, "HB", False)
#makeresplot(listofplots, False, "AM+RPC", False)
#makeresplot(listofplots, True,  "AM+RPC", False)

#for bin in range(1, listofplots[-1].GetNbinsX() + 1):
    #print "bin", bin, "contenido", listofplots[-1].GetBinContent(bin)

#makeresplot(puedlistofplots, False, "AM", True)
#makeresplot(puedlistofplots, True,  "AM", True)
#makeresplot(puedlistofplots, False, "HB", True)
#makeresplot(puedlistofplots, True , "HB", True)
#makeresplot(puedlistofplots, False, "AM+RPC", True)
#makeresplot(puedlistofplots, True,  "AM+RPC", True)

print "\nCombining and saving\n"
combineresplots(listofplots1, 1, legends)
combineresplots(listofplots2, 2, legends)
combineresplots(listofplots3, 3, legends)
combineresplots(listofplots4, 4, legends)
combineresplots(listofplots5, 5, legends)
#combineresplots(puedlistofplots, True)
