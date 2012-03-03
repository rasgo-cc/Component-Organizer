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

#ifndef PMINITABLEWIDGET_H
#define PMINITABLEWIDGET_H

#include <QTableWidget>

class pMiniTableWidget : public QTableWidget
{
    Q_OBJECT
public:
    enum RowColorHint
    {
        defaultRowColorHint = 0,
        addRowColorHint,
        removeRowColorHint,
        changeRowColorHint
    };

    explicit pMiniTableWidget(QWidget *parent = 0);

    void removeAll();
    void addItem(int row, int column, const QString &text);
    void setItemText(int row, int column, const QString &text);
    QString itemText(int row, int column);
    void setCurrentRow(int row);
    void setRowColorHint(int row, RowColorHint hint);
    void setRowColor(int row, QColor color);
    RowColorHint rowColorHint(int row);
    QList<int> rows(RowColorHint hint);
    int hasText(const QString &text, int column, Qt::CaseSensitivity cs = Qt::CaseSensitive);
    
signals:
    
public slots:

protected:
    const QColor m_defaultRowColor;
    const QColor m_addRowColor;
    const QColor m_removeRowColor;
    const QColor m_changeRowColor;
    
};

#endif // PMINITABLEWIDGET_H
