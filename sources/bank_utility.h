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
        return qHashBits(&x, sizeof(FmBank::Instrument));
    }
};

}

inline bool operator==(const FmBank::Instrument &a, const FmBank::Instrument &b)
{
    return !memcmp(&a, &b, sizeof(FmBank::Instrument));
}
