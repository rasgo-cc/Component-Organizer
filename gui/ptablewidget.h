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

#ifndef PTABLEWIDGET_H
#define PTABLEWIDGET_H

#include <QTableWidget>

class QMenu;

class pTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    explicit pTableWidget(QWidget *parent = 0);

    void setDefaultSortedColumn(int column) { m_defaultSortedColumn = column; }
    void removeAll();
    int findText(const QString &text, int column);
    void sortByColumn(int column, Qt::SortOrder order);
    void sortByColumn(int column);
    
public slots:
    int addRow();
    void removeRow(int row);
    virtual void showContextMenu(const QPoint &pos);
    void setCurrentRow(int row);

protected slots:
    void updateSelectedRowFromToolButton();
    void updateToolButtonNumber();

private:
    int m_defaultSortedColumn;
    
};

#endif // PTABLEWIDGET_H
