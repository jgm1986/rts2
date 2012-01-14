/*
 * Script element.
 * Copyright (C) 2005-2008 Petr Kubanek <petr@kubanek.net>
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

#ifndef __RTS2_SCRIPTELEMENT__
#define __RTS2_SCRIPTELEMENT__

#include "operands.h"
#include "rts2spiral.h"
#include "../rts2fits/image.h"
#include "object.h"
#include "block.h"
#include "../rts2db/scriptcommands.h"

#include "status.h"

using namespace rts2core;

#define EVENT_PRECISION_REACHED   RTS2_LOCAL_EVENT + 250

#define EVENT_MIRROR_SET          RTS2_LOCAL_EVENT + 251
#define EVENT_MIRROR_FINISH       RTS2_LOCAL_EVENT + 252

#define EVENT_ACQUIRE_START       RTS2_LOCAL_EVENT + 253
#define EVENT_ACQUIRE_WAIT        RTS2_LOCAL_EVENT + 254
#define EVENT_ACQUIRE_QUERY       RTS2_LOCAL_EVENT + 255

// send some signal to other device..so they will
// know that something is going on
#define EVENT_SIGNAL                RTS2_LOCAL_EVENT + 256

#define EVENT_SIGNAL_QUERY          RTS2_LOCAL_EVENT + 257

// send when data we received
#define EVENT_STAR_DATA             RTS2_LOCAL_EVENT + 258

#define EVENT_ADD_FIXED_OFFSET      RTS2_LOCAL_EVENT + 259

// guiding data available
#define EVENT_GUIDING_DATA          RTS2_LOCAL_EVENT + 260
// ask for acquire state..
#define EVENT_GET_ACQUIRE_STATE     RTS2_LOCAL_EVENT + 261

// change target to new RA DEC, passed as paramentr (struct ln_equ_posn *)
#define EVENT_CHANGE_TARGET         RTS2_LOCAL_EVENT + 262

// change target, start new observation
#define EVENT_NEW_TARGET            RTS2_LOCAL_EVENT + 263

#define EVENT_CHANGE_TARGET_ALTAZ   RTS2_LOCAL_EVENT + 264
#define EVENT_NEW_TARGET_ALTAZ      RTS2_LOCAL_EVENT + 265

namespace rts2script
{

/**
 * Helper class for EVENT_ACQUIRE_QUERY
 */
class AcquireQuery
{
	public:
		int tar_id;
		int count;
		AcquireQuery (int in_tar_id)
		{
			tar_id = in_tar_id;
			count = 0;
		}
};

class Script;

/**
 * This class defines one element in script, which is equal to one command in script.
 *
 * @author Petr Kubanek <petr@kubanek.net>
 */
class Element:public Object
{
	public:
		Element (Script * _script);

		virtual ~ Element (void);

		virtual int defnextCommand (rts2core::DevClient * client, rts2core::Command ** new_command, char new_device[DEVICE_NAME_SIZE]);

		virtual int nextCommand (rts2core::DevClientCamera * camera, rts2core::Command ** new_command, char new_device[DEVICE_NAME_SIZE]);

		virtual int nextCommand (rts2core::DevClientPhot * phot, rts2core::Command ** new_command, char new_device[DEVICE_NAME_SIZE]);

		/**
		 * Called after end of exposure.
		 */
		virtual void exposureEnd () {}

		/**
		 * Query image processing.
		 *
		 * @return -1 if not handled, 0 if basic image processing should be performed, > 0 if image should not be deleted (deletion must be handled Element).
		 */
		virtual int processImage (rts2image::Image * image);

		/**
		 * Returns 1 if we are waiting for that signal.
		 * Signal is > 0
		 */
		virtual int waitForSignal (int in_sig) { return 0; }

		/**
		 * That method will be called when we currently run that
		 * command and we would like to cancel observation.
		 *
		 * Should be used in children when appopriate.
		 */
		virtual void cancelCommands () {}

		/**
		 * This method will be called once after whole script is finished
		 * and before first item in script should be execute
		 */
		virtual void beforeExecuting ()	{}

		virtual int getStartPos ();

		void setLen (int in_len) { len = in_len; }

		virtual int getLen ();

		virtual int idleCall ();

		void setIdleTimeout (double sec);

		/**
		 * called every n-second, defined by setIdleTimeout function. Return
		 * NEXT_COMMAND_KEEP when we should KEEP command or NEXT_COMMAND_NEXT when
		 * script should advance to next command.
		 */
		virtual int idle ();

		virtual void prettyPrint (std::ostream &os) { os << "unknow element"; }
		virtual void printXml (std::ostream &os) {}
		virtual void printScript (std::ostream &os) = 0;
		virtual void printJson (std::ostream &os) {};

		/**
		 * Check tahat element parameters are correct.
		 *
		 * @throw rts2core::Error if some of the parameters is incorrect
		 */
		virtual void checkParameters () {};

		/**
		 * Return expected element duration in seconds.
		 */
		virtual double getExpectedDuration () { return 0; }

		/**
		 * Return expected element duration of light time (shutter opened, taking science data).
		 */
		virtual double getExpectedLightTime () { return 0; }

		/**
		 * Return number of exposures the script is expected to take.
		 */
		virtual int getExpectedImages () { return 0; }

		Script *getScript () { return script; }

	protected:
		Script * script;
		virtual void getDevice (char new_device[DEVICE_NAME_SIZE]);
	private:
		int startPos;
		int len;
		struct timeval nextIdle;
		struct timeval idleTimeout;
};

class ElementNone:public Element
{
	public:
		ElementNone (Script * _script):Element (_script) {}

		virtual void printScript (std::ostream &os) {};
};

class ElementExpose:public Element
{
	public:
		ElementExpose (Script * _script, float in_expTime);
		virtual int nextCommand (rts2core::DevClientCamera * camera, rts2core::Command ** new_command, char new_device[DEVICE_NAME_SIZE]);

		virtual void prettyPrint (std::ostream &os) { os << "exposure " << expTime; }
		virtual void printXml (std::ostream &os) { os << "  <exposure length='" << expTime << "'/>"; }
		virtual void printScript (std::ostream &os) { os << COMMAND_EXPOSURE " " << expTime; }
		virtual void printJson (std::ostream &os) { os << "\"cmd\":\"" << COMMAND_EXPOSURE << "\",\"duration\":" << expTime; }

		virtual double getExpectedDuration ();
		virtual double getExpectedLightTime ();
		virtual int getExpectedImages () { return 1; }
	private:
		float expTime;
		enum {first, SHUTTER, EXPOSURE } callProgress;
};

class ElementDark:public Element
{
	public:
		ElementDark (Script * _script, float in_expTime);
		virtual int nextCommand (rts2core::DevClientCamera * camera, rts2core::Command ** new_command, char new_device[DEVICE_NAME_SIZE]);

		virtual void prettyPrint (std::ostream &os) { os << "dark " << expTime; }
		virtual void printXml (std::ostream &os) { os << "  <dark length='" << expTime << "'/>"; }
		virtual void printScript (std::ostream &os) { os << COMMAND_DARK " " << expTime; }
		virtual void printJson (std::ostream &os) { os << "\"cmd\":\"" COMMAND_DARK "\",\"duration\":" << expTime; }

		virtual double getExpectedDuration ();
		virtual int getExpectedImages () { return 1; }
	private:
		float expTime;
		enum {first, SHUTTER, EXPOSURE } callProgress;
};

class ElementBox:public Element
{
	public:
		ElementBox (Script * _script, int in_x, int in_y, int in_w, int in_h);
		virtual int nextCommand (rts2core::DevClientCamera * camera, rts2core::Command ** new_command, char new_device[DEVICE_NAME_SIZE]);

		virtual void prettyPrint (std::ostream &os) { os << "box " << x << " " << y << " " << w << " " << h; }
		virtual void printScript (std::ostream &os) { os << COMMAND_BOX " " << y << " " << y << " " << w << " " << h; }
	private:
		int x, y, w, h;
};

class ElementCenter:public Element
{
	public:
		ElementCenter (Script * _script, int in_w, int in_h);
		virtual int nextCommand (rts2core::DevClientCamera * camera, rts2core::Command ** new_command, char new_device[DEVICE_NAME_SIZE]);

		virtual void prettyPrint (std::ostream &os) { os << "camera center "; }
		virtual void printScript (std::ostream &os) { os << COMMAND_CENTER; }
	private:
		int w, h;
};

class ElementChange:public Element
{
	public:
		ElementChange (Script * _script, char new_device[DEVICE_NAME_SIZE], double in_ra, double in_dec);
		virtual ~ElementChange (void) { delete [] deviceName; }
		virtual int defnextCommand (rts2core::DevClient * client, rts2core::Command ** new_command, char new_device[DEVICE_NAME_SIZE]);
		void setChangeRaDec (double in_ra, double in_dec)
		{
			ra = in_ra;
			dec = in_dec;
		}

		virtual void prettyPrint (std::ostream &os) { os << "offset " << ra << " " << dec; }
		virtual void printScript (std::ostream &os) { os << COMMAND_CHANGE " " << ra << " " << dec; }
	private:
		char *deviceName;
		double ra;
		double dec;
};

class ElementWait:public Element
{
	public:
		ElementWait (Script * _script);
		virtual int defnextCommand (rts2core::DevClient * client, rts2core::Command ** new_command, char new_device[DEVICE_NAME_SIZE]);

		virtual void prettyPrint (std::ostream &os) { os << "wait "; }
		virtual void printScript (std::ostream &os) { os << COMMAND_WAIT; }
};

class ElementWaitAcquire:public Element
{
	public:
		ElementWaitAcquire (Script * _script, int in_tar_id);
		virtual int defnextCommand (rts2core::DevClient * client, rts2core::Command ** new_command, char new_device[DEVICE_NAME_SIZE]);

		virtual void printScript (std::ostream &os) { os << COMMAND_WAIT_ACQUIRE; }
	private:
		// for which target shall we wait
		int tar_id;

};

class ElementPhotometer:public Element
{
	public:
		ElementPhotometer (Script * _script, int in_filter, float in_exposure, int in_count);
		virtual int nextCommand (rts2core::DevClientPhot * client, rts2core::Command ** new_command, char new_device[DEVICE_NAME_SIZE]);

		virtual void printScript (std::ostream &os) { os << COMMAND_PHOTOMETER << " " << filter << " " << exposure << " " << count; }
	private:
		int filter;
		float exposure;
		int count;

};

class ElementSendSignal:public Element
{
	public:
		ElementSendSignal (Script * _script, int in_sig);
		virtual ~ ElementSendSignal (void);
		virtual void postEvent (rts2core::Event * event);
		virtual int defnextCommand (rts2core::DevClient * client, rts2core::Command ** new_command, char new_device[DEVICE_NAME_SIZE]);

		virtual void printScript (std::ostream &os) { os << COMMAND_SEND_SIGNAL << " " << sig; }
	private:
		int sig;
		bool askedFor;
};

class ElementWaitSignal:public Element
{
	public:
		// in_sig must be > 0
		ElementWaitSignal (Script * _script, int in_sig);
		virtual int defnextCommand (rts2core::DevClient * client, rts2core::Command ** new_command, char new_device[DEVICE_NAME_SIZE]);
		virtual int waitForSignal (int in_sig);

		virtual void printScript (std::ostream &os) { os << COMMAND_WAIT_SIGNAL " " << sig; }
	private:
		int sig;
};

/**
 * Set value.
 */
class ElementChangeValue:public Element
{
	public:
		ElementChangeValue (Script * _script, const char *in_device_value, const char *chng_str);
		virtual ~ ElementChangeValue (void);
		virtual int defnextCommand (rts2core::DevClient * client, rts2core::Command ** new_command, char new_device[DEVICE_NAME_SIZE]);

		virtual void prettyPrint (std::ostream &os) { os << "set on " << deviceName << " value " << valName << " " << op << " " << operands; }
		virtual void printXml (std::ostream &os) { os << "  <set device='" << deviceName << "' value='" << valName << "' op='" << op << "' operands='" << operands << "'/>"; }
		virtual void printScript (std::ostream &os);
		virtual void printJson (std::ostream &os);

		virtual double getExpectedDuration ();

		std::string getOperands ();

	protected:
		virtual void getDevice (char new_device[DEVICE_NAME_SIZE]);
	private:
		char *deviceName;
		std::string valName;
		char op;
		rts2operands::OperandsSet operands;
		bool rawString;
};

/**
 * Class for comment.
 */
class ElementComment:public Element
{
	public:
		ElementComment (Script * _script, const char *in_comment, int in_cnum);
		virtual ~ ElementComment (void);
		virtual int defnextCommand (rts2core::DevClient * client, rts2core::Command ** new_command,	char new_device[DEVICE_NAME_SIZE]);

		virtual void printScript (std::ostream &os) { os << "# " << comment; }
	private:
		char *comment;
		// comment number
		int cnum;
};

}
#endif							 /* !__RTS2_SCRIPTELEMENT__ */
