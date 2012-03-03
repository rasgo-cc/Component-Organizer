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

#include "pminitablewidget.h"

#include <QHeaderView>
#include <QFont>
#include <QDebug>

pMiniTableWidget::pMiniTableWidget(QWidget *parent) :
    QTableWidget(parent),
    m_defaultRowColor(QColor("#000000")),
    m_addRowColor(QColor("#D6FFE2")),
    m_removeRowColor(QColor("#FFE3E3")),
    m_changeRowColor(QColor("#FFF7BF"))
{
    QFont f = font();
    f.setPointSize(8);
    f.setBold(false);
    setFont(f);

    setColumnCount(1);

    setSortingEnabled(true);

    QHeaderView *header;

    // Horizontal header
    header = horizontalHeader();
    header->setHighlightSections(false);
    header->setSortIndicatorShown(true);
    header->setDefaultSectionSize(50);
    header->setStretchLastSection(true);
    header->hide();

    // Vertical header
    header = verticalHeader();
    header->setHighlightSections(false);
    header->setDefaultSectionSize(16);
    header->hide();

    // Frame
    setFrameStyle(QFrame::StyledPanel);

    setSelectionMode(QAbstractItemView::SingleSelection);
    setSelectionBehavior(QAbstractItemView::SelectRows);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void pMiniTableWidget::removeAll()
{
    clearContents();
    while(rowCount() > 0)
        removeRow(0);
}

void pMiniTableWidget::addItem(int row, int column, const QString &text)
{
    QTableWidgetItem *item = new QTableWidgetItem(text);
    setItem(row, column, item);
}

void pMiniTableWidget::setItemText(int row, int column, const QString &text)
{
    QTableWidgetItem *i = item(row, column);
    if(i != 0)
        i->setText(text);
}

QString pMiniTableWidget::itemText(int row, int column)
{
    return item(row, column)->text();
}

void pMiniTableWidget::setCurrentRow(int row)
{
    setCurrentCell(row, 0);
}

void pMiniTableWidget::setRowColorHint(int row, RowColorHint hint)
{
    for(int col = 0; col < columnCount(); col++)
    {
        QTableWidgetItem *i = item(row, col);
        if(i == 0) continue;

        switch(hint)
        {
        case defaultRowColorHint:
            i->setBackgroundColor(m_defaultRowColor);
            break;
        case addRowColorHint:
            i->setBackgroundColor(m_addRowColor);
            break;
        case removeRowColorHint:
            i->setBackgroundColor(m_removeRowColor);
            break;
        case changeRowColorHint:
            i->setBackgroundColor(m_changeRowColor);
            break;
        }
    }
}

void pMiniTableWidget::setRowColor(int row, QColor color)
{
    for(int column = 0; column < columnCount(); column++)
    {
        item(row, column)->setBackgroundColor(color);
    }
}

pMiniTableWidget::RowColorHint pMiniTableWidget::rowColorHint(int row)
{
    for(int col = 0; col < columnCount(); col++)
    {
        QTableWidgetItem *i = item(row, col);
        if(i == 0) continue;

        if(i->backgroundColor().name() == m_addRowColor.name())
            return addRowColorHint;
        if(i->backgroundColor().name() == m_removeRowColor.name())
            return removeRowColorHint;
        if(i->backgroundColor().name() == m_changeRowColor.name())
            return changeRowColorHint;
    }

    return defaultRowColorHint;
}

QList<int> pMiniTableWidget::rows(RowColorHint hint)
{
    QList<int> list;
    for(int row = 0; row < rowCount(); row++)
    {
        if(rowColorHint(row) == hint)
            list.append(row);
    }

    return list;
}

int pMiniTableWidget::hasText(const QString &text, int column, Qt::CaseSensitivity cs)
{
    for(int row = 0; row < rowCount(); row++)
    {
        if(item(row, column)->text().compare(text, cs) == 0)
            return row;
    }
    return -1;
}
