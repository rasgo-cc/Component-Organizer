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

#include "stocktable.h"

#include "stock.h"
#include "package.h"
#include "pspinbox.h"

#include <QHeaderView>

const QColor StockTable::lowStockColor = QColor("#FFEEAB");
const QColor StockTable::withoutStockColor = QColor("#FFD6D6");

StockTable::StockTable(QWidget *parent) :
    pMiniTableWidget(parent)
{
    setColumnCount(3);

    QStringList headerLabels;
    headerLabels.append(tr("Package"));
    headerLabels.append(tr("Stock"));
    headerLabels.append(tr("Low"));

    setHorizontalHeaderLabels(headerLabels);

    QHeaderView *header;
    header = horizontalHeader();
    header->setResizeMode(0, QHeaderView::Stretch);
    header->setResizeMode(1, QHeaderView::Fixed);
    header->setResizeMode(2, QHeaderView::Fixed);
    header->setStretchLastSection(false);
    header->show();

    setColumnWidth(1, 70);
    setColumnWidth(2, 70);
}

QString StockTable::package(int row)
{
    QTableWidgetItem *package = item(row, PackageColumn);
    return package->text();
}

int StockTable::stock(int row)
{
    pSpinBox *stock = (pSpinBox *)cellWidget(row, StockColumn);
    return stock->value();
}

int StockTable::lowValue(int row)
{
    pSpinBox *low = (pSpinBox *)cellWidget(row, LowColumn);
    return low->value();
}

void StockTable::fillRow(int row, Stock *stock)
{
    QTableWidgetItem *packageItem = new QTableWidgetItem(stock->package()->name());
    QTableWidgetItem *stockItem = new QTableWidgetItem("");
    QTableWidgetItem *lowItem = new QTableWidgetItem("");
    pSpinBox *stockWidget = new pSpinBox();
    pSpinBox *lowWidget = new pSpinBox();

    stockWidget->setNumber(row);
    lowWidget->setNumber(row);

    stockWidget->setMaximum(999999);
    lowWidget->setMaximum(999999);

    stockWidget->setAlignment(Qt::AlignRight);
    lowWidget->setAlignment(Qt::AlignRight);

    stockWidget->setValue(stock->stock());
    lowWidget->setValue(stock->lowValue());

    setItem(row, PackageColumn, packageItem);
    setItem(row, StockColumn, stockItem);
    setItem(row, LowColumn, lowItem);
    setCellWidget(row, StockColumn, stockWidget);
    setCellWidget(row, LowColumn, lowWidget);

    connect(stockWidget, SIGNAL(valueChanged(int)), this, SLOT(stockChangedHandler(int)));
    connect(lowWidget, SIGNAL(valueChanged(int)), this, SLOT(lowValueChangedHandler(int)));
}

int StockTable::addStock(Stock *stock)
{
    int row = rowCount();
    insertRow(row);
    fillRow(row, stock);
    return row;
}

int StockTable::totalStock()
{
    int total = 0;
    for(int row = 0; row < rowCount(); row++)
        total += stock(row);
    return total;
}

bool StockTable::hasPackage(const QString &name)
{
    for(int row = 0; row < rowCount(); row++)
    {
        if(item(row, PackageColumn)->text().compare(name) == 0)
        {
            return true;
        }
    }

    return false;
}

void StockTable::stockChangedHandler(int newValue)
{
    pSpinBox *s = qobject_cast<pSpinBox *>(sender());
    setCurrentRow(s->number());
    emit stockChanged(currentRow());
}

void StockTable::lowValueChangedHandler(int newValue)
{
    pSpinBox *s = qobject_cast<pSpinBox *>(sender());
    setCurrentRow(s->number());
    emit lowValueChanged(currentRow());
}

