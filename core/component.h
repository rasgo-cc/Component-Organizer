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

#ifndef COMPONENT_H
#define COMPONENT_H

#include <QObject>

class Datasheet;
class Container;
class Package;
class Stock;
class Label;

class Component : public QObject
{
    Q_OBJECT
public:
    enum LabelLevel
    {
        PrimaryLabel = 0,
        SecondaryLabel = 1
    };

    explicit Component(const QString name, QObject *parent = 0);

    int ID() { return m_ID; }

    void setName(const QString &name) { m_name = name; }
    QString name() { return m_name; }

    void setDescription(const QString description) { m_description = description; }
    QString description() { return m_description; }

    void addDatasheet(Datasheet *datasheet);
    void removeDatasheet(Datasheet *datasheet);
    bool setDefaultDatasheet(Datasheet *datasheet);
    bool setDefaultDatasheetIndex(int index);
    int defaultDatasheetIndex() { return m_defaultDatasheetIndex; }
    Datasheet* defaultDatasheet();
    Datasheet* datasheet(const QString &path);
    QList<Datasheet *> datasheets() { return m_datasheets; }

    void addStock(Stock *stock);
    void removeStock(const QString &packageName);
    Stock* stock(const QString &packageName);
    QList<Stock *> stocks();

    void setIgnoreStock(bool ignore) { m_ignoreStock = ignore; }
    bool ignoreStock() { return m_ignoreStock; }

    void setTotalStock(int total) { m_totalStock = total; }
    int totalStock() { return m_totalStock; }

    void setContainer(Container *container) { m_container = container; }
    Container* container() { return m_container; }

    void setLabel(int level, Label *label);
    void setLabels(Label *primary, Label *secondary) { m_primaryLabel = primary; m_secondaryLabel = secondary; }
    Label* primaryLabel() { return m_primaryLabel; }
    Label* secondaryLabel() { return m_secondaryLabel; }

    void setNotes(const QString &notes) { m_notes = notes; }
    QString notes() { return m_notes; }

    bool isLinked() { return (m_linkedTo != 0) ? true : false; }
    void linkTo(Component *component) { m_linkedTo = component; }
    Component* linkedTo() { return m_linkedTo; }

    
signals:
    
public slots:
    
private:
    static int nextID;

    int m_ID;
    QString m_name;
    QString m_description;
    int m_defaultDatasheetIndex;
    QList<Datasheet*> m_datasheets;
    QList<Stock*> m_stocks;
    bool m_ignoreStock;
    int m_lowStock;
    int m_totalStock;
    Container *m_container;
    Label *m_primaryLabel;
    Label *m_secondaryLabel;
    QString m_notes;

    Component *m_linkedTo;
};

#endif // COMPONENT_H
