/*********************************************************************
Component Organizer
Copyright (C) Mário Ribeiro (mario.ribas@gmail.com)

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

#include "manufacturer.h"

#include <QStringList>

const QStringList Manufacturer::m_defaultNames =
    (QStringList()
     << "Altera"
     << "Analog Devices"
     << "Atmel"
     << "Avago Technologies"
     << "Burr-Brown"
     << "Fairchild Semiconductor"
     << "Freescale"
     << "Infineon"
     << "Intersil"
     << "Lattice Semiconductor"
     << "Linear Technology"
     << "Maxim"
     << "National Semiconductor"
     << "NXP"
     << "ON Semiconductor"
     << "Philips"
     << "Microchip"
     << "Texas Instruments"
     << "Roving Networks"
     << "TE Connectivity"
     << "ST Microelectronics"
     << "Vishay"
     << "Xilinx"
    );

QStringList Manufacturer::defaultNames()
{
    return m_defaultNames;
}

Manufacturer::Manufacturer(const QString &name, QObject *parent) :
    QObject(parent),
    m_name(name)
{
}


