/*********************************************************************
Component Organizer
Copyright (C) M�rio Ribeiro (mario.ribas@gmail.com)

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
**********************************************************************/

#include "package.h"

#include <QStringList>

const QStringList Package::m_defaultNames =  //TODO sort the list
    (QStringList()
     << "TH"
     << "1/8W"
     << "1/4W"
     << "1/2W"
     << "1W"
     << "DO"
     << "PDIP"
     << "TO"
     << "SMD"
     << "SOIC"
     << "DPAK"
     << "SOT"
     << "SSOP"
     << "TSOP"
     << "µMAX"
     << "TQFP"
     << "DFN"
     << "QFN"
     << "MLF"
     << "PLCC"
     << "LCC"
     << "LGA"
     << "BGA"
     << "SM1"
     << "0402"
     << "0603"
     << "0805"
     << "1008"
     << "1206"
     << "1210"
     << "1812"
     << "2010"
     << "2512"
     
    );

QStringList Package::defaultNames()
{
    return m_defaultNames;
}

Package::Package(const QString &name, QObject *parent) :
    QObject(parent),
    m_name(name)
{
}
