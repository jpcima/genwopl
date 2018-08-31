/*
 * GenWOPL - a converter of OPL3 bank collections to WOPL files
 * Copyright (c) 2018 Jean Pierre Cimalando <jp-dev@inbox.ru>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "genwopl.h"
#include "ini/ini_processing.h"
#include "FileFormats/format_adlib_bnk.h"
#include "FileFormats/format_ail2_gtl.h"
#include "FileFormats/format_apogeetmb.h"
#include "FileFormats/format_bisqwit.h"
#include "FileFormats/format_dmxopl2.h"
#include "FileFormats/format_junlevizion.h"
#include "FileFormats/format_sb_ibk.h"
#include "FileFormats/format_wohlstand_opl3.h"
#include <memory>
#include <getopt.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
    for (int c; (c = getopt(argc, argv, "")) != -1;) {
        switch (c) {
            /* argument handling */
        default:
            return 1;
        }
    }

    if (argc - optind != 1) {
        fprintf(stderr, "Bad number of arguments.\n");
        return 1;
    }

    std::string ini_filename = argv[optind];
    std::string ini_basedir;
    {
        size_t pos = ini_filename.rfind('/');
        if (pos == ini_filename.npos)
            ini_basedir.assign(".");
        else
            ini_basedir.assign(ini_filename.data(), pos);
    }

    std::unique_ptr<IniBanks> ini(load_ini(ini_filename.c_str()));
    if (!ini) {
        fprintf(stderr, "Cannot load the INI file '%s'\n", ini_filename.c_str());
        return 1;
    }

    std::vector<FmBank> banks;
    if (!load_banks(*ini, ini_basedir, banks)) {
        fprintf(stderr, "Cannot load the banks.\n");
        return 1;
    }

    
    
    

    return 0;
}

IniBanks *load_ini(const char *ini_filename)
{
    std::unique_ptr<IniBanks> iniBanks(new IniBanks);

    IniProcessing ini;
    if (!ini.open(ini_filename)) {
        fprintf(stderr, "Can't open banks.ini!\n");
        return nullptr;
    }

    uint32_t banks_count;
    ini.beginGroup("General");
    ini.read("banks", banks_count, 0);
    ini.endGroup();

    if (!banks_count) {
        fprintf(stderr, "Zero count of banks found in banks.ini!\n");
        return nullptr;
    }

    iniBanks->banks.resize(banks_count);

    for (uint32_t bank = 0; bank < banks_count; bank++) {
        if (!ini.beginGroup(std::string("bank-") + std::to_string(bank))) {
            fprintf(stderr, "Failed to find bank %u!\n", bank);
            return nullptr;
        }

        IniBank &current = iniBanks->banks[bank];

        ini.read("name", current.bank_name, "Untitled");
        ini.read("format", current.format, "Unknown");
        ini.read("file", current.filepath, "");
        ini.read("file-p", current.filepath_d, "");
        ini.read("prefix", current.prefix, "");
        ini.read("prefix-p", current.prefix_d, "");
        ini.read("filter-m", current.filter_m, "");
        ini.read("filter-p", current.filter_p, "");

        if (current.filepath.empty()) {
            fprintf(stderr, "Failed to load bank %u, file is empty!\n", bank);
            return nullptr;
        }

        // fprintf(stderr, "Loading %s...\n", current.filepath.c_str());

        ini.endGroup();
    }

    fprintf(stderr, "Have %u banks!\n", banks_count);
    fflush(stdout);

    return iniBanks.release();
}

bool load_banks(const IniBanks &ini, const std::string &basedir, std::vector<FmBank> &banks)
{
    uint32_t banks_count = ini.banks.size();
    banks.resize(banks_count);

    for (uint32_t bank = 0; bank < banks_count; bank++) {
        FmBank &current = banks[bank];
        if (!load_bank(ini.banks[bank], basedir, current))
            return false;
    }

    return true;
}

bool load_bank(const IniBank &ib, const std::string &basedir, FmBank &bank)
{
    bank.reset();

    const std::string &filepath = ib.filepath;
    const std::string &filepath_d = ib.filepath_d;
    const std::string &prefix = ib.prefix;
    const std::string &prefix_d = ib.prefix_d;
    const std::string &filter_m = ib.filter_m;
    const std::string &filter_p = ib.filter_p;
    const std::string &format = ib.format;
    // TODO prefix

    std::string fullpath;
    std::string fullpath_d;
    if (!filepath.empty())
        fullpath = basedir + '/' + filepath;
    if (!filepath_d.empty())
        fullpath_d = basedir + '/' + filepath_d;

    fprintf(stderr, "Loading %s...\n", filepath.c_str());

    bool load_success = false;
    bool fmt_unknown = false;

    if (format == "AIL") {
        AIL_GTL fmt;
        load_success = fmt.loadFile(fullpath.c_str(), bank) == FfmtErrCode::ERR_OK;
    }
    else if (format == "Bisqwit") {
        BisqwitBank fmt;
        load_success = fmt.loadFile(fullpath.c_str(), bank) == FfmtErrCode::ERR_OK;
    }
    else if (format == "WOPL") {
        WohlstandOPL3 fmt;
        load_success = fmt.loadFile(fullpath.c_str(), bank) == FfmtErrCode::ERR_OK;
    }
    else if (format == "OP2") {
        DmxOPL2 fmt;
        load_success = fmt.loadFile(fullpath.c_str(), bank) == FfmtErrCode::ERR_OK;
    }
    // else if (format == "EA") {
    // }
    else if (format == "TMB") {
        ApogeeTMB fmt;
        load_success = fmt.loadFile(fullpath.c_str(), bank) == FfmtErrCode::ERR_OK;
    }
    else if (format == "Junglevision") {
        JunleVizion fmt;
        load_success = fmt.loadFile(fullpath.c_str(), bank) == FfmtErrCode::ERR_OK;
    }
    // else if (format == "AdLibGold") {
    // }
    else if (format == "HMI") {
        AdLibAndHmiBnk_reader fmt;
        load_success = fmt.loadFile(fullpath.c_str(), bank) == FfmtErrCode::ERR_OK;
        if (load_success && !fullpath_d.empty()) {
            FmBank bankP;
            load_success = fmt.loadFile(fullpath_d.c_str(), bankP) == FfmtErrCode::ERR_OK;
            for (unsigned i = 0; i < 128; ++i)
                bank.Ins_Percussion[i] = bankP.Ins_Percussion[i];
        }
    }
    else if (format == "IBK") {
        SbIBK_DOS fmt;
        load_success = fmt.loadFile(fullpath.c_str(), bank) == FfmtErrCode::ERR_OK;
        if (load_success && !fullpath_d.empty()) {
            FmBank bankP;
            load_success = fmt.loadFile(fullpath_d.c_str(), bankP) == FfmtErrCode::ERR_OK;
            for (unsigned i = 0; i < 128; ++i)
                bank.Ins_Percussion[i] = bankP.Ins_Percussion[i];
        }
    }
    else
    {
        fmt_unknown = true;
    }

    if (!load_success) {
        if (fmt_unknown)
            fprintf(stderr, "Failed to load bank '%s'!\nUnknown format type '%s'\n",
                    filepath.c_str(), format.c_str());
        else
            fprintf(stderr, "Failed to load bank '%s'!\n", filepath.c_str());
        return false;
    }

    if (false) {
        FmBank::Instrument emptyIns = FmBank::emptyInst();
        unsigned n_melo = bank.Banks_Melodic.size();
        unsigned n_drum = bank.Banks_Percussion.size();
        for (unsigned b_i = 0; b_i < n_melo + n_drum; ++b_i) {
            const FmBank::MidiBank &mb = (b_i < n_melo) ?
                bank.Banks_Melodic[b_i] :
                bank.Banks_Percussion[b_i - n_melo];
            for (unsigned p_i = 0; p_i < 128; ++p_i) {
                const FmBank::Instrument &ins = (b_i < n_melo) ?
                    bank.Ins_Melodic[p_i + 128 * b_i] :
                    bank.Ins_Percussion[p_i + 128 * (b_i - n_melo)];
                bool empty = ins.is_blank || !memcmp(&ins, &emptyIns, sizeof(FmBank::Instrument));
                if (!empty)
                    fprintf(stderr, "-- %c%03u:%03u:%03u %s\n",
                            (b_i < n_melo) ? 'M' : 'P' , mb.msb, mb.lsb, p_i,
                            *ins.name ? ins.name : "(no name)");
            }
        }
    }

    return true;
}
