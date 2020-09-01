import os, sys
from sl3offsets import sl3offsets as offsets
import ROOT as r
r.gROOT.SetBatch(True)
r.gStyle.SetOptStat(111111)

quality_names = ["3h", "4h", "Q6", "Q8", "Q9"]


def parse_args():
    import argparse
    parser = argparse.ArgumentParser(description='Some options')
    parser.add_argument('-r', '--runNumber', type=int, default = 335947)
    parser.add_argument('-s', '--binStep', type=int, default = 50)
    parser.add_argument('-d', '--debug', type=bool, default = False)
    parser.add_argument('-n', '--nEvents', type=int, default = None)
    return parser.parse_args()


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
            if tdc_fw != tdc_emu or lat_fw != lat_emu or wi_fw != wi_emu:
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
    
    plots = {}
    stations = ["mb1", "mb2", "mb3", "mb4"]
    categories = ["all", "correlated", "uncorrelated"]
    for station in stations: 
        for category in categories:
            plots["pos_{}_{}".format(station, category)] = r.TH1F("hpos_{}_{}".format(station, category), "hpos; position (mm); events", *position_axis)
            plots["slope_{}_{}".format(station, category)] = r.TH1F("hslope_{}_{}".format(station, category), "hslope; slope; events", *slope_axis)
            plots["time_{}_{}".format(station, category)] = r.TH1F("htime_{}_{}".format(station, category), "htime; time (ns); events", *time_axis)
            plots["phi_{}_{}".format(station, category)] = r.TH1F("hphi_{}_{}".format(station, category), "hphi; phi; events", *phi_axis)
            plots["phib_{}_{}".format(station, category)] = r.TH1F("hphib_{}_{}".format(station, category), "hphib; phib; events", *phib_axis)
   
    
    quality_axis = (5, -0.5, 4.5)
    
    plots["fw_quality"] = r.TH1F("fw_quality", "; quality; events", *quality_axis)
    plots["emu_quality"] = r.TH1F("emu_quality", "; quality; events", *quality_axis)
    
    lists = {}
    lists["mean_fw_quality"] = []
    lists["mean_emu_quality"] = []

    effplots = {}
    effplots["passed"] = r.TH1F("hpassed", "; quality; events", *quality_axis)
    effplots["total"] = r.TH1F("htotal", "; quality; events", *quality_axis)

    for plot in effplots.values():
        for n_b, b in enumerate(quality_names):
            plot.GetXaxis().SetBinLabel(n_b + 1, b)
   
    for q in ["Q9", "Q8", "Q6", "3h", "4h"]:
        for cat in ["passed", "total"]:
            effplots["{}_{}".format(q, cat)] = [] 

    return effplots, plots, lists


def get_categories(prim):
    station = "mb" + str(prim["station"])
    categories = ["all"]
    categories.append("correlated" if prim["quality"] > 4 else "uncorrelated")
    return station, categories


def readFile(File, tag, events=[]) :
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
    with open(os.path.expandvars(File), 'r') as f:
        evNumber = 0
        for l in f:
            line = map(str2num,l.strip().split(' '))
            if len(line) == 1:
                evNumber += 1
            if evNumber not in events and len(events)!=0: 
                if evNumber > max(events):
                    break
                continue
            if len(line) == 1:
                prims.append({})
                continue
            line = dict(zip(labels[tag], map(str2num,l.strip().split(' '))))

            if (line['wheel'], line['sector'], line['station']) not in prims[-1] :
                prims[-1][(line['wheel'], line['sector'], line['station'])] = []
            prims[-1][(line['wheel'], line['sector'], line['station'])] += [line] 
    if not prims[0]:
        prims.pop(0)
    return prims


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
            if (wheel, sector, station) in firmware_prims[n_ev]:
                for emu_prim in ev[(wheel, sector, station)]:
                    quality_bin = get_quality_bin(emu_prim)
                    plots["emu_quality"].Fill(quality_bin)
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
                    elif quality_bin == 4 and debug:
                        for emu_prim_bis in ev[(wheel, sector, station)]:
                            print emu_prim_bis["quality"],
                        print ""
                        for prim in firmware_prims[n_ev][(wheel, sector, station)]:
                            print prim["quality"],
                        print ""
                        x = raw_input("Continue?")
                        if x == "":
                            print "Next!"
                    effplots["total"].Fill(quality_bin)
                for prim in firmware_prims[n_ev][(wheel, sector, station)]:
                    quality_bin = get_quality_bin(prim)
                    plots["fw_quality"].Fill(quality_bin)
        if n_ev % bin_step == 0:
            lists["mean_fw_quality"].append(plots["fw_quality"].GetMean())
            lists["mean_emu_quality"].append(plots["emu_quality"].GetMean())
            for i_q, q in enumerate(["3h", "4h", "Q6", "Q8", "Q9"]):
                for cat in ["passed", "total"]:
                    effplots["{}_{}".format(q, cat)].append(effplots[cat].GetBinContent(i_q + 1))


def main():
    options = parse_args()

    fw_file = "/eos/user/j/jleonhol/ntuplesST/run{}_fw.txt".format(options.runNumber) 
    emu_file = "./run{}_emu.txt".format(options.runNumber)

    n_events = count_events(fw_file) if not options.nEvents else options.nEvents

    effplots, plots, lists = make_plots(n_events, options)
    
    step = max(100, options.binStep); # n_events = 1000
    for i in range(0, n_events, step):
        sys.stdout.flush()
        print str(i) + "/" + str(n_events)
        end = i + step if i + step < n_events else n_events
        firmware_prims_unfiltered = readFile(fw_file,
                "firmware", [a for a in range(i, end)])
        firmware_prims = filter_firmware_prims(firmware_prims_unfiltered)
        emulator_prims = readFile(emu_file, "emulator", [a for a in range(i, end)])
        emulator_comparer(firmware_prims, emulator_prims, plots, effplots, lists, options.binStep, options.debug)
        #firmware_comparer(firmware_prims, emulator_prims, plots)
        del firmware_prims_unfiltered, firmware_prims, emulator_prims

    folder_name = "plots_same_hits/"
    if not os.path.isdir(folder_name):
        os.mkdir(folder_name)

    
    bin_nums = n_events / options.binStep
    eff_axis = (bin_nums + 1, 0 - n_events/ (2 * bin_nums), n_events + n_events/ (2 * bin_nums))
    
    plots["mean_fw_quality"] = r.TH1F("mean_fw_quality", "; event; mean quality", *eff_axis)
    plots["mean_emu_quality"] = r.TH1F("mean_emu_quality", "; event; mean quality", *eff_axis)

    for plot_name in ["mean_fw_quality", "mean_emu_quality"]:
        plot = plots[plot_name]
        for n_b, b in enumerate(quality_names):
            plot.GetXaxis().SetBinLabel(n_b + 1, b)
    
    for i_bin, b in enumerate(lists["mean_fw_quality"]):
        plots["mean_fw_quality"].SetBinContent(i_bin + 1, b)
        plots["mean_emu_quality"].SetBinContent(i_bin + 1, lists["mean_emu_quality"][i_bin])
    
    for stuff in plots:
        c   = r.TCanvas("c", "c", 800, 800)
        plots[stuff].Draw()
        c.SaveAs("{}{}.png".format(folder_name, stuff))
        del c
    
    c   = r.TCanvas("c", "c", 800, 800)
    for n_b in range(1, effplots["total"].GetNbinsX() + 1):
        perc = float(effplots["passed"].GetBinContent(n_b)) / float(effplots["total"].GetBinContent(n_b))
        new_bin_label = "{} ({:.1f}%)".format(effplots["total"].GetXaxis().GetBinLabel(n_b), 100*perc)
        effplots["total"].GetXaxis().SetBinLabel(n_b, new_bin_label)

    leg = r.TLegend(0.8, 0.8, 0.95, 0.95)
    leg.AddEntry(effplots["total"], "Different fit", "PL")
    leg.AddEntry(effplots["passed"], "Same fit", "PL")

    r.gStyle.SetOptStat(0)
    effplots["total"].SetFillColor(r.kRed)
    effplots["passed"].SetFillColor(r.kGreen)
    effplots["total"].SetLineColor(r.kRed)
    effplots["passed"].SetLineColor(r.kGreen)
    effplots["total"].SetMarkerColor(r.kRed)
    effplots["passed"].SetMarkerColor(r.kGreen)
    effplots["total"].Draw()
    effplots["passed"].Draw("same")
    leg.Draw("same")
    c.SaveAs("{}eff.png".format(folder_name))
    del c

    for i_q, q in enumerate(["3h", "4h", "Q6", "Q8", "Q9"]):
        c   = r.TCanvas("c", "c", 800, 800)
        print eff_axis[0], len(effplots[q + "_passed"])
        assert(bin_nums == len(effplots[q + "_passed"]))
        h_passed = r.TH1F("q_passed", "; Event; Efficiency", *eff_axis)
        h_total = r.TH1F("q_total", "; Event; Efficiency", *eff_axis)
        for ibin, b in enumerate(effplots[q + "_passed"]):
            h_passed.SetBinContent(ibin + 1, b)
            h_total.SetBinContent(ibin + 1, effplots[q + "_total"][ibin])
        eff = r.TEfficiency(h_passed, h_total)
        eff.Draw()
        c.SaveAs("{}eff_{}.png".format(folder_name, q))
        del c, h_passed, h_total, eff

if __name__ == "__main__":
    main()
