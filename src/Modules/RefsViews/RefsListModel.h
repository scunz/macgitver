/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz <sascha@babbelbox.org>
 *
 * This program is free software; you can redistribute it and/or modify it under the terms of the
 * GNU General Public License (Version 2) as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without
 * even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with this program; if
 * not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef MGV_REFS_LIST_MODEL_H
#define MGV_REFS_LIST_MODEL_H

#include <QAbstractListModel>

class RefsListModel : public QAbstractListModel
{
    Q_OBJECT

    struct RefInfo
    {
        QString mRefName;
    };

public:
    RefsListModel( QObject* parent = 0 );
    ~RefsListModel();

public:
    int rowCount( const QModelIndex& parent ) const;
    QVariant data( const QModelIndex& index, int role ) const;

public:
    void clear();
    void updateRef( const QString& refName );
    void addRef( const QString& refName );
    void removeRef( const QString& refName );
    void syncTo( const QStringList& refs );

private:
    QList< RefInfo* >           mOrderedData;
    QHash< QString, RefInfo* >  mData;
};

#endif
