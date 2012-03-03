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

#ifndef DATASHEET_H
#define DATASHEET_H

#include <QObject>

class Manufacturer;

class Datasheet : public QObject
{
    Q_OBJECT
public:

    enum Type
    {
        Normal,
        Summary,
        Errata,
        UsersGuide
    };

    static QStringList typeNames();
    static QString typeToString(Type type);
    static Type typeFromString(QString str);

    explicit Datasheet(const QString &path, QObject *parent = 0);

    void setType(Type type) { m_type = type; }
    Type type() { return m_type; }

    void setManufacturer(Manufacturer *manufacturer) { m_manufacturer = manufacturer;}
    Manufacturer* manufacturer() { return m_manufacturer; }

    QString path() { return m_path; }


    
signals:
    
public slots:

private:
    Type m_type;
    Manufacturer *m_manufacturer;
    QString m_path;

    
};

#endif // DATASHEET_H
