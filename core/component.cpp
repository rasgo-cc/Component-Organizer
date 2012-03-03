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

#include "component.h"
#include "datasheet.h"
#include "stock.h"
#include "package.h"
#include "label.h"

#include <QDebug>

int Component::nextID = 0;

Component::Component(const QString name, QObject *parent) :
    QObject(parent),
    m_ID(Component::nextID++),
    m_name(name),
    m_defaultDatasheetIndex(-1),
    m_ignoreStock(true),
    m_lowStock(0),
    m_totalStock(0),
    m_container(0),
    m_primaryLabel(0),
    m_secondaryLabel(0),
    m_linkedTo(0)
{    

}

void Component::addDatasheet(Datasheet *datasheet)
{
    m_datasheets.append(datasheet);
}

void Component::removeDatasheet(Datasheet *datasheet)
{
    if(m_datasheets.at(m_defaultDatasheetIndex) == datasheet)
        m_defaultDatasheetIndex = -1;
    m_datasheets.removeOne(datasheet);
    delete datasheet;
}

bool Component::setDefaultDatasheet(Datasheet *datasheet)
{
    m_defaultDatasheetIndex = m_datasheets.indexOf(datasheet);

    if(m_defaultDatasheetIndex >= 0)
        return true;
    else
        return false;
}

bool Component::setDefaultDatasheetIndex(int index)
{
    if(index >= 0 && index < m_datasheets.count())
    {
        m_defaultDatasheetIndex = index;
        return true;
    }
    else
        return false;
}

Datasheet* Component::defaultDatasheet()
{
    if(m_defaultDatasheetIndex >= 0)
        return m_datasheets.at(m_defaultDatasheetIndex);
    else
        return 0;
}

Datasheet* Component::datasheet(const QString &path)
{
    foreach(Datasheet* d, m_datasheets)
        if(d->path().compare(path) == 0)
            return d;

    return 0;
}

void Component::addStock(Stock *stock)
{
    m_stocks.append(stock);
    m_totalStock += stock->stock();
}

void Component::removeStock(const QString &packageName)
{
    Stock *s = stock(packageName);
    m_totalStock -= s->stock();
    m_stocks.removeOne(s);
    delete s;
}

QList<Stock *> Component::stocks()
{
    return m_stocks;
}

Stock* Component::stock(const QString &packageName)
{
    foreach(Stock *s, m_stocks)
        if(s->package()->name().compare(packageName) == 0)
            return s;

    return 0;
}

void Component::setLabel(int level, Label *label)
{
    switch(level)
    {
    case 0: m_primaryLabel = label; break;
    case 1: m_secondaryLabel = label; break;
    default: ;
    }
}
