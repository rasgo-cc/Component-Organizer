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

#include "label.h"

#include <QDebug>

Label::Label(const QString &name, Label *top, QList<Label*> leafs, QObject *parent) :
    QObject(parent),
    m_name(name),
    m_top(top)
{
}

void Label::removeLeaf(const QString &name)
{
    if(!m_leafs.isEmpty())
    {
        foreach(Label *leaf, m_leafs)
        {
            if(leaf->name().compare(name) == 0)
                m_leafs.removeOne(leaf);
        }
    }
}

Label* Label::leaf(const QString &name)
{
    foreach(Label *l, m_leafs)
        if(l->name().compare(name) == 0)
            return l;
    return 0;
}
