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

#include <QHeaderView>
#include "applicationnotetable.h"

#include "co.h"
#include "co_defs.h"
#include "applicationnote.h"
#include "ptoolbutton.h"

#include <QMenu>
#include <QMessageBox>

ApplicationNoteTable::ApplicationNoteTable(CO *co, QWidget *parent) :
    pTableWidget(parent),
    m_co(co)
{
    setColumnCount(4);
    setDefaultSortedColumn(DescriptionColumn);

    QStringList headerLabels;
    headerLabels.append(tr("Description"));
    headerLabels.append(tr("Name"));
    headerLabels.append(tr("PDF"));
    headerLabels.append(tr("Annex"));

    setHorizontalHeaderLabels(headerLabels);

    horizontalHeader()->setResizeMode(0, QHeaderView::Stretch);
    horizontalHeader()->setResizeMode(1, QHeaderView::Fixed);
    horizontalHeader()->setResizeMode(2, QHeaderView::Fixed);
    horizontalHeader()->setResizeMode(3, QHeaderView::Fixed);

    setColumnWidth(1, 100);
    setColumnWidth(2, 80);
    setColumnWidth(3, 80);

    m_contextMenu = new QMenu(this);

    m_actionEdit = m_contextMenu->addAction(tr("&Edit"));
    m_actionRemove = m_contextMenu->addAction(tr("&Remove"));
    m_contextMenu->addSeparator();
    m_actionNew = m_contextMenu->addAction(tr("&New..."));

    connect(m_actionEdit, SIGNAL(triggered()), this, SLOT(editHandler()));
    connect(m_actionRemove, SIGNAL(triggered()), this, SLOT(removeHandler()));
    connect(m_actionNew, SIGNAL(triggered()), this, SIGNAL(newApplicationNoteRequest()));
}

ApplicationNote* ApplicationNoteTable::applicationNote(int row)
{
    QString description = item(row, DescriptionColumn)->text();
    return m_co->findApplicationNote(description);
}

void ApplicationNoteTable::fillRow(int row, ApplicationNote *appnote)
{
    QTableWidgetItem *description = new QTableWidgetItem(appnote->description());
    QTableWidgetItem *name = new QTableWidgetItem(appnote->name());
    name->setTextAlignment(Qt::AlignCenter);

    QTableWidgetItem *pdf;
    if(!appnote->pdfPath().isEmpty())
        pdf = new QTableWidgetItem(tr("view"));
    else
        pdf = new QTableWidgetItem(tr("n/a"));
    pdf->setTextAlignment(Qt::AlignCenter);

    pToolButton *pdfButton = new pToolButton(this);
    pdfButton->setNumber(row);
    connect(pdfButton, SIGNAL(pressed()), this, SLOT(updateSelectedRowFromToolButton()));
    connect(pdfButton, SIGNAL(clicked()), this, SLOT(viewPDFHandler()));

    QTableWidgetItem *attachedFile;
    if(!appnote->attachedFilePath().isEmpty())
        attachedFile = new QTableWidgetItem(tr("open"));
    else
        attachedFile = new QTableWidgetItem(tr("n/a"));
    attachedFile->setTextAlignment(Qt::AlignCenter);

    pToolButton *attachedFileButton = new pToolButton(this);
    attachedFileButton->setNumber(row);
    connect(attachedFileButton, SIGNAL(pressed()), this, SLOT(updateSelectedRowFromToolButton()));
    connect(attachedFileButton, SIGNAL(clicked()), this, SLOT(openattachedFileHandler()));

    setSortingEnabled(false);
    setItem(row, DescriptionColumn, description);
    setItem(row, NameColumn, name);
    setItem(row, PdfColumn, pdf);
    setItem(row, attachedFileColumn, attachedFile);

    setCellWidget(row, PdfColumn, pdfButton);
    setCellWidget(row, attachedFileColumn, attachedFileButton);
    setSortingEnabled(true);
    horizontalHeader()->setSortIndicatorShown(false);
}

int ApplicationNoteTable::addApplicationNote(ApplicationNote *appnote)
{
    int row = rowCount();
    insertRow(row);
    fillRow(row, appnote);
    updateToolButtonNumber();
    return row;
}

void ApplicationNoteTable::updateRowContents(int row)
{
    ApplicationNote *a = applicationNote(row);
    removeRow(row);
    insertRow(row);
    fillRow(row, a);
    updateToolButtonNumber();
    setCurrentCell(row, 0);
}

void ApplicationNoteTable::viewPDFHandler()
{
    ApplicationNote *a = applicationNote(currentRow());

    if(!a->pdfPath().isEmpty())
    {
        QString path = a->pdfPath();
        QString fullPath = m_co->dirPath() + CO_APPNOTE_PATH + path;
        m_co->execFile(fullPath);
    }
}

void ApplicationNoteTable::openattachedFileHandler()
{
    ApplicationNote *a = applicationNote(currentRow());

    if(!a->attachedFilePath().isEmpty())
    {
        QString path = a->attachedFilePath();
        QString fullPath = m_co->dirPath() + CO_APPNOTE_PATH + path;
        m_co->execFile(fullPath);
    }
}

void ApplicationNoteTable::editHandler()
{
    if(!selectedItems().isEmpty())
        emit editRequest(applicationNote(currentRow()));
}

void ApplicationNoteTable::removeHandler()
{
    if(!selectedItems().isEmpty())
    {
        ApplicationNote *a = applicationNote(currentRow());

        int res = QMessageBox::question(this, tr("Confirm"),
                      tr("Are you sure you want to remove \"") + a->description() + "\"?\n" +
                      tr("The PDF and attached files will be deleted."),
                      QMessageBox::Yes, QMessageBox::No);
        if(res == QMessageBox::Yes)
        {
            m_co->removeApplicationNote(a);

            removeRow(currentRow());
        }
    }
}

void ApplicationNoteTable::showContextMenu(const QPoint &pos)
{
    if(!m_contextMenu->isEmpty())
    {
        if(itemAt(pos) == 0)
        {
            m_actionEdit->setEnabled(false);
            m_actionRemove->setEnabled(false);
        }
        else
        {
            m_actionEdit->setEnabled(true);
            m_actionRemove->setEnabled(true);
        }
        m_contextMenu->exec(viewport()->mapToGlobal(pos));
    }
}
