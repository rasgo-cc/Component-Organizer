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

#include "componentdetails.h"
#include "ui_componentdetails.h"

#include "co.h"
#include "co_defs.h"
#include "component.h"
#include "container.h"
#include "stock.h"
#include "datasheettable.h"
#include "stocktable.h"
#include "label.h"

#include <QDebug>

ComponentDetails::ComponentDetails(CO *co, Component *component, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComponentDetails),
    m_component(component),
    m_co(co)
{
    ui->setupUi(this);

    m_datasheetTable = new DatasheetTable(this);
    m_stockTable = new StockTable(this);

    m_datasheetTable->setMinimumSize(400,100);
    m_datasheetTable->setMaximumHeight(100);
    m_stockTable->setMaximumSize(240, 105);

    setWindowTitle("Details of " + component->name());

    setup();
}

ComponentDetails::~ComponentDetails()
{
    delete m_stockTable;
    delete m_datasheetTable;
    delete ui;
}

void ComponentDetails::setup()
{
    if(m_component->isLinked())
    {
        ui->linkedTo_label->setText(tr("linked to ") + m_component->linkedTo()->name());
        ui->linkedTo_label->show();
    }
    else
        ui->linkedTo_label->hide();

    ui->name_label->setText(m_component->name());
    ui->description_label->setText(m_component->description());

    ui->datasheet_groupBox->layout()->addWidget(m_datasheetTable);

    ui->stock_groupBox->layout()->addWidget(m_stockTable);


    Component *link;
    if(m_component->isLinked())
        link = m_component->linkedTo();
    else
        link = m_component;

    foreach(Datasheet *d, link->datasheets())
    {
        int row = m_datasheetTable->addDatasheet(d);
        if(d == link->defaultDatasheet())
            m_datasheetTable->setDefaultDatasheet(row);
    }

    foreach(Stock *s, m_component->stocks())
    {
        m_stockTable->addStock(s);
    }
    updateStockTable();


    if(m_component->container() != 0)
        ui->container_lineEdit->setText(m_component->container()->name());

    if(m_component->primaryLabel() != 0)
        ui->primaryLabel_lineEdit->setText(m_component->primaryLabel()->name());
    if(m_component->secondaryLabel() != 0)
        ui->secondaryLabel_lineEdit->setText(m_component->secondaryLabel()->name());

    ui->notes_textEdit->setPlainText(m_component->notes());

    //ui->label_groupBox->hide();

    connect(m_datasheetTable, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(viewDatasheetHandler()));
    connect(m_stockTable, SIGNAL(stockChanged(int)), this, SLOT(updateStockTable()));
    connect(m_stockTable, SIGNAL(lowValueChanged(int)), this, SLOT(updateStockTable()));
}

void ComponentDetails::viewDatasheetHandler()
{
    int row = m_datasheetTable->currentRow();
    QString filePath = m_datasheetTable->path(row);
    m_co->execFile(m_co->dirPath() + CO_DATASHEET_PATH + filePath);
}

void ComponentDetails::updateStockTable()
{
    m_stockTable->clearSelection();

    for(int row = 0; row < m_stockTable->rowCount(); row++)
    {
        if(m_stockTable->stock(row) == 0)
            m_stockTable->setRowColor(row, StockTable::withoutStockColor);
        else if(m_stockTable->stock(row) <= m_stockTable->lowValue(row))
            m_stockTable->setRowColor(row, StockTable::lowStockColor);
        else
            m_stockTable->setRowColor(row, Qt::white);
    }
}

void ComponentDetails::accept()
{
    int total = 0;

    for(int row = 0; row < m_stockTable->rowCount(); row++)
    {
        qDebug() << "update stock" << m_stockTable->package(row);
        QString packageName = m_stockTable->package(row);
        Stock *s = m_component->stock(packageName);
        s->setStock(m_stockTable->stock(row));
        s->setLowValue(m_stockTable->lowValue(row));

        total += m_stockTable->stock(row);
    }
    m_component->setTotalStock(total);

    done(QDialog::Accepted);
}

void ComponentDetails::closeEvent(QCloseEvent *)
{
    accept();
}
