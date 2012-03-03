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

#ifndef COMPONENTTABLE_H
#define COMPONENTTABLE_H

#include "ptablewidget.h"

class CO;
class Component;
class QMenu;

class ComponentTable : public pTableWidget
{
    Q_OBJECT
public:
    enum ColumnIndex
    {
        IDColumn = 0,
        NameColumn = 1,
        DescriptionColumn = 2,
        DatasheetColumn = 3,
        StockColumn = 4,
        ContainerColumn = 5
    };

    explicit ComponentTable(CO *co, QWidget *parent = 0);

    void setMarkLowStock(bool mark) { m_markLowStock = mark; }
    Component* component(int row);


signals:
    void showDetailsRequest(Component *);
    void editRequest(Component *);
    void removeRequest(Component *);
    void newComponentRequest();
    
public slots:
    int addComponent(Component *component);
    void updateRowContents(int row);
    void showContextMenu(const QPoint &pos);

private slots:
    void viewDatasheetHandler();
    void showDetailsHandler();
    void editHandler();
    void removeHandler();
    void itemSelectionChangedHandler();

private:
    CO *m_co;
    Component *m_selected;

    QMenu *m_contextMenu;
    QAction *m_actionShowDetails;
    QAction *m_actionEdit;
    QAction *m_actionRemove;
    QAction *m_actionNew;

    bool m_markLowStock;

    void fillRow(int row, Component *component);
    
};

#endif // COMPONENTTABLE_H
