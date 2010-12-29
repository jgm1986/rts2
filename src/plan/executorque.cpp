/*
 * Executor body.
 * Copyright (C) 2003-2010 Petr Kubanek <petr@kubanek.net>
 * Copyright (C) 2010      Petr Kubanek, Institute of Physics <kubanek@fzu.cz>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include "executorque.h"

using namespace rts2plan;

ExecutorQueue::ExecutorQueue (Rts2DeviceDb *_master, const char *name)
{
  	master = _master;
	std::string sn (name);
	master->createValue (nextIds, (sn + "_ids").c_str (), "next queue IDs", false);
	master->createValue (nextNames, (sn + "_names").c_str (), "next queue names", false);
}

ExecutorQueue::~ExecutorQueue ()
{
	clearNext ();
	updateVals ();
}

void ExecutorQueue::updateVals ()
{
	std::vector <int> _id_arr;
	std::vector <std::string> _name_arr;
	for (ExecutorQueue::iterator iter = begin (); iter != end (); iter++)
	{
		_id_arr.push_back ((*iter)->getTargetID ());
		_name_arr.push_back ((*iter)->getTargetName ());
		std::cout << (*iter)->getTargetID () << std::endl;
	}
	nextIds->setValueArray (_id_arr);
	nextNames->setValueArray (_name_arr);
	master->sendValueAll (nextIds);
	master->sendValueAll (nextNames);
}

int ExecutorQueue::addFront (rts2db::Target *nt)
{
	push_front (nt);
	updateVals ();
	return 0;
}

int ExecutorQueue::addTarget (rts2db::Target *nt)
{
	push_back (nt);
	updateVals ();
	return 0;
}

void ExecutorQueue::popFront ()
{
	pop_front ();
	updateVals ();
}

void ExecutorQueue::clearNext ()
{
	for (ExecutorQueue::iterator iter = begin (); iter != end (); iter++)
		delete *iter;
	clear ();
}

