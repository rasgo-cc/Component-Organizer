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

#ifndef APPLICATIONNOTE_H
#define APPLICATIONNOTE_H

#include <QObject>

class ApplicationNote : public QObject
{
    Q_OBJECT
public:
    explicit ApplicationNote(const QString description, QObject *parent = 0);
    
    void setDescription(const QString &description) { m_description = description; }
    QString description() { return m_description; }

    void setName(const QString &name) { m_name = name; }
    QString name() { return m_name; }

    void setPdfPath(const QString &pdfPath) { m_pdfPath = pdfPath; }
    QString pdfPath() { return m_pdfPath; }

    void setAttachedFilePath(const QString &attachedFilePath) { m_attachedFilePath = attachedFilePath; }
    QString attachedFilePath() { return m_attachedFilePath; }

signals:
    
public slots:

private:
    QString m_description;
    QString m_name;
    QString m_pdfPath;
    QString m_attachedFilePath;
    
};

#endif // APPLICATIONNOTE_H
