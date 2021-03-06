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

#include <QStandardItemModel>

#include "MacGitver/MacGitver.h"

#include "RemotesViewContext.h"

RemotesViewContext::RemotesViewContext()
    : mModel( NULL )
{
    connect( &MacGitver::self(), SIGNAL(repositoryChanged(Git::Repository)),
             this, SLOT(repositoryChanged(Git::Repository)) );

    mModel = new QStandardItemModel( 0, 1, this );

    Git::Repository repo = MacGitver::self().repository();
    if( repo.isValid() )
    {
        repositoryChanged( repo );
    }
}

void RemotesViewContext::repositoryChanged( Git::Repository repo )
{
    mRepo = repo;
}

QAbstractItemModel* RemotesViewContext::model()
{
    return mModel;
}
