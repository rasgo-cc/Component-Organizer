/*********************************************************************
Component Organizer
Copyright (C) M�rio Ribeiro (mario.ribas@gmail.com)

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

#include "co.h"
#include "co_defs.h"
#include "component.h"
#include "applicationnote.h"
#include "datasheet.h"
#include "manufacturer.h"
#include "package.h"
#include "container.h"
#include "label.h"
#include "stock.h"

#include <QApplication>
#include <QDesktopServices>
#include <QUrl>
#include <QFile>
#include <QStringList>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QDebug>

CO::CO(QObject *parent) :
    QObject(parent)
{
    m_dirPath = QApplication::applicationDirPath();
}

void CO::useDefaultData()
{
    //TODO before adding default data, remove/delete the current one (if any)

    // Default manufacturers
    foreach(QString name, Manufacturer::defaultNames())
        m_manufacturers.append(new Manufacturer(name, this));

    // Default packages
    foreach(QString name, Package::defaultNames())
        m_packages.append(new Package(name, this));

    // Default labels
    Label *top;

    top = new Label(tr("Transistor"));
    top->addLeaf(new Label(tr("BJT"), top));
    top->addLeaf(new Label(tr("MOS"), top));
    top->addLeaf(new Label(tr("JFET"), top));
    top->addLeaf(new Label(tr("IGBT"), top));
    m_topLabels.append(top);

    top = new Label(tr("Diode"));
    top->addLeaf(new Label(tr("Fast Recovery"), top));
    top->addLeaf(new Label(tr("Zener"), top));
    top->addLeaf(new Label(tr("Schottky"), top));
    top->addLeaf(new Label(tr("Bridge"), top));
    top->addLeaf(new Label(tr("TRIAC"), top));
    top->addLeaf(new Label(tr("DIAC"), top));
    top->addLeaf(new Label(tr("Varicap"), top));
     m_topLabels.append(top);

    top = new Label(tr("Resistor"));
    top->addLeaf(new Label(tr("Carbon Film"), top));
    top->addLeaf(new Label(tr("Metal Film"), top));
    top->addLeaf(new Label(tr("Potentiometer"), top));
    top->addLeaf(new Label(tr("Adjustable"), top));
    top->addLeaf(new Label(tr("Common Pin Network"), top));
    top->addLeaf(new Label(tr("Isolated Network"), top));
    top->addLeaf(new Label(tr("Varistor"), top));
    m_topLabels.append(top);

    top = new Label(tr("Capacitor"));
    top->addLeaf(new Label(tr("Ceramic"), top));
    top->addLeaf(new Label(tr("Electrolytic"), top));
    top->addLeaf(new Label(tr("Aluminium Electrolytic"), top));
    top->addLeaf(new Label(tr("Trimmer"), top));
    top->addLeaf(new Label(tr("Multilayer"), top));
    top->addLeaf(new Label(tr("Tantalum"), top));
    top->addLeaf(new Label(tr("KP / MKP"), top));
    top->addLeaf(new Label(tr("KC / MKC"), top));
    top->addLeaf(new Label(tr("KS / MKS"), top));
    top->addLeaf(new Label(tr("MKV"), top));
    top->addLeaf(new Label(tr("MKT"), top));
    top->addLeaf(new Label(tr("Mica"), top));
    top->addLeaf(new Label(tr("Glass"), top));
    top->addLeaf(new Label(tr("Paper"), top));
    top->addLeaf(new Label(tr("Metalized Paper"), top));
    m_topLabels.append(top);

    top = new Label(tr("Inductor"));
    m_topLabels.append(top);
    
    top = new Label(tr("Optoelectronic"));
    top->addLeaf(new Label(tr("lamp"), top));
    top->addLeaf(new Label(tr("LED"), top));
    top->addLeaf(new Label(tr("Laser"), top));
    top->addLeaf(new Label(tr("Optocoupler"), top));
    m_topLabels.append(top);
    
    top = new Label(tr("Display"));
    top->addLeaf(new Label(tr("OLED"), top));
    top->addLeaf(new Label(tr("LCD"), top));
    top->addLeaf(new Label(tr("7-Segment"), top));
    top->addLeaf(new Label(tr("Bar graph"), top));
    m_topLabels.append(top);
    
    top = new Label(tr("Microcontroller"));
    top->addLeaf(new Label(tr("PIC"), top));
    top->addLeaf(new Label(tr("ATmega"), top));
    top->addLeaf(new Label(tr("ATtiny"), top));
    top->addLeaf(new Label(tr("MSP430"), top));
    top->addLeaf(new Label(tr("ARM"), top));
    m_topLabels.append(top);

    top = new Label(tr("Sensor"));
    top->addLeaf(new Label(tr("Accelerometer"), top));
    top->addLeaf(new Label(tr("Gyroscope"), top));
    top->addLeaf(new Label(tr("Humidity"), top));
    top->addLeaf(new Label(tr("Temperature"), top));
    top->addLeaf(new Label(tr("Pressure"), top));
    top->addLeaf(new Label(tr("Light"), top));
    top->addLeaf(new Label(tr("Current"), top));
    m_topLabels.append(top);

    top = new Label(tr("Data Converter"));
    top->addLeaf(new Label(tr("A/D"), top));
    top->addLeaf(new Label(tr("D/A"), top));
    m_topLabels.append(top);
    
    top = new Label(tr("Signal Conditioner"));
    top->addLeaf(new Label(tr("Op. Amp."), top));
    top->addLeaf(new Label(tr("Comparator"), top));
    top->addLeaf(new Label(tr("Filter"), top));
    top->addLeaf(new Label(tr("Voltage Ref"), top));
    top->addLeaf(new Label(tr("RMS-DC Converter"), top));
    m_topLabels.append(top);

    top = new Label(tr("Interface"));
    top->addLeaf(new Label(tr("USB"), top));
    top->addLeaf(new Label(tr("Firewire"), top));
    top->addLeaf(new Label(tr("RS232"), top));
    top->addLeaf(new Label(tr("I2C"), top));
    top->addLeaf(new Label(tr("1-Wire"), top));
    top->addLeaf(new Label(tr("SPI"), top));
    top->addLeaf(new Label(tr("TWI"), top));
    top->addLeaf(new Label(tr("SIM"), top));
    top->addLeaf(new Label(tr("CAN"), top));
    top->addLeaf(new Label(tr("Wireless/RF"), top));
    m_topLabels.append(top);

    top = new Label(tr("Supply"));
    top->addLeaf(new Label(tr("LDO")));
    top->addLeaf(new Label(tr("Buck-Boost")));
    top->addLeaf(new Label(tr("Buck"), top));
    top->addLeaf(new Label(tr("Boost"), top));
    top->addLeaf(new Label(tr("Battery Charger"), top));
    top->addLeaf(new Label(tr("LED Driver"), top));
    top->addLeaf(new Label(tr("Supply Protection"), top));
    top->addLeaf(new Label(tr("Transformer"), top));
    m_topLabels.append(top);

    top = new Label(tr("Memory"));
    top->addLeaf(new Label(tr("EEPROM"), top));
    top->addLeaf(new Label(tr("SRAM"), top));
    top->addLeaf(new Label(tr("FLASH"), top));
    m_topLabels.append(top);
    
    top = new Label(tr("Relay"));
    top->addLeaf(new Label(tr("Power Relay"), top));
    top->addLeaf(new Label(tr("Signal Relay"), top));
    top->addLeaf(new Label(tr("Solid State"), top));
    top->addLeaf(new Label(tr("High Frequency"), top));
    top->addLeaf(new Label(tr("Analog Switch"), top));
    
    top = new Label(tr("Switch"));
    top->addLeaf(new Label(tr("Slide"), top));
    top->addLeaf(new Label(tr("Lever"), top));
    top->addLeaf(new Label(tr("Rocker"), top));
    top->addLeaf(new Label(tr("Momentary Push"), top));
    top->addLeaf(new Label(tr("Push-lock"), top));
    m_topLabels.append(top);

    top = new Label(tr("Connector"));
    top->addLeaf(new Label(tr("I/O connector"), top));
    top->addLeaf(new Label(tr("Power plug"), top));
    top->addLeaf(new Label(tr("Ribbon"), top));
    top->addLeaf(new Label(tr("Header"), top));
    top->addLeaf(new Label(tr("Screw Terminal"), top));
}

void CO::addManufacturer(Manufacturer *manufacturer)
{
    m_manufacturers.append(manufacturer);
}

void CO::addPackage(Package *package)
{
    m_packages.append(package);
}

void CO::addContainer(Container *container)
{
    m_containers.append(container);
}

void CO::addTopLabel(Label *topLabel)
{
    m_topLabels.append(topLabel);
}

void CO::addComponent(Component *component)
{
    m_components.append(component);
}

void CO::addApplicationNote(ApplicationNote *appnote)
{
    m_appnotes.append(appnote);
}


void CO::removeManufacturer(const QString &name)
{
    for(int i = 0; i < m_manufacturers.count(); i++)
        if(m_manufacturers[i]->name() == name)
        {
            delete m_manufacturers[i];
            m_manufacturers.removeAt(i);
            return;
        }
}

void CO::removePackage(const QString &name)
{
    for(int i = 0; i < m_packages.count(); i++)
        if(m_packages[i]->name() == name)
        {
            delete m_packages[i];
            m_packages.removeAt(i);
            return;
        }
}

void CO::removeContainer(const QString &name)
{
    for(int i = 0; i < m_containers.count(); i++)
    {
        if(m_containers[i]->name() == name)
        {
            delete m_containers[i];
            m_containers.removeAt(i);
            return;
        }
    }
}

void CO::removeLabel(const QString &name)
{
    qDebug() << "lets remove label" << name;
    Label *label = findLabel(name);
    if(label != 0)
    {
        Label *top = label->top();
        if(top == 0)
            m_topLabels.removeOne(label);
        else
            top->removeLeaf(label->name());

        if(!label->leafs().isEmpty())
            foreach(Label *leaf, label->leafs())
                leaf->setTop(0);

        delete label;
    }
}

void CO::removeComponent(Component *component)
{
    foreach(Datasheet *d, component->datasheets())
    {
        removeFile(dirPath() + CO_DATASHEET_PATH + d->path());
        component->removeDatasheet(d);
    }
    m_components.removeOne(component);
    delete component;
}

void CO::removeComponent(const QString &name)
{
    Component *c = findComponent(name);
    removeComponent(c);
}

void CO::removeApplicationNote(ApplicationNote *appnote)
{
    if(!appnote->pdfPath().isEmpty())
        removeFile(dirPath() + CO_APPNOTE_PATH + appnote->pdfPath());
    if(!appnote->attachedFilePath().isEmpty())
        removeFile(dirPath() + CO_APPNOTE_PATH + appnote->attachedFilePath());

    m_appnotes.removeOne(appnote);
    delete appnote;
}

void CO::removeApplicationNote(const QString &description)
{
    ApplicationNote *a = findApplicationNote(description);
    removeApplicationNote(a);
}

Component* CO::findComponent(int ID)
{
    foreach(Component *c, m_components)
        if(c->ID() == ID)
            return c;

    return 0;
}

Component* CO::findComponent(const QString &name)
{
    foreach(Component *c, m_components)
        if(c->name().compare(name) == 0)
            return c;

    return 0;
}

ApplicationNote* CO::findApplicationNote(const QString &description)
{
    foreach(ApplicationNote *a, m_appnotes)
        if(a->description().compare(description) == 0)
            return a;

    return 0;
}

Manufacturer* CO::findManufacturer(const QString &name)
{
    foreach(Manufacturer* m, m_manufacturers)
        if(m->name().compare(name) == 0)
            return m;

    return 0;
}

Package* CO::findPackage(const QString &name)
{
    foreach(Package* p, m_packages)
        if(p->name().compare(name) == 0)
            return p;

    return 0;
}

Container* CO::findContainer(const QString &name)
{
    foreach(Container* c, m_containers)
        if(c->name().compare(name) == 0)
            return c;

    return 0;
}

Label* CO::findTopLabel(const QString &name)
{
    foreach(Label *top, m_topLabels)
        if(top->name().compare(name) == 0)
            return top;

    return 0;
}

Label* CO::findLabel(const QString &name)
{
    foreach(Label *top, m_topLabels)
    {
        if(top->name().compare(name) == 0)
            return top;
        foreach(Label *leaf, top->leafs())
            if(leaf->name().compare(name) == 0)
                return leaf;
    }

    return 0;
}

QStringList CO::componentNames()
{
    QStringList list;
    foreach(Component *c, m_components)
        list.append(c->name());
    list.sort();
    return list;
}



QStringList CO::manufacturerNames()
{
    QStringList list;
    foreach(Manufacturer *m, m_manufacturers)
        list.append(m->name());
    list.sort();

    return list;
}

QStringList CO::packageNames()
{
    QStringList list;
    foreach(Package *p, m_packages)
        list.append(p->name());

    return list;
}

QStringList CO::containerNames()
{
    QStringList list;
    foreach(Container *c, m_containers)
        list.append(c->name());
    list.sort();

    return list;
}

bool CO::execFile(const QString &filePath)
{
    qDebug() << "execFile()" << filePath;
    return QDesktopServices::openUrl(QUrl("file:///" + filePath));
}

bool CO::copyFile(const QString &filePath, const QString &newPath)
{
    qDebug() << "copy" << filePath << "to" << newPath;

    removeFile(newPath);
    return QFile::copy(filePath, newPath);
}

bool CO::removeFile(const QString &filePath)
{
    qDebug() << "remove" << filePath;
    //return QFile::remove(filePath);
    return true;
}

bool CO::writeXML(const QString &filePath)
{
    QFile file(filePath);

    if(!file.open(QIODevice::WriteOnly))
    {
        qDebug() << "Unable to write XML file:" << file.errorString();
        return false;
    }

    qDebug() << "writing XML file:" << filePath;

    QXmlStreamWriter stream(&file);
    stream.setAutoFormatting(true);

    stream.writeStartDocument();

    stream.writeStartElement("comporg");
    stream.writeAttribute("version", CO_VERSION);

    stream.writeStartElement("manufacturers");
    stream.writeAttribute("n", QString::number(m_manufacturers.count()));
    foreach(Manufacturer *m, m_manufacturers)
    {
        stream.writeStartElement("manufacturer");
        stream.writeAttribute("name", m->name());
        stream.writeEndElement(); // </manufacturer>
    }
    stream.writeEndElement(); // </manufacturers>

    stream.writeStartElement("packages");
    stream.writeAttribute("n", QString::number(m_packages.count()));
    foreach(Package *p, m_packages)
    {
        stream.writeStartElement("package");
        stream.writeAttribute("name", p->name());
        stream.writeEndElement(); // </package>
    }
    stream.writeEndElement(); // </packages>

    stream.writeStartElement("containers");
    stream.writeAttribute("n", QString::number(m_containers.count()));
    foreach(Container *c, m_containers)
    {
        stream.writeStartElement("container");
        stream.writeAttribute("name", c->name());
        stream.writeEndElement();
    }
    stream.writeEndElement(); // </containters>

    stream.writeStartElement("labels");
    stream.writeAttribute("ntop", QString::number(m_topLabels.count()));
    stream.writeAttribute("levels", QString::number(2));
    foreach(Label *p, m_topLabels)
    {
        stream.writeStartElement("label");
        stream.writeAttribute("name", p->name());
        stream.writeAttribute("leafs", QString::number(p->leafs().count()));
        foreach(Label *s, p->leafs())
        {
            stream.writeStartElement("label");
            stream.writeAttribute("name", s->name());
            stream.writeAttribute("leafs", QString::number(s->leafs().count()));
            stream.writeEndElement(); // </label>
        }
        stream.writeEndElement(); // </label>
    }
    stream.writeEndElement(); // </labels>

    stream.writeStartElement("components");
    stream.writeAttribute("n", QString::number(m_components.count()));

    foreach(Component *c, m_components)
    {
        stream.writeStartElement("component");

        stream.writeAttribute("name", c->name());
        stream.writeTextElement("description", c->description());

        stream.writeStartElement("datasheets");
        stream.writeAttribute("n", QString::number(c->datasheets().count()));
        stream.writeAttribute("default", QString::number(c->defaultDatasheetIndex()));
        if(c->isLinked())
            stream.writeAttribute("link", c->linkedTo()->name());
        else
            stream.writeAttribute("link", "");
        foreach(Datasheet *d, c->datasheets())
        {
            stream.writeStartElement("datasheet");
            stream.writeAttribute("type", Datasheet::typeToString(d->type()));
            if(d->manufacturer() != 0)
                stream.writeAttribute("manufacturer", d->manufacturer()->name());
            else
                stream.writeAttribute("manufacturer", "");
            stream.writeAttribute("path", d->path());
            stream.writeEndElement(); // </datasheet>
        }
        stream.writeEndElement(); // </datasheets>

        stream.writeStartElement("stocks");
        stream.writeAttribute("n", QString::number(c->stocks().count()));
        stream.writeAttribute("ignore", QString(c->ignoreStock()?"true":"false"));
        foreach(Stock *s, c->stocks())
        {
            stream.writeStartElement("stock");
            stream.writeAttribute("package", s->package()->name());
            stream.writeAttribute("value", QString::number(s->stock()));
            stream.writeAttribute("low", QString::number(s->lowValue()));
            stream.writeEndElement(); // </stock>
        }
        stream.writeEndElement(); // </stocks>

        stream.writeStartElement("container");
        if(c->container() != 0)
            stream.writeAttribute("name", c->container()->name());
        else
            stream.writeAttribute("name", "");
        stream.writeEndElement(); // </container>

        stream.writeStartElement("labels");
        int count=0;
        if(c->primaryLabel() != 0) count++;
        if(c->secondaryLabel() != 0) count++;
        stream.writeAttribute("n", QString::number(count++));
        if(c->primaryLabel() != 0)
        {
            stream.writeStartElement("label");
            stream.writeAttribute("level", QString::number(0));
            stream.writeAttribute("name", c->primaryLabel()->name());
            stream.writeEndElement(); // </plabel>
        }
        if(c->secondaryLabel() != 0)
        {
            stream.writeStartElement("label");
            stream.writeAttribute("level", QString::number(1));
            stream.writeAttribute("name", c->secondaryLabel()->name());
            stream.writeEndElement(); // </slabel>
        }
        stream.writeEndElement(); // </labels>

        stream.writeTextElement("notes", c->notes());

        stream.writeEndElement(); // </component>
    }

    stream.writeEndElement(); // </components>

    stream.writeStartElement("appnotes");
    stream.writeAttribute("n", QString::number(m_appnotes.count()));
    foreach(ApplicationNote *a, m_appnotes)
    {
        stream.writeStartElement("appnote");
        stream.writeAttribute("description", a->description());
        stream.writeAttribute("name", a->name());
        stream.writeAttribute("path", a->pdfPath());
        stream.writeAttribute("attachedFile", a->attachedFilePath());

        stream.writeEndElement(); // </appnote>
    }
    stream.writeEndElement(); // </appnotes>

    stream.writeEndElement(); // </comporg>

    stream.writeEndDocument();

    return true;
}

bool CO::readXML(const QString &filePath)
{
    QFile file(filePath);

    if(!file.open(QIODevice::ReadOnly))
    {
        qDebug() << "Unable to read XML file:" << file.errorString();
        return false;
    }

    QXmlStreamReader xml(&file);

    while(!xml.atEnd())
    {
        xml.readNext();

        switch(xml.tokenType())
        {
        case QXmlStreamReader::StartElement:
            processXmlNode(xml);
            break;
        default: ;
        }
    }

    if(xml.hasError())
    {
        qDebug() << "XML error:" << xml.errorString();
        return false;
    }
    else
    {
        linkDatasheets();
    }

    return true;
}

void CO::processXmlNode(QXmlStreamReader &xml)
{
    int n;
    QString nodeName = xml.name().toString();

    if(nodeName == "manufacturer")
    {
        QString name = xml.attributes().at(0).value().toString();
        Manufacturer *m = new Manufacturer(name);
        qDebug() << name;

        addManufacturer(m);
    }
    else if(nodeName == "package")
    {
        QString name = xml.attributes().at(0).value().toString();
        Package *p = new Package(name);
        qDebug() << name;

        addPackage(p);
    }
    else if(nodeName == "container")
    {
        QString name = xml.attributes().at(0).value().toString();
        Container *c = new Container(name);
        qDebug() << name;

        addContainer(c);
    }
    else if(nodeName == "label") // level=0
    {
        QString pName = xml.attributes().at(0).value().toString();
        Label *pLabel = new Label(pName);
        qDebug() << pName;

        int leafs = xml.attributes().at(1).value().toString().toInt();
        while(leafs-- > 0)
        {
            xml.readNextStartElement(); // level=1

            QString sName = xml.attributes().at(0).value().toString();
            Label *sLabel = new Label(sName);
            qDebug() << sName;
            sLabel->setTop(pLabel);
            pLabel->addLeaf(sLabel);

            xml.skipCurrentElement();
        }

        addTopLabel(pLabel);
    }
    else if(nodeName == "component")
    {
        QString name = xml.attributes().at(0).value().toString();
        Component *c = new Component(name);
        qDebug() << name;

        xml.readNextStartElement(); // description
        c->setDescription(xml.readElementText());

        xml.readNextStartElement(); // datasheets
        qDebug() << xml.name();
        n = xml.attributes().at(0).value().toString().toInt();
        int defaultIndex = xml.attributes().at(1).value().toString().toInt();
        QString link = xml.attributes().at(2).value().toString();
        if(!link.isEmpty())
            m_toLink.insert(c, link);

        while(n-- > 0)
        {
            xml.readNextStartElement(); // datasheet
            qDebug() << xml.name();

            QString type = xml.attributes().at(0).value().toString();
            QString manufacturer = xml.attributes().at(1).value().toString();
            QString path = xml.attributes().at(2).value().toString();
            Datasheet *d = new Datasheet(path);
            d->setType(Datasheet::typeFromString(type));
            d->setManufacturer(findManufacturer(manufacturer));
            c->addDatasheet(d);

            xml.skipCurrentElement();
        }
        c->setDefaultDatasheetIndex(defaultIndex);
        xml.skipCurrentElement();

        xml.readNextStartElement(); // stocks
        qDebug() << xml.name();
        n = xml.attributes().at(0).value().toString().toInt();
        if(xml.attributes().at(1).value().toString() == "true")
            c->setIgnoreStock(true);
        else
            c->setIgnoreStock(false);

        while(n-- > 0)
        {
            xml.readNextStartElement(); // stock
            qDebug() << xml.name();

            QString packageName = xml.attributes().at(0).value().toString();
            int value = xml.attributes().at(1).value().toString().toInt();
            int low = xml.attributes().at(2).value().toString().toInt();
            Stock *s = new Stock(findPackage(packageName));
            s->setStock(value);
            s->setLowValue(low);
            c->addStock(s);
            xml.skipCurrentElement();
        }
        xml.skipCurrentElement();

        xml.readNextStartElement(); // container
        qDebug() << xml.name();
        c->setContainer(findContainer(xml.attributes().at(0).value().toString()));
        xml.skipCurrentElement();

        xml.readNextStartElement(); // labels
        qDebug() << xml.name();
        n = xml.attributes().at(0).value().toString().toInt();
        while(n-- > 0)
        {
            xml.readNextStartElement(); // label
            qDebug() << xml.name();

            int level = xml.attributes().at(0).value().toString().toInt();
            QString labelName = xml.attributes().at(1).value().toString();
            switch(level)
            {
            case 0:
                c->setLabel((Component::LabelLevel)level, findTopLabel(labelName));
                break;
            case 1:
                c->setLabel((Component::LabelLevel)level, findTopLabel(c->primaryLabel()->name())->leaf(labelName));
                break;
            default: ;
            }

            xml.skipCurrentElement();
        }
        xml.skipCurrentElement();

        addComponent(c);
    }
    else if(nodeName == "appnote")
    {
        QString description = xml.attributes().at(0).value().toString();
        QString name = xml.attributes().at(1).value().toString();
        QString pdfPath = xml.attributes().at(2).value().toString();
        QString attachedFilePath = xml.attributes().at(3).value().toString();

        ApplicationNote *a = new ApplicationNote(description);
        a->setName(name);
        a->setPdfPath(pdfPath);
        a->setAttachedFilePath(attachedFilePath);

        addApplicationNote(a);
    }

}

void CO::linkDatasheets()
{
    QMap<Component *, QString>::const_iterator i = m_toLink.constBegin();
    while (i != m_toLink.constEnd())
    {
        Component *c = (Component *) i.key();
        QString name = (QString) i.value();
        c->linkTo(findComponent(name));

        ++i;
    }
}

bool CO::updateDataXML()
{
    return writeXML(dirPath() + CO_XML_PATH);
}
