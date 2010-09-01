/*
 * Scripting support for target manipulations.
 * Copyright (C) 2005-2009 Petr Kubanek <petr@kubanek.net>
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

#ifndef __RTS2_SETARGET__
#define __RTS2_SETARGET__

#include "script.h"
#include "elementacquire.h"
#include "element.h"
#include "../utils/rts2target.h"

namespace rts2script
{

class ElementTarget:public Element
{
	public:
		ElementTarget (Script * in_script, Rts2Target * in_target):Element (in_script)
			{ target = in_target; }
	protected:
		Rts2Target * getTarget () { return target; }
	private:
		Rts2Target * target;
};

class ElementDisable:public ElementTarget
{
	public:
		ElementDisable (Script * in_script, Rts2Target * in_target):ElementTarget (in_script, in_target) {}
		virtual int defnextCommand (Rts2DevClient * client, Rts2Command ** new_command, char new_device[DEVICE_NAME_SIZE]);

		virtual void prettyPrint (std::ostream &os) { os << "disable target"; }
		virtual void printXml (std::ostream &os) { os << "  <disable>"; }
};

class ElementTempDisable:public ElementTarget
{
	public:
		ElementTempDisable (Script * in_script, Rts2Target * in_target, int in_seconds):ElementTarget (in_script, in_target)
			{ seconds = in_seconds; }
		virtual int defnextCommand (Rts2DevClient * client, Rts2Command ** new_command, char new_device[DEVICE_NAME_SIZE]);

		virtual void prettyPrint (std::ostream &os) { os << "temporary disable target for " << seconds << " seconds"; }
		virtual void printXml (std::ostream &os) { os << "  <tempdisable>" << seconds << "</tempdisable>"; }

	private:
		int seconds;
};

class ElementTarBoost:public ElementTarget
{
	public:
		ElementTarBoost (Script * in_script, Rts2Target * in_target, int in_seconds, int in_bonus):ElementTarget (in_script, in_target)
		{
			seconds = in_seconds;
			bonus = in_bonus;
		}
		virtual int defnextCommand (Rts2DevClient * client, Rts2Command ** new_command, char new_device[DEVICE_NAME_SIZE]);
	private:
		int seconds;
		int bonus;
};

}
#endif							 /* !__RTS2_SETARGET__ */
