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

#ifndef MGV_MODULE_REPOSITORY_H
#define MGV_MODULE_REPOSITORY_H

#include "MacGitver/Module.h"

#include "Config/ConfigUser.h"

#include "hic_RepositoryModule.h"

class RepositoryCore;

class RepositoryModule : public Module, public RepositoryActions, private ConfigUser
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID "org.babbelbox.sacu.macgitver.IModule/0.1" FILE "Module.json" )
    Q_INTERFACES( IModule )

public:
    RepositoryModule();
    ~RepositoryModule();

public:
    void repositoryChanged( Git::Repository newRepository );

    void setupConfigPages( IConfigDialog* dlg );
    Types providesModuleTypes() const;

    void initialize();
    void deinitialize();

private slots:
    void onRepositoryCreate();
    void onRepositoryClone();
    void onRepositoryOpen();
    /**
     * @brief Helper slot to open a git repository using an OS specific dialog.
     */
    void onRepositoryOpenHelper();
    void onRepositoryClose();

private:
    Git::Repository mRepo;
    RepositoryCore* mCore;
};

#endif
