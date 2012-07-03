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

#include <QDebug>

#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "componenttable.h"
#include "applicationnotetable.h"
#include "componentdialog.h"
#include "componentdetails.h"
#include "applicationnotedialog.h"
#include "optionsdialog.h"

#include "co.h"
#include "co_defs.h"
#include "component.h"
#include "applicationnote.h"
#include "label.h"

#include <QDateTime>
#include <QMessageBox>
#include <QSettings>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    co = new CO(this);
    if(!co->readXML(co->dirPath() + CO_XML_PATH))
    {
        co->useDefaultData();
        QDir().mkdir(co->dirPath() + CO_DATA_PATH);
        QDir().mkdir(co->dirPath() + CO_DATASHEET_PATH);
        QDir().mkdir(co->dirPath() + CO_APPNOTE_PATH);
    }

    componentTable = new ComponentTable(co, this);
    appnoteTable = new ApplicationNoteTable(co, this);

    ui->stackedWidget->insertWidget(0, componentTable);
    ui->stackedWidget->insertWidget(1, appnoteTable);

    ui->stackedWidget->setCurrentIndex(0);

    ui->primaryLabel_comboBox->addItem(tr("[ALL]"));
    ui->primaryLabel_comboBox->addItem(tr("[none]"));
    foreach(Label *l, co->topLabels())
        ui->primaryLabel_comboBox->addItem(l->name());
    ui->secondaryLabel_comboBox->setEnabled(false);

    connect(ui->primaryLabel_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(primaryLabelChangedHandler()));
    connect(ui->secondaryLabel_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(secondaryLabelChangedHandler()));

    connect(ui->actionAddComponent, SIGNAL(triggered()), this, SLOT(showAddComponentDialog()));
    connect(ui->actionSettings, SIGNAL(triggered()), this, SLOT(showOptionsDialog()));
    connect(ui->actionAddApplicationNote, SIGNAL(triggered()), this, SLOT(showAddAppNoteDialog()));

    connect(ui->component_radioButton, SIGNAL(toggled(bool)), this, SLOT(changeView()));
    connect(ui->appnote_radioButton, SIGNAL(toggled(bool)), this, SLOT(changeView()));

    connect(ui->search_lineEdit, SIGNAL(textChanged(QString)), this, SLOT(search(QString)));

    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(close()));

    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(about()));

    connect(componentTable, SIGNAL(showDetailsRequest(Component*)), this, SLOT(showComponentDetailsDialog(Component*)));
    connect(componentTable, SIGNAL(editRequest(Component*)), this, SLOT(showEditComponentDialog(Component*)));
    connect(componentTable, SIGNAL(newComponentRequest()), this, SLOT(showAddComponentDialog()));

    connect(appnoteTable, SIGNAL(editRequest(ApplicationNote*)), this, SLOT(showEditAppNoteDialog(ApplicationNote*)));
    connect(appnoteTable, SIGNAL(newApplicationNoteRequest()), this, SLOT(showAddAppNoteDialog()));

    m_settings.saveDimensions = false;
    m_settings.width = 650;
    m_settings.height = 550;
    m_settings.markLowStock = true;
    m_settings.showContainers = false;

    readSettings();
    resize(m_settings.width, m_settings.height);

    foreach(Component *c, co->components())
        componentTable->addComponent(c);
    componentTable->sortByColumn(ComponentTable::NameColumn, Qt::AscendingOrder);

    foreach(ApplicationNote *a, co->applicationNotes())
        appnoteTable->addApplicationNote(a);
    appnoteTable->sortByColumn(ApplicationNoteTable::DescriptionColumn, Qt::AscendingOrder);

    /*for(int i=0; i < 20; i++)
    {
        Component *c = new Component(QString("XPTO") + QString::number(i), co);
        c->setDescription("Description of XPTO");
        co->addComponent(c);
        componentTable->addComponent(c);
    }*/
}

MainWindow::~MainWindow()
{
    delete componentTable;
    delete appnoteTable;
    delete ui;
}

void MainWindow::search(QString searchText)
{
    if(ui->component_radioButton->isChecked())
    {
        componentTable->removeAll();
        if(searchText == "")
        {
            foreach(Component *c, co->components())
                componentTable->addComponent(c);
            componentTable->sortByColumn(ComponentTable::NameColumn, Qt::AscendingOrder);
            return;
        }
        else
        {
            foreach(Component *c, co->components())
            {
                if(c->name().contains(searchText, Qt::CaseInsensitive) ||
                   c->description().contains(searchText, Qt::CaseInsensitive))
                    componentTable->addComponent(c);
                componentTable->sortByColumn(ComponentTable::NameColumn, Qt::AscendingOrder);
            }
        }
    }
    else
    {
        appnoteTable->removeAll();
        if(searchText == "")
        {
            foreach(ApplicationNote *a, co->applicationNotes())
                appnoteTable->addApplicationNote(a);
            appnoteTable->sortByColumn(ApplicationNoteTable::DescriptionColumn, Qt::AscendingOrder);
            return;
        }
        else
        {
            foreach(ApplicationNote *a, co->applicationNotes())
            {
                if(a->description().contains(searchText, Qt::CaseInsensitive) ||
                   a->name().contains(searchText, Qt::CaseInsensitive))
                    appnoteTable->addApplicationNote(a);
                appnoteTable->sortByColumn(ApplicationNoteTable::DescriptionColumn, Qt::AscendingOrder);
            }
        }
    }
}

void MainWindow::changeView()
{
    if(ui->component_radioButton->isChecked())
    {
        ui->primaryLabel_comboBox->show();
        ui->secondaryLabel_comboBox->show();

        ui->stackedWidget->setCurrentIndex(0);
    }
    else
    {
        ui->primaryLabel_comboBox->hide();
        ui->secondaryLabel_comboBox->hide();

        ui->stackedWidget->setCurrentIndex(1);
    }
}

void MainWindow::showAddComponentDialog()
{
    ComponentDialog dialog(co, this);

    if(dialog.exec() == QDialog::Accepted)
    {
        componentTable->addComponent(dialog.component());
        componentTable->sortByColumn(ComponentTable::NameColumn, Qt::AscendingOrder);
        int row = componentTable->findText(dialog.component()->name(), ComponentTable::NameColumn);
        componentTable->setCurrentRow(row);
        ui->component_radioButton->setChecked(true);

        updateXML();
    }
}

void MainWindow::showEditComponentDialog(Component *toEdit)
{
    ComponentDialog dialog(co, toEdit, this);

    if(dialog.exec() == QDialog::Accepted)
    {
        componentTable->updateRowContents(componentTable->currentRow());
        componentTable->sortByColumn(ComponentTable::NameColumn, Qt::AscendingOrder);
        int row = componentTable->findText(dialog.component()->name(), ComponentTable::NameColumn);
        componentTable->setCurrentRow(row);
        componentTable->clearSelection();

        updateXML();
    }
}

void MainWindow::showComponentDetailsDialog(Component *component)
{
    ComponentDetails dialog(co, component, this);
    dialog.exec();
    componentTable->updateRowContents(componentTable->currentRow());
    componentTable->clearSelection();
    updateXML();
}

void MainWindow::showAddAppNoteDialog()
{
    ApplicationNoteDialog dialog(co, this);

    if(dialog.exec() == QDialog::Accepted)
    {
        int row = appnoteTable->addApplicationNote(dialog.applicationNote());
        appnoteTable->setCurrentRow(row);
        appnoteTable->sortByColumn(ApplicationNoteTable::DescriptionColumn, Qt::AscendingOrder);
        ui->appnote_radioButton->setChecked(true);
        updateXML();
    }
}

void MainWindow::showEditAppNoteDialog(ApplicationNote *appnote)
{
    ApplicationNoteDialog dialog(co, appnote, this);

    if(dialog.exec() == QDialog::Accepted)
    {
        appnoteTable->updateRowContents(appnoteTable->currentRow());
        appnoteTable->sortByColumn(ApplicationNoteTable::DescriptionColumn, Qt::AscendingOrder);
        appnoteTable->clearSelection();
        updateXML();
    }
}

void MainWindow::showInfoMessage(QString msg)
{
    QMessageBox::information(this, "Info", msg, QMessageBox::Ok);
}

void MainWindow::showOptionsDialog()
{
    OptionsDialog dialog(co, this);
    dialog.exec();

    updateXML();

    if(m_settings.showContainers)
        componentTable->setColumnHidden(ComponentTable::ContainerColumn, false);
    else
        componentTable->setColumnHidden(ComponentTable::ContainerColumn, true);
    if(m_settings.markLowStock)
        componentTable->setMarkLowStock(true);
    else
        componentTable->setMarkLowStock(false);


    componentTable->removeAll();
    foreach(Component *c, co->components())
        componentTable->addComponent(c);

}

void MainWindow::primaryLabelChangedHandler()
{
    ui->secondaryLabel_comboBox->clear();
    QString labelName = ui->primaryLabel_comboBox->currentText();

    if(labelName == tr("[ALL]") || labelName == tr("[none]"))
    {
        ui->secondaryLabel_comboBox->setEnabled(false);
    }
    else
    {
        if(!labelName.isEmpty())
        {
            componentTable->removeAll();
            Label *top = co->findTopLabel(labelName);

            if(top->leafs().isEmpty())
                ui->secondaryLabel_comboBox->setEnabled(false);
            else
            {
                ui->secondaryLabel_comboBox->setEnabled(true);
                ui->secondaryLabel_comboBox->addItem("");
                foreach(Label *l, top->leafs())
                    ui->secondaryLabel_comboBox->addItem(l->name());
            }
        }
        else
            ui->secondaryLabel_comboBox->setEnabled(false);
    }

    sortyBySelectedLabels();
}

void MainWindow::secondaryLabelChangedHandler()
{
    sortyBySelectedLabels();
}

void MainWindow::sortyBySelectedLabels()
{
    QString pLabelName = ui->primaryLabel_comboBox->currentText();
    QString sLabelName = ui->secondaryLabel_comboBox->currentText();

    componentTable->removeAll();

    if(pLabelName == tr("[ALL]"))
    {
        foreach(Component *c, co->components())
            componentTable->addComponent(c);
        return;
    }
    if(pLabelName == tr("[none]"))
    {
        foreach(Component *c, co->components())
            if(c->primaryLabel() == 0)
                componentTable->addComponent(c);
        return;
    }

    foreach(Component *c, co->components())
    {
        if(c->primaryLabel() == 0)
            continue;

        if(c->primaryLabel()->name() == pLabelName)
        {
            if(sLabelName.isEmpty())
                componentTable->addComponent(c);
            else
            {
                if(c->secondaryLabel() == 0)
                    continue;

                if(c->secondaryLabel()->name() == sLabelName)
                    componentTable->addComponent(c);
            }
        }
    }
}

void MainWindow::readSettings()
{
    QSettings settings;
    settings.beginGroup("mainWindow");

    m_settings.saveDimensions = settings.value("saveDimensions").toBool();
    if(m_settings.saveDimensions)
    {
        m_settings.width = settings.value("width").toInt();
        m_settings.height = settings.value("height").toInt();
    }
    m_settings.markLowStock = settings.value("markLowStock").toBool();
    m_settings.showContainers = settings.value("showContainers").toBool();

    settings.endGroup();

    componentTable->setMarkLowStock(m_settings.markLowStock);
    componentTable->setColumnHidden(ComponentTable::ContainerColumn, !m_settings.showContainers);
}

void MainWindow::updateXML()
{
    ui->statusBar->showMessage(tr("Updating..."));
    co->updateDataXML();
    ui->statusBar->showMessage(tr("Updated"), 500);
}

#include <QTextEdit>
void MainWindow::about()
{
    QString text;

    text.append("Component Organizer " + CO_VERSION);
    text.append("\nBuilt with Qt " + QString(QT_VERSION_STR) + " ");
    text.append("(" + QDateTime::currentDateTime().toString("dd/MM/yyyy") + ")");
    text.append("\n");
    text.append("\nThis is OPEN SOURCE software.\nFeel free to contribute!");
    text.append("\n");
    text.append("\n3xbla.wordpress.com/component-organizer/");
    text.append("\ngithub.com/3xdigital/Component-Organizer");

    QMessageBox::about(this, "About", text);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    QSettings settings;

    settings.beginGroup("mainWindow");
    settings.setValue("saveDimensions", m_settings.saveDimensions);
    settings.setValue("width", width());
    settings.setValue("height", height());
    settings.setValue("markLowStock", m_settings.markLowStock);
    settings.setValue("showContainers", m_settings.showContainers);

    settings.endGroup();

    updateXML();
}


