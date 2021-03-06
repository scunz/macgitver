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

#ifndef HISTORY_MODEL_H
#define HISTORY_MODEL_H

#include <QList>
#include <QAbstractTableModel>

#include "libGitWrap/Repository.h"

class HistoryEntry;
class HistoryBuilder;

class HistoryModel : public QAbstractTableModel
{
    friend class HistoryBuilder;
    Q_OBJECT
public:
    enum Columns
    {
        colGraph,
        colMessage,
        colAuthor,
        colAuthorDate,
        colCommitter,
        colCommitterDate,
        colSHA1
    };

    enum Modes
    {
        modeFull,
        modeSimple,
        modeFancy
    };

public:
    HistoryModel( QObject* parent = NULL );
    ~HistoryModel();

public:
    int columnMap( int index ) const;

public:
    HistoryEntry* indexToEntry( const QModelIndex& index ) const;
    HistoryEntry* at( int row, bool populate = true ) const;

    int rowCount( const QModelIndex& parent = QModelIndex() ) const;
    int columnCount( const QModelIndex& parent = QModelIndex() ) const;
    QVariant data( const QModelIndex& index, int role ) const;
    QVariant headerData( int section, Qt::Orientation orientation, int role ) const;

public:
    void append( HistoryEntry* entry );
    void updateRow( int row );
    void setRepository( Git::Repository repo );

public slots:
    void ensurePopulated( int row );

private slots:
    void beforeClear();
    void afterClear();
    void beforeAppend();
    void afterAppend();

private:
    Git::Repository         mRepo;
    Modes                   mMode;
    QList< HistoryEntry* >  mEntries;
};

#endif
