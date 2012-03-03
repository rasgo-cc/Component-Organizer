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

#include "datasheettable.h"
#include "datasheet.h"
#include "manufacturer.h"

#include <QHeaderView>
#include <QLabel>
#include <QDebug>

DatasheetTable::DatasheetTable(QWidget *parent) :
    pMiniTableWidget(parent),
    m_defaultDatasheetRow(-1)
{
    setColumnCount(4);

    QStringList headerLabels;
    headerLabels.append(tr("Type"));
    headerLabels.append(tr("Manufacturer"));
    headerLabels.append(tr("Path"));
    headerLabels.append("");

    setHorizontalHeaderLabels(headerLabels);

    QHeaderView *header;
    header = horizontalHeader();
    header->setResizeMode(TypeColumn,         QHeaderView::Fixed);
    header->setResizeMode(ManufacturerColumn, QHeaderView::Fixed);
    header->setResizeMode(PathColumn,         QHeaderView::Stretch);
    header->setResizeMode(DefaultColumn,      QHeaderView::Fixed);
    header->setStretchLastSection(false);
    header->show();

    header = verticalHeader();
    header->setResizeMode(QHeaderView::Fixed);
    header->hide();

    setColumnWidth(TypeColumn,         80);
    setColumnWidth(ManufacturerColumn, 130);
    setColumnWidth(DefaultColumn,      16);
}

Datasheet::Type DatasheetTable::type(int row)
{
    QTableWidgetItem *type = item(row, TypeColumn);
    return Datasheet::typeFromString(type->text());
}

QString DatasheetTable::manufacturer(int row)
{
    QTableWidgetItem *manufacturer = item(row, ManufacturerColumn);
    return manufacturer->text();
}

QString DatasheetTable::path(int row)
{
    QTableWidgetItem *path = item(row, PathColumn);
    return path->text();
}

bool DatasheetTable::isDefaultDatasheet(int row)
{
    if(row == m_defaultDatasheetRow)
        return true;
    else
        return false;
}

void DatasheetTable::setDefaultDatasheet(int row)
{
    for(int r = 0; r < rowCount(); r++)
    {
        QWidget *w = cellWidget(r, DefaultColumn);
        removeCellWidget(r, DefaultColumn);
        delete w;
    }

    if(row < 0)
    {
        m_defaultDatasheetRow = -1;
        return;
    }

    QLabel *bullet = new QLabel(this);
    bullet->setPixmap(QPixmap(":/img/bullet_star.png"));
    setCellWidget(row, DefaultColumn, bullet);

    m_defaultDatasheetRow = row;
}

void DatasheetTable::autoSetDefaultDatasheet()
{
    QList<int> datasheetsToAddRows = rows(DatasheetTable::addRowColorHint);
    if(!datasheetsToAddRows.isEmpty())
    {
        setDefaultDatasheet(datasheetsToAddRows.first());
        return;
    }

    QList<int> datasheetsToUpdateRows = rows(DatasheetTable::defaultRowColorHint);
    if(!datasheetsToUpdateRows.isEmpty())
    {
        setDefaultDatasheet(datasheetsToUpdateRows.first());
        return;
    }
}

bool DatasheetTable::hasDatasheet(Datasheet *datasheet)
{
    for(int row = 0; row < rowCount(); row++)
    {
        if(rowColorHint(row) == DatasheetTable::removeRowColorHint)
            continue;

        if(type(row) == datasheet->type() &&
           manufacturer(row) == datasheet->manufacturer()->name())
        {
            return true;
        }
    }
    return false;
}

void DatasheetTable::fillRow(int row, Datasheet *datasheet)
{
    QTableWidgetItem *type;
    QTableWidgetItem *manufacturer;
    QTableWidgetItem *path;

    type = new QTableWidgetItem(Datasheet::typeToString(datasheet->type()));
    if(datasheet->manufacturer() != 0)
        manufacturer = new QTableWidgetItem(datasheet->manufacturer()->name());
    else
        manufacturer = new QTableWidgetItem(QString(""));
    path = new QTableWidgetItem(datasheet->path());

    path->setToolTip(path->text());

    setItem(row, TypeColumn, type);
    setItem(row, ManufacturerColumn, manufacturer);
    setItem(row, PathColumn, path);
}

int DatasheetTable::addDatasheet(Datasheet *datasheet)
{
    int row = rowCount();
    insertRow(row);
    fillRow(row, datasheet);

    if(rowCount() == 1)
        autoSetDefaultDatasheet(); // Is it really necessary?

    return row;
}

void DatasheetTable::removeRow(int row)
{
    pMiniTableWidget::removeRow(row);

    if(row == m_defaultDatasheetRow)
        autoSetDefaultDatasheet();
}

