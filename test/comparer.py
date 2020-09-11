import os, sys
from sl3offsets import sl3offsets as offsets
import ROOT as r
r.gROOT.SetBatch(True)
r.gStyle.SetOptStat(111111)

quality_names = ["3h", "4h", "Q6", "Q8", "Q9"]
stations = ["mb1", "mb2", "mb3", "mb4"]
categories = ["all", "correlated", "uncorrelated"]


def parse_args():
    import argparse
    parser = argparse.ArgumentParser(description='Some options')
    parser.add_argument('-r', '--runNumber', type=int, default = 335947)
    parser.add_argument('-s', '--binStep', type=int, default = 500)
    parser.add_argument('-d', '--debug', action="store_true", default = False)
    parser.add_argument('-n', '--nEvents', type=int, default = None)
    parser.add_argument('-c', '--copy', action="store_false", default = True)
    return parser.parse_args()

def get_labels(upper_left="Preliminary", upper_right="DT SliceTest, Cosmic Data (2020)", inner_text = None):
    labels = []
    if upper_left:
        label = r.TLatex(0.02, 0.91, "#scale[1.5]{       CMS} " + "{}".format(upper_left))
        label.SetNDC(True)
        label.SetTextSize(0.03)
        labels.append(label)
    if upper_right:
        label = r.TLatex(0.48, 0.91, "{}".format(upper_right))
        label.SetNDC(True)
        label.SetTextSize(0.03)
        labels.append(label)
    if inner_text:
        y = 0.87
        for text in inner_text:
            label = r.TLatex(0.76, y, "{}".format(text))
            label.SetNDC(True)
            label.SetTextSize(0.03)
            labels.append(label)
            y -= 0.05
    return labels
        
        


def str2num(text):
  try:
    mynum = int(text) if not '.' in text else float(text)
    return mynum
  except ValueError:
    return text


def get_quality_bin(prim_emu):
    if prim_emu["quality"] == 1 or prim_emu["quality"] == 2:
        return 0
    elif prim_emu["quality"] == 3 or prim_emu["quality"] == 4:
        return 1
    elif prim_emu["quality"] == 6:
        return 2
    elif prim_emu["quality"] == 8:
        return 3
    elif prim_emu["quality"] == 9:
        return 4
    return -1  # failed

def get_station(station):
    return "mb{}".format(station)

def same_hits(prim_fw, prim_emu):
    for i in range(8):
        if prim_fw["lat" + str(i)] == -1 and prim_emu["lat" + str(i)] == -1:
            continue
        elif prim_fw["lat" + str(i)] == -1 or prim_emu["lat" + str(i)] == -1:
            return False
        else:
            tdc_fw = prim_fw["tdc" + str(i)]
            tdc_emu = int((prim_emu["tdc" + str(i)] - prim_emu["time"])) / 32
            lat_fw = prim_fw["lat" + str(i)]
            lat_emu = prim_emu["lat" + str(i)]
            wi_fw = prim_fw["wi" + str(i)]
            wi_emu = prim_emu["wi" + str(i)]
            if abs(tdc_fw - tdc_emu)>1 or lat_fw != lat_emu or wi_fw != wi_emu:
                return False
    return True


def count_events(File):
    with open(os.path.expandvars(File), 'r') as f:
        n_events = 0
        for l in f:
            line = map(str2num,l.strip().split(' '))
            if len(line) == 1:
                n_events += 1
                continue
    return n_events


def make_plots(n_events, options):
    position_axis = (100, -0.5, 0.5)
    slope_axis = (100, -0.02, 0.02)
    time_axis = (21, -10.5, 10.5)
    phi_axis = (100, -0.001, 0.001)
    phib_axis = (100, -0.02, 0.02)
    quality_axis = (5, -0.5, 4.5)
    multiplicity_axis = (31, -0.5, 30.5)
    
    plots = {}
    lists = {}
    effplots = {}
    
    plots["fw_quality"] = r.TH1F("fw_quality", "; quality; events", *quality_axis)
    plots["emu_quality"] = r.TH1F("emu_quality", "; quality; events", *quality_axis)
    
    lists["mean_fw_quality"] = []
    lists["mean_emu_quality"] = []
    
    effplots["passed"] = r.TH1F("hpassed", "; quality; events", *quality_axis)
    effplots["total"] = r.TH1F("htotal", "; quality; events", *quality_axis)
    
    
    for station in stations: 
        plots["fw_quality_" + station] = r.TH1F("fw_quality_" + station, "; quality; events", *quality_axis)
        plots["emu_quality_" + station] = r.TH1F("emu_quality_" + station, "; quality; events", *quality_axis)
        plots["fw_multiplicity_" + station] = r.TH1F("fw_multiplicity_" + station, "; Primitive multiplicity; events", *multiplicity_axis)
        plots["emu_multiplicity_" + station] = r.TH1F("emu_multiplicity_" + station, "; Primitive multiplicity; events", *multiplicity_axis)
        lists["mean_fw_quality_" + station] = []
        lists["mean_emu_quality_" + station] = []
        lists["mean_fw_multiplicity_" + station] = []
        lists["mean_emu_multiplicity_" + station] = []
        effplots["passed_" + station] = r.TH1F("hpassed_" + station, "; quality; events", *quality_axis)
        effplots["total_" + station] = r.TH1F("htotal_" + station, "; quality; events", *quality_axis)
        for category in categories:
            plots["pos_{}_{}".format(station, category)] = r.TH1F("hpos_{}_{}".format(station, category), "hpos; position (mm); events", *position_axis)
            plots["slope_{}_{}".format(station, category)] = r.TH1F("hslope_{}_{}".format(station, category), "hslope; slope; events", *slope_axis)
            plots["time_{}_{}".format(station, category)] = r.TH1F("htime_{}_{}".format(station, category), "htime; time (ns); events", *time_axis)
            plots["phi_{}_{}".format(station, category)] = r.TH1F("hphi_{}_{}".format(station, category), "hphi; phi; events", *phi_axis)
            plots["phib_{}_{}".format(station, category)] = r.TH1F("hphib_{}_{}".format(station, category), "hphib; phib; events", *phib_axis)

    for suffix in ([""] + ["_" + station for station in stations]):
        for s in ["fw", "emu"]:
            for plot_name in ["quality"]:
                plot = plots["{}_{}{}".format(s, plot_name, suffix)]
                for n_b, b in enumerate(quality_names):
                    plot.GetXaxis().SetBinLabel(n_b + 1, b)
   
    for q in ["Q9", "Q8", "Q6", "3h", "4h"]:
        for cat in ["passed", "total"]:
            effplots["{}_{}".format(q, cat)] = [] 
            for station in stations: 
                effplots["{}_{}_{}".format(q, cat, station)] = [] 

    return effplots, plots, lists


def get_categories(prim):
    station = "mb" + str(prim["station"])
    categories = ["all"]
    categories.append("correlated" if prim["quality"] > 4 else "uncorrelated")
    return station, categories


def readFile(File, tag, events_to_read=10, initialEvent = 0) :
    labels = {}
    labels['emulator'] = ['quality', 'position', 'slope', 'time', 'phi', 'phib', 'chi2', 'shift',
            'wheel', 'sector', 'station', 'superlayer', 
            'wi0', 'wi1','wi2', 'wi3', 'wi4', 'wi5', 'wi6', 'wi7', 
            'tdc0', 'tdc1','tdc2', 'tdc3', 'tdc4', 'tdc5', 'tdc6', 'tdc7', 
            'lat0', 'lat1','lat2', 'lat3', 'lat4', 'lat5', 'lat6', 'lat7', 
            'eventBX']  
    labels['firmware'] = ['wheel', 'sector', 'station', 'superlayer', 'quality',
            'position', 'slope', 'time', 'phi', 'phib', 
            'wi0', 'wi1','wi2', 'wi3', 
            'tdc0', 'tdc1','tdc2', 'tdc3',
            'lat0', 'lat1','lat2', 'lat3']

    labels['hits'] = ['wheel', 'sector', 'station', 'num']          

    prims = [{}]
    cons_zeros = 0
    cons_zero = False
    with open(os.path.expandvars(File), 'r') as f:
        evNumber = 0
        for i, l in enumerate(f):
            line = map(str2num,l.strip().split(' '))
            if len(line) == 1:
                evNumber += 1
            if evNumber < initialEvent: continue
            if evNumber >= events_to_read:
                 break
            if len(line) == 1:
                if cons_zero:
                    cons_zeros += 1
                cons_zero = True
                prims.append({})
                continue
            cons_zero = False
            line = dict(zip(labels[tag], map(str2num,l.strip().split(' '))))

            if (line['wheel'], line['sector'], line['station']) not in prims[-1] :
                prims[-1][(line['wheel'], line['sector'], line['station'])] = []
            prims[-1][(line['wheel'], line['sector'], line['station'])] += [line] 
    return prims, cons_zeros


def filter_firmware_prims(prims):
    filtered_prims = []
    for ev_prims in prims:
        filtered_prims.append({})
        for (wheel, sector, station) in ev_prims:
            ev_prims_ch = ev_prims[(wheel, sector, station)]
            filtered_prims[-1][(wheel, sector, station)] = []
            for i, prim in enumerate(ev_prims_ch):
                # sl1
                if i < len(ev_prims_ch) - 2:
                    if (prim['superlayer'] == 1 or prim['superlayer'] == 3) and (
                            ev_prims_ch[i+1]['superlayer'] == 0 or ev_prims_ch[i+2]['superlayer'] == 0):
                        continue
                elif i < len(ev_prims_ch) - 1:
                    if (prim['superlayer'] == 1 or prim['superlayer'] == 3) and (
                            ev_prims_ch[i+1]['superlayer'] == 0):
                        continue                
                if prim['superlayer'] == 0:
                    prim_sl1 = ev_prims_ch[i-2]
                    prim_sl3 = ev_prims_ch[i-1]
                    prim['tdc0'] = prim_sl1['tdc0']; prim['wi0'] = prim_sl1['wi0']; prim['lat0'] = prim_sl1['lat0'];
                    prim['tdc1'] = prim_sl1['tdc1']; prim['wi1'] = prim_sl1['wi1']; prim['lat1'] = prim_sl1['lat1'];
                    prim['tdc2'] = prim_sl1['tdc2']; prim['wi2'] = prim_sl1['wi2']; prim['lat2'] = prim_sl1['lat2'];
                    prim['tdc3'] = prim_sl1['tdc3']; prim['wi3'] = prim_sl1['wi3']; prim['lat3'] = prim_sl1['lat3'];
                    prim['tdc4'] = prim_sl3['tdc0']; prim['wi4'] = prim_sl3['wi0']; prim['lat4'] = prim_sl3['lat0'];
                    prim['tdc5'] = prim_sl3['tdc1']; prim['wi5'] = prim_sl3['wi1']; prim['lat5'] = prim_sl3['lat1'];
                    prim['tdc6'] = prim_sl3['tdc2']; prim['wi6'] = prim_sl3['wi2']; prim['lat6'] = prim_sl3['lat2'];
                    prim['tdc7'] = prim_sl3['tdc3']; prim['wi7'] = prim_sl3['wi3']; prim['lat7'] = prim_sl3['lat3'];
                elif prim['superlayer'] == 1:
                    prim['tdc4'] = -1; prim['wi4'] = -1; prim['lat4'] = -1;
                    prim['tdc5'] = -1; prim['wi5'] = -1; prim['lat5'] = -1;
                    prim['tdc6'] = -1; prim['wi6'] = -1; prim['lat6'] = -1;
                    prim['tdc7'] = -1; prim['wi7'] = -1; prim['lat7'] = -1;
                elif prim['superlayer'] == 3:
                    prim['tdc4'] = prim['tdc0']; prim['wi4'] = prim['wi0']; prim['lat4'] = prim['lat0'];
                    prim['tdc5'] = prim['tdc1']; prim['wi5'] = prim['wi1']; prim['lat5'] = prim['lat1'];
                    prim['tdc6'] = prim['tdc2']; prim['wi6'] = prim['wi2']; prim['lat6'] = prim['lat2'];
                    prim['tdc7'] = prim['tdc3']; prim['wi7'] = prim['wi3']; prim['lat7'] = prim['lat3'];
                    prim['tdc0'] = -1; prim['wi0'] = -1; prim['lat0'] = -1;
                    prim['tdc1'] = -1; prim['wi1'] = -1; prim['lat1'] = -1;
                    prim['tdc2'] = -1; prim['wi2'] = -1; prim['lat2'] = -1;
                    prim['tdc3'] = -1; prim['wi3'] = -1; prim['lat3'] = -1;
                else:  # sl2
                    continue
                filtered_prims[-1][(wheel, sector, station)].append(prim)
    return filtered_prims


def firmware_comparer(firmware_prims, emulator_prims, plots):
    for n_ev, ev in enumerate(firmware_prims):
        for (wheel, sector, station) in ev:
            if (wheel, sector, station) in emulator_prims[n_ev]:
                for prim in ev[(wheel, sector, station)]:
                    for emu_prim in emulator_prims[n_ev][(wheel, sector, station)]:
                        if same_hits(prim, emu_prim):
                            emu_position = 10 * (emu_prim["position"] - emu_prim["shift"])
                            emu_position -= offsets[(wheel, sector, station)] if emu_prim["superlayer"] == 3 else 0
                            my_station, categories = get_categories(emu_prim)
                            for cat in categories:
                                plots["pos_{}_{}".format(my_station, cat)].Fill(prim["position"] - emu_position)
                                plots["slope_{}_{}".format(my_station, cat)].Fill(prim["slope"] + emu_prim["slope"])
                                plots["time_{}_{}".format(my_station, cat)].Fill(prim["time"] - emu_prim["time"])
                                plots["phi_{}_{}".format(my_station, cat)].Fill(prim["phi"] - emu_prim["phi"])
                                plots["phib_{}_{}".format(my_station, cat)].Fill(prim["phib"] - emu_prim["phib"])


def emulator_comparer(firmware_prims, emulator_prims, plots, effplots, lists, bin_step = 50, debug=False):
    for n_ev, ev in enumerate(emulator_prims):
        for (wheel, sector, station) in ev:
            plots["emu_multiplicity_" + get_station(station)].Fill(len(ev[(wheel, sector, station)]))
            if (wheel, sector, station) in firmware_prims[n_ev]:
                plots["fw_multiplicity_" + get_station(station)].Fill(len(firmware_prims[n_ev][(wheel, sector, station)]))
                for emu_prim in ev[(wheel, sector, station)]:
                    quality_bin = get_quality_bin(emu_prim)
                    plots["emu_quality"].Fill(quality_bin)
                    plots["emu_quality_{}".format(get_station(station))].Fill(quality_bin)
                    found = False
                    for prim in firmware_prims[n_ev][(wheel, sector, station)]:
                        if same_hits(prim, emu_prim):
                            found = True
                            emu_position = 10 * (emu_prim["position"] - emu_prim["shift"])
                            emu_position -= offsets[(wheel, sector, station)] if emu_prim["superlayer"] == 3 else 0
                            my_station, categories = get_categories(emu_prim)
                            for cat in categories:
                                plots["pos_{}_{}".format(my_station, cat)].Fill(prim["position"] - emu_position)
                                plots["slope_{}_{}".format(my_station, cat)].Fill(prim["slope"] + emu_prim["slope"])
                                plots["time_{}_{}".format(my_station, cat)].Fill(prim["time"] - emu_prim["time"])
                                plots["phi_{}_{}".format(my_station, cat)].Fill(prim["phi"] - emu_prim["phi"])
                                plots["phib_{}_{}".format(my_station, cat)].Fill(prim["phib"] - emu_prim["phib"])
                            break
                    quality_bin = get_quality_bin(emu_prim)
                    if quality_bin == -1:
                        raise ValueError("Primitive {} with bizarre quality").format(emu_prim)
                    if found:
                        effplots["passed"].Fill(quality_bin)
                        effplots["passed_{}".format(get_station(station))].Fill(quality_bin)

                    if quality_bin == 4 and debug:
                    #elif quality_bin == 4 and debug:
                        print "evNumber =",n_ev, 
                        if not found: continue
                        print "GOOD ONE!" if found else ""
                        #for emu_prim_bis in ev[(wheel, sector, station)]:
                        #    print emu_prim_bis["quality"],
                        print "****emu******"
                        print emu_prim["time"]
                        for i in range(8):
                            print "wi{0}: {1} tdc{0}:{2} lat{0}:{3}".format(i, 
                                emu_prim["wi"+str(i)], emu_prim["tdc"+str(i)], emu_prim["lat"+str(i)])
                        for prim in firmware_prims[n_ev][(wheel, sector, station)]:
                            if True or prim["quality"] == 9:
                                print "****fw******"
                                for i in range(8):
                                    print "wi{0}: {1} tdc{0}:{2} lat{0}:{3}".format(i, prim["wi"+str(i)], prim["tdc"+str(i)], prim["lat"+str(i)])
                        print ""
                        x = raw_input("Continue?")
                        if x == "":
                            print "Next!"
                    effplots["total"].Fill(quality_bin)
                    effplots["total_{}".format(get_station(station))].Fill(quality_bin)
                for prim in firmware_prims[n_ev][(wheel, sector, station)]:
                    quality_bin = get_quality_bin(prim)
                    plots["fw_quality"].Fill(quality_bin)
                    plots["fw_quality_{}".format(get_station(station))].Fill(quality_bin)
        if n_ev % bin_step == 0:
            lists["mean_fw_quality"].append(plots["fw_quality"].GetMean())
            lists["mean_emu_quality"].append(plots["emu_quality"].GetMean())
            for station in stations:
                lists["mean_fw_quality_" + station].append(plots["fw_quality_" + station].GetMean())
                lists["mean_emu_quality_" + station].append(plots["emu_quality_" + station].GetMean())
                lists["mean_fw_multiplicity_" + station].append(plots["fw_multiplicity_" + station].GetMean())
                lists["mean_emu_multiplicity_" + station].append(plots["emu_multiplicity_" + station].GetMean())
            for i_q, q in enumerate(["3h", "4h", "Q6", "Q8", "Q9"]):
                for cat in ["passed", "total"]:
                    effplots["{}_{}".format(q, cat)].append(effplots[cat].GetBinContent(i_q + 1))
                    for station in stations:
                       effplots["{}_{}_{}".format(q, cat, station)].append(effplots["{}_{}".format(cat, station)].GetBinContent(i_q + 1))


def main():
    import time
    options = parse_args()

    fw_file = "/eos/user/j/jleonhol/ntuplesST/run{}_fw.txt".format(options.runNumber) 
    emu_file = "/eos/user/j/jleonhol/ntuplesST/run{}_emu.txt".format(options.runNumber) 

    n_events = count_events(fw_file) if not options.nEvents else options.nEvents

    effplots, plots, lists = make_plots(n_events, options)
    
    step = max(10000, options.binStep); # n_events = 1000
    step = n_events
    end_fw = -2
    end_emu = -2
    for i in range(0, n_events, step):
        print str(i) + "/" + str(n_events),
        end = i + step if i + step < n_events else n_events
        t0 = time.time()
        firmware_prims_unfiltered, fw_zeros = readFile(fw_file,
                "firmware", end - i, i)
        t1 = time.time()
        firmware_prims = filter_firmware_prims(firmware_prims_unfiltered)
        t2 = time.time()
        emulator_prims, emu_zeros = readFile(emu_file, "emulator", end - i, i)
        t3 = time.time()
        emulator_comparer(firmware_prims, emulator_prims, plots, effplots, lists, options.binStep, options.debug)
        t4 = time.time()

        missing_time = round( float(n_events - i) / step) * (t4 - t0) 

        print "Reading fw: {:.4f}s| Filtering fw: {:.4f}s| Reading emu: {:.4f}s| Comparing prims:{:.4f}s |||| Missing time:{:.1f}".format(t1-t0, t2-t1, t3-t2, t4-t3, missing_time)
        #firmware_comparer(firmware_prims, emulator_prims, plots)
        del firmware_prims_unfiltered, firmware_prims, emulator_prims

    folder_name = "plots_same_hits/"
    if not os.path.isdir(folder_name):
        os.mkdir(folder_name)

    
    bin_nums = n_events / options.binStep
    event_axis = (bin_nums + 1, 0 - n_events/ (2 * bin_nums), n_events + n_events/ (2 * bin_nums))
    
    suffixes = [""] + ["_" + station for station in stations]

    for variable in ["quality", "multiplicity"]:
        for suffix in suffixes:
            if suffix == "" and variable == "multiplicity": continue
            for s in ["fw", "emu"]:
                plots["mean_{}_{}{}".format(s, variable, suffix)] = r.TH1F("mean_{}_{}{}".format(s, variable, suffix), "; event; mean " + variable, *event_axis)
                plot = plots["mean_{}_{}{}".format(s, variable, suffix)]
                plot.GetYaxis().SetRangeUser(0, 5) 
            for i_bin, b in enumerate(lists["mean_fw_{}{}".format(variable, suffix)]):
                plots["mean_fw_{}{}".format(variable, suffix)].SetBinContent(i_bin + 1, b)
                plots["mean_emu_{}{}".format(variable, suffix)].SetBinContent(i_bin + 1, lists["mean_emu_{}{}".format(variable, suffix)][i_bin])
                    
    for stuff in plots:
        c   = r.TCanvas("c", "c", 800, 800)
        plots[stuff].SetTitleSize(0, "y");
        plots[stuff].Draw()
        labels = get_labels()
        for label in labels:
            label.Draw("same")
        c.SaveAs("{}{}.png".format(folder_name, stuff))
        del c
   
    def save_eff_plot(suffix):
        c   = r.TCanvas("c", "c", 800, 800)
        for n_b in range(1, effplots["total" + suffix].GetNbinsX() + 1):
            perc = float(effplots["passed" + suffix].GetBinContent(n_b)) / float(effplots["total" + suffix].GetBinContent(n_b))
            new_bin_label = "{} ({:.1f}%)".format(effplots["total" + suffix].GetXaxis().GetBinLabel(n_b), 100*perc)
            effplots["total" + suffix].GetXaxis().SetBinLabel(n_b, new_bin_label)

        leg = r.TLegend(0.8, 0.6, 0.95, 0.75)
        leg.AddEntry(effplots["total" + suffix], "Different fit", "PL")
        leg.AddEntry(effplots["passed" + suffix], "Same fit", "PL")

        r.gStyle.SetOptStat(0)
        effplots["total" + suffix].SetFillColor(r.kRed)
        effplots["passed" + suffix].SetFillColor(r.kGreen)
        effplots["total" + suffix].SetLineColor(r.kRed)
        effplots["passed" + suffix].SetLineColor(r.kGreen)
        effplots["total" + suffix].SetMarkerColor(r.kRed)
        effplots["passed" + suffix].SetMarkerColor(r.kGreen)
        effplots["total" + suffix].SetTitleSize(0, "y");
        effplots["total" + suffix].Draw()
        effplots["passed" + suffix].Draw("same")
        leg.Draw("same")
        labels = get_labels()
        for label in labels:
            label.Draw("same")
        c.SaveAs("{}eff{}.png".format(folder_name, suffix))
        del c

        for i_q, q in enumerate(["3h", "4h", "Q6", "Q8", "Q9"]):
            r.TGaxis.SetMaxDigits(4)
            c   = r.TCanvas("c", "c", 800, 800)
            h_passed = r.TH1F("q_passed", "; Event; Efficiency", *event_axis)
            h_total = r.TH1F("q_total", "; Event; Efficiency", *event_axis)
            for ibin, b in enumerate(effplots[q + "_passed" + suffix]):
                h_passed.SetBinContent(ibin + 1, b)
                h_total.SetBinContent(ibin + 1, effplots[q + "_total" + suffix][ibin])
            eff = r.TEfficiency(h_passed, h_total)
            eff.Draw()
            r.gPad.Update()
            graph = eff.GetPaintedGraph().GetYaxis().SetRangeUser(0, 1)
            labels = get_labels()
            for label in labels:
                label.Draw("same")
            c.SaveAs("{}acceff{}_{}.png".format(folder_name, suffix, q))
            del c, h_passed, h_total, eff

            c   = r.TCanvas("c", "c", 800, 800)
            h_passed = r.TH1F("q_passed", "; Event; Efficiency", *event_axis)
            h_total = r.TH1F("q_total", "; Event; Efficiency", *event_axis)
            for ibin, b in enumerate(effplots[q + "_passed" + suffix]):
                if ibin == 0: 
                    h_passed.SetBinContent(1, 0)
                    h_total.SetBinContent(1, 0)
                else:
                    h_passed.SetBinContent(ibin + 1, effplots[q + "_passed" + suffix][ibin] - effplots[q + "_passed" + suffix][ibin - 1])
                    h_total.SetBinContent(ibin + 1, effplots[q + "_total" + suffix][ibin] - effplots[q + "_total" + suffix][ibin - 1])
            eff = r.TEfficiency(h_passed, h_total)
            eff.Draw()
            r.gPad.Update()
            graph = eff.GetPaintedGraph().GetYaxis().SetRangeUser(0, 1)
            labels = get_labels()
            for label in labels:
                label.Draw("same")
            c.SaveAs("{}eff{}_{}.png".format(folder_name, suffix, q))
            del c, h_passed, h_total, eff



    for suffix in suffixes:
        save_eff_plot(suffix)

    
    if options.copy:
        from shutil import copyfile as copy
        www_path = "/eos/user/j/jleonhol/www/SliceTest/"
        index_path = "/eos/user/j/jleonhol/backup/"
        output_folder = folder_name.split("/")[0]

        if not os.path.isdir("{}run{}".format(www_path, options.runNumber)):
            os.mkdir("{}run{}".format(www_path, options.runNumber))
            copy(index_path + "index_withFolders_php", "{}run{}/index.php".format(www_path, options.runNumber))
       
        suffix = output_folder if not options.nEvents else "{}_{}".format(output_folder, options.nEvents)
        if not os.path.isdir("{}run{}/{}/".format(www_path, options.runNumber, suffix)):
            os.mkdir("{}run{}/{}".format(www_path, options.runNumber, suffix))
            copy(index_path + "index_noFolders_php", "{}run{}/{}/index.php".format(www_path, options.runNumber, suffix))

        list_of_files = os.listdir(folder_name)
        for f in list_of_files:
            if os.path.isfile(folder_name + f):
                print "Copying", f
                os.system("cp " + folder_name + f + " {}run{}/{}/{}".format(www_path, options.runNumber, suffix, f))



if __name__ == "__main__":
    main()
