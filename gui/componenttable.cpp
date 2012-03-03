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

#include "componenttable.h"
#include "co_defs.h"
#include "co.h"
#include "component.h"
#include "datasheet.h"
#include "container.h"
#include "package.h"
#include "stock.h"
#include "ptoolbutton.h"
#include "stocktable.h"

#include <QApplication>
#include <QHeaderView>
#include <QAction>
#include <QIcon>
#include <QMenu>
#include <QToolButton>
#include <QLabel>
#include <QMessageBox>

#include <QDebug>

ComponentTable::ComponentTable(CO *co, QWidget *parent) :
    pTableWidget(parent),
    m_co(co),
    m_markLowStock(true)
{
    setColumnCount(6);
    setDefaultSortedColumn(NameColumn);

    QStringList headerLabels;
    headerLabels.append(tr("ID"));
    headerLabels.append(tr("Name"));
    headerLabels.append(tr("Description"));
    headerLabels.append(tr("Datasheet"));
    headerLabels.append(tr("Stock"));
    headerLabels.append(tr("Container"));

    setHorizontalHeaderLabels(headerLabels);

    setColumnHidden(IDColumn, true);
    setColumnHidden(ContainerColumn, true);

    horizontalHeader()->setResizeMode(IDColumn, QHeaderView::Fixed);
    horizontalHeader()->setResizeMode(NameColumn, QHeaderView::Fixed);
    horizontalHeader()->setResizeMode(DescriptionColumn, QHeaderView::Stretch);
    horizontalHeader()->setResizeMode(DatasheetColumn, QHeaderView::Fixed);
    horizontalHeader()->setResizeMode(StockColumn, QHeaderView::Fixed);
    horizontalHeader()->setResizeMode(ContainerColumn, QHeaderView::Fixed);

    setColumnWidth(IDColumn, 50);
    setColumnWidth(NameColumn, 120);
    setColumnWidth(DatasheetColumn, 80);
    setColumnWidth(StockColumn, 60);
    setColumnWidth(ContainerColumn, 100);

    m_contextMenu = new QMenu(this);

    m_actionShowDetails = m_contextMenu->addAction(tr("Show &Details..."));
    m_actionEdit = m_contextMenu->addAction(tr("&Edit"));
    m_actionRemove = m_contextMenu->addAction(tr("&Remove"));
    m_contextMenu->addSeparator();
    m_actionNew = m_contextMenu->addAction(tr("&New..."));

    connect(m_actionShowDetails, SIGNAL(triggered()), this, SLOT(showDetailsHandler()));
    connect(m_actionEdit, SIGNAL(triggered()), this, SLOT(editHandler()));
    connect(m_actionRemove, SIGNAL(triggered()), this, SLOT(removeHandler()));
    connect(m_actionNew, SIGNAL(triggered()), this, SIGNAL(newComponentRequest()));

    connect(this, SIGNAL(itemSelectionChanged()), this, SLOT(itemSelectionChangedHandler()));
    connect(this, SIGNAL(cellDoubleClicked(int,int)), this, SLOT(showDetailsHandler()));
}

Component* ComponentTable::component(int row)
{
    int ID = item(row, IDColumn)->text().toInt();
    return m_co->findComponent(ID);
}

void ComponentTable::fillRow(int row, Component *component)
{
    bool changeColor = false;
    QColor color;

    if(m_markLowStock && !component->ignoreStock())
    {
        if(component->totalStock() == 0)
        {
            changeColor = true;
            color = StockTable::withoutStockColor;
        }
        else
        {
            foreach(Stock *s, component->stocks())
            {
                if(s->stock() == 0)
                {
                    changeColor = true;
                    color = StockTable::withoutStockColor;
                    break;
                }
                else if(s->stock() <= s->lowValue())
                {
                    changeColor = true;
                    color = StockTable::lowStockColor;
                }
            }
        }
    }

    QTableWidgetItem *ID = new QTableWidgetItem(QString::number(component->ID()));

    QTableWidgetItem *name = new QTableWidgetItem(component->name());
    name->setTextAlignment(Qt::AlignCenter);
    if(changeColor) name->setBackgroundColor(color);

    QTableWidgetItem *desc = new QTableWidgetItem(component->description());
    if(changeColor) desc->setBackgroundColor(color);

    QTableWidgetItem *view = new QTableWidgetItem("");
    view->setTextAlignment(Qt::AlignCenter);
    if(changeColor) view->setBackgroundColor(color);

    pToolButton *viewButton = new pToolButton(this);
    if(changeColor) view->setBackgroundColor(color);

    Component *link;
    if(component->isLinked())
        link = component->linkedTo();
    else
        link = component;

    if(link->defaultDatasheet() == 0)
        view->setText("n/a");
    else
        view->setText("view");

    viewButton->setNumber(row);
    connect(viewButton, SIGNAL(pressed()), this, SLOT(updateSelectedRowFromToolButton()));
    connect(viewButton, SIGNAL(clicked()), this, SLOT(viewDatasheetHandler()));

    QTableWidgetItem *stock;
    if(component->ignoreStock())
        stock = new QTableWidgetItem("---");
    else
        stock = new QTableWidgetItem(QString::number(component->totalStock()));
    stock->setTextAlignment(Qt::AlignCenter);
    if(changeColor) stock->setBackgroundColor(color);

    QTableWidgetItem *container = new QTableWidgetItem("");
    if(component->container() != 0)
    {
        container->setText(component->container()->name());
        container->setTextAlignment(Qt::AlignCenter);
    }
    if(changeColor) container->setBackgroundColor(color);

    setSortingEnabled(false);
    setItem(row, IDColumn, ID);
    setItem(row, NameColumn, name);
    setItem(row, DescriptionColumn, desc);
    setItem(row, DatasheetColumn, view);
    setItem(row, StockColumn, stock);
    setItem(row, ContainerColumn, container);

    setCellWidget(row, DatasheetColumn, viewButton);
    setSortingEnabled(true);
    horizontalHeader()->setSortIndicatorShown(false);
}

int ComponentTable::addComponent(Component *component)
{
    int row = rowCount();
    insertRow(row);
    fillRow(row, component);
    updateToolButtonNumber();
    return row;
}

void ComponentTable::updateRowContents(int row)
{
    Component *c = component(row);
    removeRow(row);
    insertRow(row);
    fillRow(row, c);
    updateToolButtonNumber();
    setCurrentCell(row, 0);
}

void ComponentTable::viewDatasheetHandler()
{
    Component *c = component(currentRow());
    Component *link;

    if(c->isLinked())
        link = c->linkedTo();
    else
        link = c;

    if(!link->datasheets().isEmpty())
    {
        Datasheet *d = link->defaultDatasheet();
        QString filePath = d->path();
        QString fullPath = QApplication::applicationDirPath() +
                CO_DATASHEET_PATH + filePath;

        m_co->execFile(fullPath);
    }
    else
    {

    }
}

void ComponentTable::showDetailsHandler()
{
    if(m_selected != 0)
    {
        emit showDetailsRequest(m_selected);
    }
}

void ComponentTable::editHandler()
{
    if(m_selected != 0)
    {
        emit editRequest(m_selected);
    }
}
#include <QScrollBar>
void ComponentTable::removeHandler()
{
    if(m_selected != 0)
    {
        qDebug() << "removeHandler()" << m_selected->name();

        int res = QMessageBox::question(this, tr("Confirm"),
                      tr("Are you sure you want to remove \"") + m_selected->name() + "\"?\n" +
                      tr("All datasheets will be deleted."),
                      QMessageBox::Yes, QMessageBox::No);
        if(res == QMessageBox::Yes)
        {
            Component *c = component(currentRow());
            m_co->removeComponent(c);

            removeRow(currentRow());
        }
    }
}

void ComponentTable::itemSelectionChangedHandler()
{
    m_selected = component(currentRow());
}

void ComponentTable::showContextMenu(const QPoint &pos)
{   
    if(!m_contextMenu->isEmpty())
    {
        if(itemAt(pos) == 0)
        {
            m_actionShowDetails->setEnabled(false);
            m_actionEdit->setEnabled(false);
            m_actionRemove->setEnabled(false);
        }
        else
        {
            m_actionShowDetails->setEnabled(true);
            m_actionEdit->setEnabled(true);
            m_actionRemove->setEnabled(true);
        }
        m_contextMenu->exec(viewport()->mapToGlobal(pos));
    }
}

