/*
 * MacGitver
 * Copyright (C) 2012 Sascha Cunz
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

#ifndef GIT_REPOSITORY_H
#define GIT_REPOSITORY_H

#include <QStringList>

#include "Git.h"

namespace Git
{

	class Reference;
	class Object;
	class ObjectCommit;
	class ObjectTag;
	class ObjectBlob;
	class ObjectTree;
	class ObjectId;
	class RevisionWalker;

	class RepositoryPrivate;

	class GITWRAP_API Repository
	{
	public:
		explicit Repository( RepositoryPrivate* _d );
		Repository();
		Repository( const Repository& other );
		Repository& operator=( const Repository& other );

	public:
		~Repository();

	public:
		static Repository create( const QString& path, bool bare );
		static Repository open( const QString& path );

		bool isValid() const;
		bool isBare() const;

		QString basePath() const;

		QStringList allReferences();
		QStringList allBranches();
		QStringList branches( bool local = true, bool remote = false );
		QStringList allTags();

		Index index();

		StatusHash statusHash();

		Reference HEAD();

		Object lookup( const ObjectId& id, ObjectType ot = otAny );
		ObjectCommit lookupCommit( const ObjectId& id );
		ObjectTree lookupTree( const ObjectId& id );
		ObjectBlob lookupBlob( const ObjectId& id );
		ObjectTag lookupTag( const ObjectId& id );

		bool shouldIgnore( const QString& filePath ) const;
		bool shouldIgnore( const QByteArray& filePath ) const;

		RevisionWalker newWalker();

		void test();

	private:
		GitPtr< RepositoryPrivate > d;
	};

}

#endif