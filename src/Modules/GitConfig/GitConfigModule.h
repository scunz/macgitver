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

#ifndef MGV_MODULE_GITCONFIG_H
#define MGV_MODULE_GITCONFIG_H

#include "MacGitver/Module.h"

#include "hic_GitConfigModuleActions.h"

class GitConfigModule : public Module, public GitConfigModuleActions
{
    Q_OBJECT
    Q_PLUGIN_METADATA( IID "org.babbelbox.sacu.macgitver.IModule/0.1" FILE "Module.json" )
    Q_INTERFACES( IModule )

public:
    GitConfigModule();

public:
    void repositoryChanged( Git::Repository newRepository );

    void setupConfigPages( IConfigDialog* dlg );
    Types providesModuleTypes() const;

    void initialize();
    void deinitialize();

private slots:
    void onToolsGitConfig();

private:
    Git::Repository mRepo;
};

#endif
