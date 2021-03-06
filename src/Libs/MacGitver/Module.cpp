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

#include "MacGitver/MacGitver.h"
#include "MacGitver/Module.h"

Module::Module()
{
}

IMainWindow* Module::mainWindow()
{
    return MacGitver::self().mainWindow();
}


void Module::repositoryChanged( Git::Repository newRepository )
{
    Q_UNUSED( newRepository );
}

void Module::registerView( const QString& identifier, Heaven::ViewTypes type, ViewCreator* creator )
{
    MacGitver::self().registerView( identifier, type, creator );
}

void Module::unregisterView( const QString& identifier )
{
    MacGitver::self().unregisterView( identifier );
}
