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

#ifndef CO_H
#define CO_H

#include <QObject>
#include <QMap>

class Component;
class ApplicationNote;
class Manufacturer;
class Package;
class Container;
class Label;

class QXmlStreamReader;

class CO : public QObject
{
    Q_OBJECT
public:
    explicit CO(QObject *parent = 0);

    QString dirPath() { return m_dirPath; }

    void useDefaultData();

    void addManufacturer(Manufacturer *manufacturer);
    void addPackage(Package *package);
    void addContainer(Container *container);
    void addTopLabel(Label *topLabel);
    void addComponent(Component *component);
    void addApplicationNote(ApplicationNote *appnote);

    void removeManufacturer(const QString &name);
    void removePackage(const QString &name);
    void removeContainer(const QString &name);
    void removeLabel(const QString &name);
    void removeComponent(Component *component);
    void removeComponent(const QString &name);
    void removeApplicationNote(ApplicationNote *appnote);
    void removeApplicationNote(const QString &description);

    QList<Component *> components() { return m_components; }
    QList<ApplicationNote*> applicationNotes() { return m_appnotes; }

    Component* findComponent(int ID);
    Component* findComponent(const QString &name);
    ApplicationNote* findApplicationNote(const QString &description);
    Manufacturer* findManufacturer(const QString &name);
    Package* findPackage(const QString &name);
    Container* findContainer(const QString &name);
    Label* findTopLabel(const QString &name);
    Label* findLabel(const QString &name);

    QStringList componentNames();
    QStringList appnoteNames();
    QStringList manufacturerNames();
    QStringList packageNames();
    QStringList containerNames();

    QList<Label*> topLabels() { return m_topLabels; }

signals:
    
public slots:
    bool execFile(const QString &filePath);
    bool copyFile(const QString &filePath, const QString &newPath);
    bool removeFile(const QString &filePath);
    bool writeXML(const QString &filePath);
    bool readXML(const QString &filePath);
    bool updateDataXML();

private:
    QList<Component *>       m_components;
    QList<ApplicationNote *> m_appnotes;
    QList<Manufacturer *>    m_manufacturers;
    QList<Package *>         m_packages;
    QList<Container *>       m_containers;
    QList<Label*>            m_topLabels;

    QString m_dirPath;

    QMap<Component *, QString> m_toLink;
    void processXmlNode(QXmlStreamReader &xml);
    void linkDatasheets();

    void initLabels();
};

#endif // CO_H
