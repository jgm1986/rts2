/* 
 * Abstract class for fork mounts.
 * Copyright (C) 2008 Petr Kubanek <petr@kubanek.net>
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

#include "teld.h"

namespace rts2teld
{

/**
 * Abstract fork mount.
 *
 * This class solves various problems related to fork type, either equatorial or 
 * altaz proper modelling.
 *
 * @author Petr Kubanek <petr@kubanek.net>
 */
class Fork: public Telescope
{
	protected:
		/**
		 * Fork parameters, in degrees.
		 */
		double haZero;
		double decZero;

		double haCpd;
		double decCpd;

		int32_t acMin;
		int32_t acMax;

		int acMargin;

		// ticks per revolution
		int32_t ra_ticks;
		int32_t dec_ticks;

		virtual int updateLimits () = 0;

		/**
		 * Gets home offset.
		 */
		virtual int getHomeOffset (int32_t & off) = 0;

		int sky2counts (int32_t & ac, int32_t & dc);
		int sky2counts (struct ln_equ_posn *pos, int32_t & ac, int32_t & dc, double JD, int32_t homeOff);
		int counts2sky (int32_t & ac, int32_t dc, double &ra, double &dec);
	public:
		Fork (int in_argc, char **in_argv);
		virtual ~Fork (void);
};

};
