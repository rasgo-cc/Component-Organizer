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

#ifndef APPLICATIONNOTETABLE_H
#define APPLICATIONNOTETABLE_H

#include "ptablewidget.h"

class CO;
class ApplicationNote;

class ApplicationNoteTable : public pTableWidget
{
    Q_OBJECT
public:
    enum ColumnIndex
    {
        DescriptionColumn = 0,
        NameColumn = 1,
        PdfColumn = 2,
        attachedFileColumn = 3
    };

    explicit ApplicationNoteTable(CO *co, QWidget *parent = 0);

    ApplicationNote* applicationNote(int row);

signals:
    void editRequest(ApplicationNote *);
    //void removeRequest(ApplicationNote *);
    void newApplicationNoteRequest();

public slots:
    int addApplicationNote(ApplicationNote *appnote);
    void updateRowContents(int row);
    void showContextMenu(const QPoint &pos);

private slots:
    void viewPDFHandler();
    void openattachedFileHandler();
    void editHandler();
    void removeHandler();

private:
    CO *m_co;
    ApplicationNote *m_selected;

    QMenu *m_contextMenu;
    QAction *m_actionEdit;
    QAction *m_actionRemove;
    QAction *m_actionNew;

    void fillRow(int row, ApplicationNote *appnote);
};

#endif // APPLICATIONNOTETABLE_H
