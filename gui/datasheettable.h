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

#ifndef DATASHEETTABLE_H
#define DATASHEETTABLE_H

#include "pminitablewidget.h"
#include "datasheet.h"

class DatasheetTable : public pMiniTableWidget
{
    Q_OBJECT
public:
    enum ColumnIndex
    {
        TypeColumn = 0,
        ManufacturerColumn = 1,
        PathColumn = 2,
        DefaultColumn = 3
    };

    explicit DatasheetTable(QWidget *parent = 0);

    Datasheet::Type type(int row);
    QString manufacturer(int row);
    QString path(int row);
    int defaultDatasheetRow() { return m_defaultDatasheetRow; }
    bool isDefaultDatasheet(int row);
    void setDefaultDatasheet(int row);
    void autoSetDefaultDatasheet();
    bool hasDatasheet(Datasheet *datasheet);
    
signals:
    
public slots:
    int addDatasheet(Datasheet *datasheet);
    void removeRow(int row);

private:
    void fillRow(int row, Datasheet *datasheet);
    
    int m_defaultDatasheetRow;
};

#endif // DATASHEETTABLE_H
