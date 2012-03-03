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

#include "applicationnotedialog.h"
#include "ui_applicationnotedialog.h"

#include "co.h"
#include "co_defs.h"
#include "applicationnote.h"

#include <QMessageBox>
#include <QFileDialog>

#include <QDebug>

ApplicationNoteDialog::ApplicationNoteDialog(CO *co, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApplicationNoteDialog),
    m_co(co),
    m_mode(Add),
    m_appnote(0)
{
    setup();
}

ApplicationNoteDialog::ApplicationNoteDialog(CO *co, ApplicationNote *appnote, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ApplicationNoteDialog),
    m_co(co),
    m_mode(Edit),
    m_appnote(appnote)
{
    setup();

    ui->description_lineEdit->setText(appnote->description());
    ui->name_lineEdit->setText(appnote->name());
    ui->appnote_lineEdit->setText(appnote->pdfPath());
    ui->attach_lineEdit->setText(appnote->attachedFilePath());
}

ApplicationNoteDialog::~ApplicationNoteDialog()
{
    delete ui;
}


void ApplicationNoteDialog::setup()
{
    ui->setupUi(this);

    switch(m_mode)
    {
    case ApplicationNoteDialog::Add:
        setWindowTitle(tr("Add Application Note"));
        break;
    case ApplicationNoteDialog::Edit:
        setWindowTitle(tr("Edit Application Note"));
        break;
    }

    connect(ui->appnote_pushButton, SIGNAL(clicked()), this, SLOT(browsePDF()));
    connect(ui->attach_pushButton, SIGNAL(clicked()), this, SLOT(browseattachedFile()));

    ui->description_lineEdit->setFocus();
}

void ApplicationNoteDialog::accept()
{
    QString description = ui->description_lineEdit->text();

    if(description.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("A description must be provided."),
                              QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    switch(m_mode)
    {
    case ApplicationNoteDialog::Add:
        createApplicationNote();
        m_co->addApplicationNote(m_appnote);
        break;
    case ApplicationNoteDialog::Edit:
        updateApplicationNote();
        break;
    }

    done(QDialog::Accepted);
}

void ApplicationNoteDialog::browsePDF()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Select Application Note"),
                                                    "",
                                                    tr("Application Note (*.pdf)"));

    if(!filePath.isNull())
        ui->appnote_lineEdit->setText(filePath);
    else
        ui->appnote_lineEdit->setText("");
}

void ApplicationNoteDialog::browseattachedFile()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Select attachedFile"));

    if(!filePath.isNull())
        ui->attach_lineEdit->setText(filePath);
    else
        ui->attach_lineEdit->setText("");
}

void ApplicationNoteDialog::createApplicationNote()
{
    ApplicationNote *a = new ApplicationNote(ui->description_lineEdit->text());
    a->setName(ui->name_lineEdit->text());

    if(!ui->appnote_lineEdit->text().isEmpty())
        a->setPdfPath(savePdf());

    if(!ui->attach_lineEdit->text().isEmpty())
        a->setAttachedFilePath(saveAttachedFile());

    m_appnote = a;
}

void ApplicationNoteDialog::updateApplicationNote()
{
    m_appnote->setDescription(ui->description_lineEdit->text());
    m_appnote->setName(ui->name_lineEdit->text());

    if(!ui->appnote_lineEdit->text().isEmpty())
    {
        if(!m_appnote->pdfPath().isEmpty())
            m_co->removeFile(m_co->dirPath() + CO_APPNOTE_PATH + m_appnote->pdfPath());
        m_appnote->setPdfPath(savePdf());
    }
    else
    {
        m_co->removeFile(m_co->dirPath() + CO_APPNOTE_PATH + m_appnote->pdfPath());
        m_appnote->setPdfPath("");
    }

    if(!ui->attach_lineEdit->text().isEmpty())
    {
        if(!m_appnote->attachedFilePath().isEmpty())
            m_co->removeFile(m_co->dirPath() + CO_APPNOTE_PATH + m_appnote->attachedFilePath());
        m_appnote->setAttachedFilePath(saveAttachedFile());
    }
    else
    {
        m_co->removeFile(m_co->dirPath() + CO_APPNOTE_PATH + m_appnote->attachedFilePath());
        m_appnote->setAttachedFilePath("");
    }

}

QString ApplicationNoteDialog::savePdf()
{
    QString pdfPath = "/" + ui->description_lineEdit->text();
    pdfPath.append(".pdf");

    QString from = ui->appnote_lineEdit->text();
    QString to = m_co->dirPath() + CO_APPNOTE_PATH + pdfPath;
    m_co->copyFile(from, to);

    return pdfPath;
}

QString ApplicationNoteDialog::saveAttachedFile()
{
    QString fileName = ui->attach_lineEdit->text().split("/").last();
    QString attachedFilePath = "/" + fileName;

    QString from = ui->attach_lineEdit->text();
    QString to = m_co->dirPath() + CO_APPNOTE_PATH + attachedFilePath;
    m_co->copyFile(from, to);

    return attachedFilePath;
}
