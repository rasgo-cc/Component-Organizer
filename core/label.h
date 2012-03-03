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

#ifndef LABEL_H
#define LABEL_H

#include <QObject>

class Label : public QObject
{
    Q_OBJECT
public:
    explicit Label(const QString &name, Label *top = 0, QList<Label*> leafs = QList<Label*>(), QObject *parent = 0);

    QString name() { return m_name; }

    void setTop(Label *label) { m_top = label; }
    void addLeaf(Label *label) { m_leafs.append(label); }
    void removeLeaf(const QString &name);

    Label* top() { return m_top; }
    QList<Label*> leafs() { return m_leafs; }
    Label* leaf(const QString &name);

signals:
    
public slots:

private:

    QString       m_name;
    Label*        m_top;
    QList<Label*> m_leafs;
    
};

#endif // LABEL_H
