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

#include "ptablewidget.h"

#include "ptoolbutton.h"

#include <QHeaderView>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QAction>
#include <QMenu>
#include <QScrollBar>

#include <QDebug>

pTableWidget::pTableWidget(QWidget *parent) :
    QTableWidget(parent),
    m_defaultSortedColumn(0)
{
    setSortingEnabled(true);

    QHeaderView *header;

    // Horizontal header
    header = horizontalHeader();
    header->setHighlightSections(false);
    header->setSortIndicatorShown(false);
    header->setDefaultSectionSize(50);
    header->show();

    // Vertical header
    header = verticalHeader();
    header->setDefaultSectionSize(21);
    header->hide();

    // Frame
    setFrameStyle(QFrame::NoFrame);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);

    setAlternatingRowColors(true);

    setContextMenuPolicy(Qt::CustomContextMenu);
    connect(this, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(showContextMenu(QPoint)));

    connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(updateToolButtonNumber()));
}

void pTableWidget::removeAll()
{
    clearContents();

    while(rowCount() > 0)
        removeRow(0);
}

int pTableWidget::addRow()
{
    int row = rowCount();
    insertRow(row);

    return row;
}

void pTableWidget::removeRow(int row)
{
    if(rowCount() == 1)
        clearContents();

    for(int column = 0; column < columnCount(); column++)
    {
        QWidget *w = cellWidget(row, column);
        if(w != 0)
        {
            removeCellWidget(row, column);
            delete w;
        }
    }

    QTableWidget::removeRow(row);

    updateToolButtonNumber();
}

int pTableWidget::findText(const QString &text, int column)
{
    for(int row = 0; row < rowCount(); row++)
        if(item(row, column)->text().compare(text) == 0)
            return row;

    return -1;
}

void pTableWidget::sortByColumn(int column, Qt::SortOrder order)
{
    QTableWidget::sortByColumn(column, order);
    updateToolButtonNumber();
}

void pTableWidget::sortByColumn(int column)
{
    QTableWidget::sortByColumn(column);
    updateToolButtonNumber();
}

void pTableWidget::showContextMenu(const QPoint &pos)
{
    QMenu *contextMenu = new QMenu(this);
    foreach(QAction *a, actions())
        contextMenu->addAction(a);

    if(!contextMenu->isEmpty())
        contextMenu->exec(viewport()->mapToGlobal(pos));
}

void pTableWidget::setCurrentRow(int row)
{
    setCurrentCell(row, 0);
}

void pTableWidget::updateSelectedRowFromToolButton()
{
    pToolButton *b = qobject_cast<pToolButton *>(sender());
    setCurrentRow(b->number());
}

void pTableWidget::updateToolButtonNumber()
{
    for(int row = 0; row < rowCount(); row++)
    {
        for(int column = 0; column < columnCount(); column++)
        {
            pToolButton *button = (pToolButton *) cellWidget(row, column);
            if(button != 0)
            {
                button->setNumber(row);
            }
        }
    }
}



