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

namespace std {

template <class T> struct hash;

template <> struct hash<FmBank::Instrument> {
    size_t operator()(const FmBank::Instrument &x) const {
        FmBank::Instrument noname_x(x);
        memset(noname_x.name, 0, sizeof(FmBank::Instrument::name));
        noname_x.ms_sound_kon = 0;
        noname_x.ms_sound_koff = 0;
        return qHashBits(&noname_x, sizeof(FmBank::Instrument));
    }
};

}

inline bool operator==(const FmBank::Instrument &a, const FmBank::Instrument &b)
{
    FmBank::Instrument noname_a(a);
    FmBank::Instrument noname_b(b);
    memset(noname_a.name, 0, sizeof(FmBank::Instrument::name));
    memset(noname_b.name, 0, sizeof(FmBank::Instrument::name));
    noname_a.ms_sound_kon = 0;
    noname_a.ms_sound_koff = 0;
    noname_b.ms_sound_kon = 0;
    noname_b.ms_sound_koff = 0;
    return !memcmp(&noname_a, &noname_b, sizeof(FmBank::Instrument));
}
