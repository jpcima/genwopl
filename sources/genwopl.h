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

#include "bank.h"
#include <string>
#include <vector>

struct IniBank
{
    std::string bank_name;
    std::string filepath;
    std::string filepath_d;
    std::string prefix;
    std::string prefix_d;
    std::string filter_m;
    std::string filter_p;
    std::string format;
};

struct IniBanks
{
    std::vector<IniBank> banks;
};

IniBanks *load_ini(const char *ini_filename);
bool load_banks(const IniBanks &ini, const std::string &basedir, std::vector<FmBank> &banks);
bool load_bank(const IniBank &ib, const std::string &basedir, FmBank &bank);
