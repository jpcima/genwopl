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
#include <QHash>
#include <string.h>

inline FmBank::Instrument clean_instrument(const FmBank::Instrument &x)
{
    FmBank::Instrument clean(x);
    memset(clean.name, 0, sizeof(FmBank::Instrument::name));
    clean.is_blank = false;
    clean.ms_sound_kon = 0;
    clean.ms_sound_koff = 0;
    return x;
}

struct measurer_cache_hash {
    size_t operator()(const FmBank::Instrument &x) const
    {
        FmBank::Instrument clean_x = clean_instrument(x);
        return qHashBits(&clean_x, sizeof(FmBank::Instrument));
    }
};

struct measurer_cache_equal {
    bool operator()(const FmBank::Instrument &a, const FmBank::Instrument &b) const
    {
        FmBank::Instrument clean_a = clean_instrument(a);
        FmBank::Instrument clean_b = clean_instrument(b);
        return !memcmp(&clean_a, &clean_b, sizeof(FmBank::Instrument));
    }
};
