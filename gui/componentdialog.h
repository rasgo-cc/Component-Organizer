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

#ifndef COMPONENTDIALOG_H
#define COMPONENTDIALOG_H

#include <QDialog>

class CO;
class Component;
class Datasheet;
class DatasheetTable;
class StockTable;
class QLabel;

namespace Ui {
class ComponentDialog;
}

class ComponentDialog : public QDialog
{
    Q_OBJECT
    
public:

    enum Mode {
        Add = 0,
        Edit
    };

    enum DatasheetSourceIndex
    {
        BrowseFromDisk = 0,
        LinkToExistingComponent = 1
    };

    explicit ComponentDialog(CO *co, QWidget *parent = 0);
    explicit ComponentDialog(CO *co, Component *toEdit, QWidget *parent = 0);
    ~ComponentDialog();

    Component* component() { return m_component; }

    void accept();
    
private slots:
    void datasheetSourceChangedHandler();
    void browseFile();
    void addDatasheetHandler();
    void removeDatasheetHandler();
    void setDefaultHandler();
    void addStockHandler();
    void removeStockHandler();
    void stockChangedHandler(int row);
    void updateSecondaryLabel();
    void updateBrowsedDatasheets();
    void updateInterface();

private:

    enum DatasheetSourceView {
        BrowseDatasheetView = 0,
        LinkDatasheetView = 1
    };

    Ui::ComponentDialog *ui;

    DatasheetTable *m_datasheetTable;
    StockTable     *m_stockTable;

    CO *m_co;
    Mode m_mode;
    Component *m_component;
    QList<QStringList> m_browsedDatasheets;

    void setup();
    Datasheet* createDatasheet(int row);
    void createComponent();
    void updateComponent();
    void updateDatasheets();
    void updateStock();
    void updateContainer();
    void updateLabels();

};

#endif // COMPONENTDIALOG_H
