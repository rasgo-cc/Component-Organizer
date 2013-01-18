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

#include "componentdialog.h"
#include "ui_componentdialog.h"

#include "co_defs.h"
#include "co.h"
#include "component.h"
#include "datasheet.h"
#include "manufacturer.h"
#include "package.h"
#include "stock.h"
#include "container.h"
#include "datasheettable.h"
#include "stocktable.h"
#include "label.h"

#include <QApplication>
#include <QMessageBox>
#include <QHeaderView>
#include <QPushButton>
#include <QFileDialog>
#include <QSpinBox>
#include <QLabel>

#include <QDebug>

ComponentDialog::ComponentDialog(CO *co, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComponentDialog),
    m_co(co),
    m_mode(ComponentDialog::Add)
{
    setup();
}

ComponentDialog::ComponentDialog(CO *co, Component *toEdit, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ComponentDialog),
    m_co(co) ,
    m_mode(ComponentDialog::Edit),
    m_component(toEdit)
{
    setup();

    if(m_component->isLinked())
    {
        ui->source_comboBox->setCurrentIndex(LinkToExistingComponent);
        int index = ui->component_comboBox->findText(m_component->linkedTo()->name());
        qDebug() << index << m_component->linkedTo()->name();
        ui->component_comboBox->setCurrentIndex(index);
    }

    ui->editDatasheet_toolButton->hide();
    ui->setDefaultDatasheet_toolButton->show();

    ui->name_lineEdit->setText(toEdit->name());
    ui->description_lineEdit->setText(toEdit->description());

    foreach(Datasheet *d, toEdit->datasheets())
    {
        int row = m_datasheetTable->addDatasheet(d);
        if(d == toEdit->defaultDatasheet())
            m_datasheetTable->setDefaultDatasheet(row);
    }

    foreach(Stock *s, toEdit->stocks())
        m_stockTable->addStock(s);

    if(toEdit->ignoreStock())
        ui->stock_groupBox->setChecked(false);
    else
        ui->stock_groupBox->setChecked(true);

    if(toEdit->container())
    {
        int index = ui->container_comboBox->findText(toEdit->container()->name());
        ui->container_comboBox->setCurrentIndex(index);
    }

    if(toEdit->primaryLabel())
    {
        int index = ui->primaryLabel_comboBox->findText(toEdit->primaryLabel()->name());
        ui->primaryLabel_comboBox->setCurrentIndex(index);

        if(toEdit->secondaryLabel() != 0){
            index = ui->secondaryLabel_comboBox->findText(toEdit->secondaryLabel()->name());
            ui->secondaryLabel_comboBox->setCurrentIndex(index);
        }
    }

    ui->notes_textEdit->setText(toEdit->notes());

    int index = ui->component_comboBox->findText(m_component->name());
    ui->component_comboBox->removeItem(index);

    updateBrowsedDatasheets();
    updateInterface();
}

ComponentDialog::~ComponentDialog()
{
    delete m_stockTable;
    delete m_datasheetTable;
    delete ui;
}

void ComponentDialog::setup()
{
    ui->setupUi(this);

    m_datasheetTable = new DatasheetTable(this);
    m_stockTable = new StockTable(this);

    m_datasheetTable->setMinimumWidth(400);
    m_datasheetTable->setMaximumHeight(100);
    m_stockTable->setMaximumSize(240, 100);

    // Title
    switch(m_mode)
    {
    case ComponentDialog::Add:
        setWindowTitle(tr("Add Component"));
        break;
    case ComponentDialog::Edit:
        setWindowTitle(tr("Edit Component"));
        break;
    }

    // Tool buttons
    ui->addDatasheet_toolButton->setEnabled(false);
    ui->removeDatasheet_toolButton->setEnabled(false);
    ui->editDatasheet_toolButton->setEnabled(false);
    ui->setDefaultDatasheet_toolButton->setEnabled(false);
    ui->editDatasheet_toolButton->hide();
    ui->setDefaultDatasheet_toolButton->show();
    ui->addPackage_toolButton->setEnabled(false);
    ui->removePackage_toolButton->setEnabled(false);

    connect(ui->addDatasheet_toolButton, SIGNAL(clicked()), this, SLOT(addDatasheetHandler()));
    connect(ui->removeDatasheet_toolButton, SIGNAL(clicked()), this, SLOT(removeDatasheetHandler()));
    connect(ui->setDefaultDatasheet_toolButton, SIGNAL(clicked()), this, SLOT(setDefaultHandler()));

    ui->removePackage_toolButton->setEnabled(false);

    connect(ui->addPackage_toolButton, SIGNAL(clicked()), this, SLOT(addStockHandler()));
    connect(ui->removePackage_toolButton, SIGNAL(clicked()), this, SLOT(removeStockHandler()));

    // Labels
    ui->primaryLabel_comboBox->addItem("");
    foreach(Label *l, m_co->topLabels())
        ui->primaryLabel_comboBox->addItem(l->name());

    connect(ui->primaryLabel_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateSecondaryLabel()));

    // Manufacturers
    ui->manufacturer_comboBox->addItem("");
    foreach(QString name, m_co->manufacturerNames())
        ui->manufacturer_comboBox->addItem(name);

    // Datasheet types
    foreach(QString typeName, Datasheet::typeNames())
        ui->type_comboBox->addItem(typeName);

    // Components
    foreach(QString name, m_co->componentNames())
        ui->component_comboBox->addItem(name);

    // Datasheet table
    ui->datasheet_groupBox->layout()->addWidget(m_datasheetTable);

    // Packages
    foreach(QString name, m_co->packageNames())
        ui->package_comboBox->addItem(name);

    if(ui->package_comboBox->count() > 0)
        ui->stock_groupBox->setEnabled(true);
    else
        ui->stock_groupBox->setEnabled(false);



    // Stock table
    ui->stock_groupBox->layout()->addWidget(m_stockTable);

    // Container
    ui->container_comboBox->addItem("");
    foreach(QString name, m_co->containerNames())
        ui->container_comboBox->addItem(name);

    // Signals & Slots
    connect(ui->source_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(updateInterface()));
    connect(ui->source_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(datasheetSourceChangedHandler()));
    connect(ui->component_comboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(datasheetSourceChangedHandler()));
    connect(ui->browse_pushButton, SIGNAL(clicked()), this, SLOT(browseFile()));
    connect(ui->stock_groupBox, SIGNAL(clicked()), this, SLOT(updateInterface()));
    connect(m_datasheetTable, SIGNAL(itemSelectionChanged()), this, SLOT(updateInterface()));
    connect(m_stockTable, SIGNAL(itemSelectionChanged()), this, SLOT(updateInterface()));
    connect(m_stockTable, SIGNAL(stockChanged(int)), this, SLOT(stockChangedHandler(int)));
    connect(m_stockTable, SIGNAL(lowValueChanged(int)), this, SLOT(stockChangedHandler(int)));

    ui->name_lineEdit->setFocus();
}


void ComponentDialog::accept()
{
    QString name = ui->name_lineEdit->text();

    if(name.isEmpty())
    {
        QMessageBox::critical(this, tr("Error"), tr("Invalid name"),
                              QMessageBox::Ok, QMessageBox::NoButton);
        return;
    }

    switch(m_mode)
    {
    case ComponentDialog::Add:
        if(m_co->componentNames().contains(name, Qt::CaseInsensitive))
        {
            QMessageBox::critical(this, tr("Error"),
                                  tr("A component with name \"") + name +
                                  tr("\" already exists.") + "\n" +
                                  tr("The component's name must be unique."),
                                  QMessageBox::Ok);
            return;
        }
        createComponent();
        m_co->addComponent(m_component);
        break;
    case ComponentDialog::Edit:
        if(m_component->name() != name &&
           m_co->componentNames().contains(name, Qt::CaseInsensitive))
        {
            QMessageBox::critical(this, tr("Error"),
                                  tr("A component with name ") + name +
                                  tr(" already exists.") + "\n" +
                                  tr("The component's name must be unique."),
                                  QMessageBox::Ok);
            return;
        }
        updateComponent();
        break;
    }

    done(QDialog::Accepted);
}

void ComponentDialog::datasheetSourceChangedHandler()
{
    int index = ui->source_comboBox->currentIndex();

    if(index == BrowseFromDisk)
    {
        m_datasheetTable->removeAll();
        for(int row = 0; row < m_browsedDatasheets.count(); row++)
        {
            m_datasheetTable->insertRow(row);
            m_datasheetTable->addItem(row, DatasheetTable::TypeColumn, m_browsedDatasheets[row].at(0));
            m_datasheetTable->addItem(row, DatasheetTable::ManufacturerColumn, m_browsedDatasheets[row].at(1));
            m_datasheetTable->addItem(row, DatasheetTable::PathColumn, m_browsedDatasheets[row].at(2));
            if(m_browsedDatasheets[row].at(3).compare("default") == 0)
                m_datasheetTable->setDefaultDatasheet(row);
        }
    }
    else if(index == LinkToExistingComponent)
    {
        QString componentName = ui->component_comboBox->currentText();
        Component *c = m_co->findComponent(componentName);
        if(c != 0)
        {
            m_datasheetTable->removeAll();
            for(int row = 0; row < c->datasheets().count(); row++)
            {
                Datasheet *d = c->datasheets().at(row);

                m_datasheetTable->insertRow(row);
                m_datasheetTable->addItem(row, DatasheetTable::TypeColumn, Datasheet::typeToString(d->type()));
                if(d->manufacturer() != 0)
                    m_datasheetTable->addItem(row, DatasheetTable::ManufacturerColumn, d->manufacturer()->name());
                else
                    m_datasheetTable->addItem(row, DatasheetTable::ManufacturerColumn, "");
                m_datasheetTable->addItem(row, DatasheetTable::PathColumn, d->path());
                if(d == c->defaultDatasheet())
                    m_datasheetTable->setDefaultDatasheet(row);
            }
        }
    }
}

Datasheet* ComponentDialog::createDatasheet(int row)
{
    QString filePath = "/" +
                       ui->name_lineEdit->text().toUpper() + "_" +
                       Datasheet::typeToString(m_datasheetTable->type(row));

    if(!m_datasheetTable->manufacturer(row).isEmpty())
        filePath.append("_" + m_datasheetTable->manufacturer(row));

    filePath.append(".pdf");

    QString from = m_datasheetTable->path(row);
    QString to = m_co->dirPath() + CO_DATASHEET_PATH + filePath;

    m_co->copyFile(from, to);

    Datasheet *d = new Datasheet(filePath);
    d->setType(m_datasheetTable->type(row));

    if(!m_datasheetTable->manufacturer(row).isEmpty())
    {
        Manufacturer *m = m_co->findManufacturer(m_datasheetTable->manufacturer(row));
        d->setManufacturer(m);
    }

    return d;
}

void ComponentDialog::createComponent()
{
    Component *c = new Component(ui->name_lineEdit->text(), m_co);
    c->setDescription(ui->description_lineEdit->text());

    for(int row = 0; row < m_datasheetTable->rowCount(); row++)
    {
        Datasheet *d = createDatasheet(row);
        c->addDatasheet(d);
        if(m_datasheetTable->isDefaultDatasheet(row))
            c->setDefaultDatasheet(d);
    }

    if(ui->stock_groupBox->isChecked())
        c->setIgnoreStock(false);
    else
        c->setIgnoreStock(true);

    for(int row = 0; row < m_stockTable->rowCount(); row++)
    {
        Package *p = m_co->findPackage(m_stockTable->package(row));

        int stock = m_stockTable->stock(row);
        int lowValue = m_stockTable->lowValue(row);

        Stock *s = new Stock(p);
        s->setStock(stock);
        s->setLowValue(lowValue);

        c->addStock(s);
    }

    Container *container = m_co->findContainer(ui->component_comboBox->currentText());
    c->setContainer(container);

    Label *primary = m_co->findTopLabel(ui->primaryLabel_comboBox->currentText());
    Label *secondary = 0;
    if(primary != 0)
        secondary = primary->leaf(ui->secondaryLabel_comboBox->currentText());

    c->setLabels(primary, secondary);

    c->setNotes(ui->notes_textEdit->toPlainText());

    m_component = c;
}

void ComponentDialog::updateComponent()
{
    m_component->setName(ui->name_lineEdit->text());
    m_component->setDescription(ui->description_lineEdit->text());

    updateDatasheets();
    updateStock();
    updateContainer();
    updateLabels();

    m_component->setNotes(ui->notes_textEdit->toPlainText());
}

void ComponentDialog::updateDatasheets()
{
    if(ui->source_comboBox->currentIndex() == LinkToExistingComponent)
    {
        while(m_component->datasheets().count() > 0)
        {
            Datasheet *d = m_component->datasheets().at(0);
            m_co->removeFile(m_co->dirPath() + CO_DATASHEET_PATH + d->path());
            m_component->removeDatasheet(d);
        }

        QString name = ui->component_comboBox->currentText();
        m_component->linkTo(m_co->findComponent(name));
        return;
    }
    else
        m_component->linkTo(0);

    bool defaultRemoved = false;

    QList<int> datasheetsToRemoveRows = m_datasheetTable->rows(DatasheetTable::removeRowColorHint);
    foreach(int row, datasheetsToRemoveRows)
    {
        if(m_datasheetTable->isDefaultDatasheet(row))
            defaultRemoved = true;

        QString path = m_datasheetTable->path(row);
        Datasheet *d = m_component->datasheet(path);
        m_component->removeDatasheet(d);

        m_co->removeFile(m_co->dirPath() + CO_DATASHEET_PATH + path);
    }

    QList<int> datasheetsToAddRows = m_datasheetTable->rows(DatasheetTable::addRowColorHint);
    foreach(int row, datasheetsToAddRows)
    {
        Datasheet *d = createDatasheet(row);
        m_component->addDatasheet(d);

        if(m_datasheetTable->isDefaultDatasheet(row))
            m_component->setDefaultDatasheet(d);
    }

    QList<int> datasheetsToUpdateRows = m_datasheetTable->rows(DatasheetTable::defaultRowColorHint);
    foreach(int row, datasheetsToUpdateRows)
    {
        if(m_datasheetTable->isDefaultDatasheet(row))
        {
            QString path = m_datasheetTable->path(row);
            Datasheet *defaultDatasheet = m_component->datasheet(path);
            m_component->setDefaultDatasheet(defaultDatasheet);
        }
    }

    if(defaultRemoved && !m_component->datasheets().isEmpty())
    {
        Datasheet *defaultDatasheet = m_component->datasheets().first();
        m_component->setDefaultDatasheet(defaultDatasheet);
    }
}

void ComponentDialog::updateStock()
{
    if(ui->stock_groupBox->isChecked())
        m_component->setIgnoreStock(false);
    else
        m_component->setIgnoreStock(true);

    QList<int> stocksToUpdateRows = m_stockTable->rows(StockTable::changeRowColorHint);
    foreach(int row, stocksToUpdateRows)
    {
        qDebug() << "update stock" << m_stockTable->package(row);
        QString packageName = m_stockTable->package(row);
        Stock *s = m_component->stock(packageName);
        s->setStock(m_stockTable->stock(row));
        s->setLowValue(m_stockTable->lowValue(row));
    }

    QList<int> stocksToRemoveRows = m_stockTable->rows(StockTable::removeRowColorHint);
    foreach(int row, stocksToRemoveRows)
    {
        qDebug() << "remove stock" << m_stockTable->package(row);
        QString packageName = m_stockTable->package(row);
        m_component->removeStock(packageName);
    }

    QList<int> stocksToAddRows = m_stockTable->rows(StockTable::addRowColorHint);
    foreach(int row, stocksToAddRows)
    {
        qDebug() << "add stock" << m_stockTable->package(row);
        Package *package = m_co->findPackage(m_stockTable->package(row));
        Stock *stock = new Stock(package);
        stock->setStock(m_stockTable->stock(row));
        stock->setLowValue(m_stockTable->lowValue(row));
        m_component->addStock(stock);
    }

    int total = 0;
    foreach(Stock *s, m_component->stocks())
        total += s->stock();
    m_component->setTotalStock(total);
}

void ComponentDialog::updateContainer()
{
    Container *container = m_co->findContainer(ui->container_comboBox->currentText());
    if(container != 0)
        m_component->setContainer(container);
}

void ComponentDialog::updateLabels()
{
    Label *primary = m_co->findTopLabel(ui->primaryLabel_comboBox->currentText());
    Label *secondary = 0;
    if(primary != 0)
        secondary = primary->leaf(ui->secondaryLabel_comboBox->currentText());

    m_component->setLabels(primary, secondary);
}

void ComponentDialog::browseFile()
{
    QString filePath = QFileDialog::getOpenFileName(this,
                                                    tr("Select Datasheet"),
                                                    "",
                                                    tr("Datasheet (*.pdf)"));

    if(!filePath.isNull())
        ui->filePath_lineEdit->setText(filePath);
    else
        ui->filePath_lineEdit->setText("");

    updateInterface();
}

void ComponentDialog::addDatasheetHandler() // TODO use addItem() instead of addDatasheet()
{
    Datasheet *d = new Datasheet(ui->filePath_lineEdit->text());
    Manufacturer *m;

    QString manufacturerName;
    manufacturerName = ui->manufacturer_comboBox->currentText();

    // verify if a manufacturer name was selected not
    if(manufacturerName != "")
        m = new Manufacturer(ui->manufacturer_comboBox->currentText());
    else
        m = new Manufacturer("0");

    d->setType(Datasheet::typeFromString(ui->type_comboBox->currentText()));
    d->setManufacturer(m);

    if(!m_datasheetTable->hasDatasheet(d))
    {
        int row = m_datasheetTable->addDatasheet(d);
        if(m_mode == ComponentDialog::Edit)
            m_datasheetTable->setRowColorHint(row, DatasheetTable::addRowColorHint);

        ui->manufacturer_comboBox->setCurrentIndex(0);
        ui->type_comboBox->setCurrentIndex(0);
        ui->filePath_lineEdit->setText("");
    }
    else
    {
        QMessageBox::information(this,
                                 tr("Info"),
                                 tr("Datasheet with same type and manufacturer already exists"),
                                 QMessageBox::Ok
                                 );
    }

    delete d;
    delete m;

    if(m_datasheetTable->defaultDatasheetRow() < 0)
        m_datasheetTable->autoSetDefaultDatasheet();

    updateBrowsedDatasheets();
    updateInterface();
}

void ComponentDialog::removeDatasheetHandler()
{
    int curRow = m_datasheetTable->currentRow();

    switch(m_mode)
    {
    case ComponentDialog::Add:
        m_datasheetTable->removeRow(curRow);
        break;
    case ComponentDialog::Edit:
        if(m_datasheetTable->rowColorHint(curRow) == DatasheetTable::addRowColorHint)
            m_datasheetTable->removeRow(curRow);
        else
            m_datasheetTable->setRowColorHint(curRow, DatasheetTable::removeRowColorHint);
        break;
    }

    if(m_datasheetTable->isDefaultDatasheet(curRow))
    {
        m_datasheetTable->setDefaultDatasheet(-1);
        m_datasheetTable->autoSetDefaultDatasheet();
    }

    m_datasheetTable->clearSelection();

    updateBrowsedDatasheets();
    updateInterface();
}

void ComponentDialog::setDefaultHandler()
{
    m_datasheetTable->setDefaultDatasheet(m_datasheetTable->currentRow());
}

void ComponentDialog::addStockHandler()
{
    Package *package = m_co->findPackage(ui->package_comboBox->currentText());
    Stock stock(package);

    stock.setStock(0);
    stock.setLowValue(1);

    int row = m_stockTable->addStock(&stock);

    if(m_mode == ComponentDialog::Edit)
        m_stockTable->setRowColorHint(row, StockTable::addRowColorHint);


    updateInterface();
}

void ComponentDialog::removeStockHandler()
{
    int curRow = m_stockTable->currentRow();

    switch(m_mode)
    {
    case ComponentDialog::Add:
        m_stockTable->removeRow(curRow);
        break;
    case ComponentDialog::Edit:
        if(m_stockTable->rowColorHint(curRow) == StockTable::addRowColorHint)
            m_stockTable->removeRow(curRow);
        else
            m_stockTable->setRowColorHint(curRow, StockTable::removeRowColorHint);
        break;
    }


    m_stockTable->clearSelection();
    updateInterface();
}

void ComponentDialog::stockChangedHandler(int row)
{
    if(m_mode == ComponentDialog::Edit)
        if(m_stockTable->rowColorHint(row) == StockTable::defaultRowColorHint)
            m_stockTable->setRowColorHint(row, StockTable::changeRowColorHint);

    m_stockTable->clearSelection();

    updateInterface();
}

void ComponentDialog::updateSecondaryLabel()
{
    ui->secondaryLabel_comboBox->clear();
    QString labelName = ui->primaryLabel_comboBox->currentText();

    if(!labelName.isEmpty())
    {
        Label *top = m_co->findTopLabel(labelName);

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

void ComponentDialog::updateBrowsedDatasheets()
{
    if(ui->source_comboBox->currentIndex() == BrowseFromDisk)
    {
        m_browsedDatasheets.clear();
        for(int row = 0; row < m_datasheetTable->rowCount(); row++)
        {
            QStringList browsed;
            browsed.append(m_datasheetTable->itemText(row, DatasheetTable::TypeColumn));
            browsed.append(m_datasheetTable->itemText(row, DatasheetTable::ManufacturerColumn));
            browsed.append(m_datasheetTable->itemText(row, DatasheetTable::PathColumn));
            if(m_datasheetTable->isDefaultDatasheet(row))
                browsed.append("default");
            else
                browsed.append("");

            m_browsedDatasheets.append(browsed);
        }
    }
}

void ComponentDialog::updateInterface()
{
    int dCurRow = m_datasheetTable->currentRow();

    switch(ui->source_comboBox->currentIndex())
    {
    case ComponentDialog::BrowseDatasheetView:
        ui->datasheet_widget->show();
        ui->component_widget->hide();
        ui->stackedWidget->setCurrentIndex(0);

        if(!ui->filePath_lineEdit->text().isEmpty())
            ui->addDatasheet_toolButton->setEnabled(true);
        else
            ui->addDatasheet_toolButton->setEnabled(false);

        if(!m_datasheetTable->selectedItems().isEmpty())
        {
            ui->editDatasheet_toolButton->setEnabled(true);
            ui->removeDatasheet_toolButton->setEnabled(true);
            if(m_datasheetTable->rowCount() > 1 &&
               m_datasheetTable->rowColorHint(dCurRow) != DatasheetTable::removeRowColorHint)
                ui->setDefaultDatasheet_toolButton->setEnabled(true);
            else
                ui->setDefaultDatasheet_toolButton->setEnabled(false);
        }
        else
        {
            ui->editDatasheet_toolButton->setEnabled(false);
            ui->removeDatasheet_toolButton->setEnabled(false);
            ui->setDefaultDatasheet_toolButton->setEnabled(false);
        }
        break;

    case ComponentDialog::LinkDatasheetView:
        ui->datasheet_widget->hide();
        ui->component_widget->show();
        ui->stackedWidget->setCurrentIndex(1);

        ui->addDatasheet_toolButton->setEnabled(false);
        ui->editDatasheet_toolButton->setEnabled(false);
        ui->removeDatasheet_toolButton->setEnabled(false);
        ui->setDefaultDatasheet_toolButton->setEnabled(false);
        break;
    }

    if(!m_stockTable->selectedItems().isEmpty())
        ui->removePackage_toolButton->setEnabled(true);
    else
        ui->removePackage_toolButton->setEnabled(false);

    ui->package_comboBox->clear();
    foreach(QString name, m_co->packageNames())
    {
        if(!m_stockTable->hasPackage(name))
            ui->package_comboBox->addItem(name);
    }

    if(ui->stock_groupBox->isChecked() && ui->package_comboBox->currentText() != "")
        ui->addPackage_toolButton->setEnabled(true);
    else
        ui->addPackage_toolButton->setEnabled(false);
}
