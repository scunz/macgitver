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

#ifndef HIC_H
#define HIC_H

#include <QCoreApplication>

class QString;
class QTextStream;

class HIDModel;
class HICObject;

class HeavenInterfaceCompiler : public QCoreApplication
{
public:
	HeavenInterfaceCompiler( int argc, char** argv );

public:
	int run();

	bool spitHeader( const HIDModel& model, QTextStream& tsOut );
	bool spitSource( const HIDModel& model, QTextStream& tsOut, const QString& basename );
	void spitSetProperties(QTextStream& tsOut, HICObject* obj,
						   const char* whitespace, const char* prefix );
};

#endif
