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

#ifndef STOCK_H
#define STOCK_H

#include <QObject>

class Package;

class Stock : public QObject
{
    Q_OBJECT
public:
    explicit Stock(Package *package, QObject *parent = 0);

    Package* package() { return m_package; }

    void setStock(int stock) { m_stock = stock; }
    int stock() { return m_stock; }

    void setLowValue(int low) { m_lowValue = low; }
    int lowValue() { return m_lowValue; }
    
signals:
    
public slots:

private:
    Package *m_package;
    int m_stock;
    int m_lowValue;
    
};

#endif // STOCK_H
