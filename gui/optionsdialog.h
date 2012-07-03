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

#ifndef OPTIONSDIALOG_H
#define OPTIONSDIALOG_H

#include <QDialog>

class QMainWindow;

class CO;
class pMiniTableWidget;

namespace Ui {
class OptionsDialog;
}

class OptionsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit OptionsDialog(CO *co, QWidget *parent = 0);
    ~OptionsDialog();

protected:
    void accept();

private slots:
    void primaryLabelChangedHandler();
    void addContainerHandler();
    void removeContainerHandler();
    void addPackageHandler();
    void removePackageHandler();
    void addPrimaryLabelHandler();
    void removePrimaryLabelHandler();
    void addSecondaryLabelHandler();
    void removeSecondaryLabelHandler();
    void addManufacturerHandler();
    void removeManufacturerHandler();

    bool message(const QString &msg);

    void updateInterface();
    
private:
    Ui::OptionsDialog *ui;

    pMiniTableWidget *m_containerTable;
    pMiniTableWidget *m_packageTable;
    pMiniTableWidget *m_primaryLabelTable;
    pMiniTableWidget *m_secondaryLabelTable;
    pMiniTableWidget *m_manufacturerTable;

    CO *m_co;

    enum MaxWidth { ColumnMaxWidth = 250 };

    void setup();
};

#endif // OPTIONSDIALOG_H
