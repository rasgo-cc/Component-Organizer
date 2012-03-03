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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class CO;
class Component;
class ApplicationNote;
class ComponentTable;
class ApplicationNoteTable;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    struct Settings
    {
        bool saveDimensions;
        int  width;
        int  height;
        bool markLowStock;
        bool showContainers;
    };

    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setSettings(Settings settings) { m_settings = settings; }
    Settings settings() { return m_settings; }

protected:
    void closeEvent(QCloseEvent *);

public slots:
    void search(QString searchText);
    void changeView();
    void showAddComponentDialog();
    void showEditComponentDialog(Component *toEdit);
    void showComponentDetailsDialog(Component *component);
    void showAddAppNoteDialog();
    void showEditAppNoteDialog(ApplicationNote *appnote);
    void showOptionsDialog();
    void showInfoMessage(QString msg);
    void about();

private slots:
    void primaryLabelChangedHandler();
    void secondaryLabelChangedHandler();
    
private:
    Ui::MainWindow *ui;

    CO *co;

    ComponentTable       *componentTable;
    ApplicationNoteTable *appnoteTable;

    Settings m_settings;

    void sortyBySelectedLabels();
    void readSettings();
    void updateXML();
};

#endif // MAINWINDOW_H
