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

#ifndef MGV_WORKING_TREE_ABSTRACT_ITEM_H
#define MGV_WORKING_TREE_ABSTRACT_ITEM_H

#include "WorkingTreeFilters.h"

class QVariant;
class QString;
class QModelIndex;

class WorkingTreeModel;

class WorkingTreeAbstractItem
{
public:
	WorkingTreeAbstractItem( WorkingTreeModel* model, WorkingTreeAbstractItem* parent );
	virtual ~WorkingTreeAbstractItem();

public:
	virtual QString name() const = 0;
	virtual bool isDirectory() const = 0;
	virtual int visibleChildren() const = 0;
	virtual WorkingTreeAbstractItem* visibleChildAt( int index ) = 0;
	virtual QVariant data( int column, int role ) const = 0;
	virtual int visibleIndex() const = 0;
	virtual WorkingTreeAbstractItem* childByName( const QString& name ) = 0;
	virtual void removeChild( WorkingTreeAbstractItem* child ) = 0;
	bool isVisible() const;

	WorkingTreeAbstractItem* parent();
	QModelIndex index() const;

protected:
	WorkingTreeModel* model();
	void makeVisible();
	void makeInvisible();

private:
	WorkingTreeModel* mModel;
	bool mVisible;
protected:
	WorkingTreeAbstractItem* mParent;
};

#endif
