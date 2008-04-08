/* 
 * User management.
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

#include "rts2user.h"
#include "../utils/rts2app.h"
#include "../utils/rts2target.h"

#include <iomanip>

Rts2TypeUser::Rts2TypeUser (char in_type, int in_eventMask)
{
	type = in_type;
	eventMask = in_eventMask;
}


Rts2TypeUser::~Rts2TypeUser (void)
{

}


std::ostream & operator << (std::ostream & _os, Rts2TypeUser & usr)
{
	_os << usr.type << " "
		<< std::hex << std::setw (4) << usr.eventMask << " ";
	printEventMask (usr.eventMask, _os);
	_os << std::endl;
	return _os;
}


int
Rts2TypeUserSet::load (int usr_id)
{
	EXEC SQL BEGIN DECLARE SECTION;
		int db_usr_id = usr_id;
		char db_type;
		int db_eventMask;
	EXEC SQL END DECLARE SECTION;

	EXEC SQL BEGIN TRANSACTION;

	EXEC SQL DECLARE typeuser_cur CURSOR FOR
		SELECT
			type_id,
			event_mask
		FROM
			type_users
		WHERE
			usr_id = :db_usr_id;

	EXEC SQL OPEN typeuser_cur;

	while (1)
	{
		EXEC SQL FETCH next FROM typeuser_cur INTO
				:db_type,
				:db_eventMask;
		if (sqlca.sqlcode)
			break;
		push_back (Rts2TypeUser (db_type, db_eventMask));
	}
	if (sqlca.sqlcode != ECPG_NOT_FOUND)
	{
		logStream (MESSAGE_ERROR) << "Rts2TypeUserSet::load cannot load user set " << sqlca.sqlerrm.sqlerrmc << sendLog;
		EXEC SQL ROLLBACK;
		return -1;
	}
	EXEC SQL ROLLBACK;
	return 0;
}


Rts2TypeUserSet::Rts2TypeUserSet (int usr_id)
{
	load (usr_id);
}


std::ostream & operator << (std::ostream & _os, Rts2TypeUserSet & usrSet)
{
	for (Rts2TypeUserSet::iterator iter = usrSet.begin (); iter != usrSet.end (); iter++)
		_os << "   - " << (*iter);
	return _os;
}


Rts2TypeUserSet::~Rts2TypeUserSet (void)
{

}


Rts2User::Rts2User ()
{
 	id = -1;
	types = NULL;
}


Rts2User::Rts2User (int in_id, std::string in_login, std::string in_email)
{
	id = in_id;
	login = in_login;
	email = in_email;

	types = NULL;
}


Rts2User::~Rts2User (void)
{
	delete types;
}


int
Rts2User::load (const char * in_login)
{
	EXEC SQL BEGIN DECLARE SECTION;
	int db_id;
	VARCHAR db_login[25];
	VARCHAR db_email[200];
	EXEC SQL END DECLARE SECTION;

	if (strlen (in_login) > 25)
	{
		logStream (MESSAGE_ERROR) << "login string '" << in_login << "' too long." << sendLog;
		return -1;
	}

	strcpy (db_login.arr, in_login);
	db_login.len = strlen (in_login);

	EXEC SQL SELECT
		usr_id,
		usr_email
	INTO
		:db_id,
		:db_email
	FROM
		users
	WHERE
		usr_login = :db_login;

	if (sqlca.sqlcode)
	{
		logStream (MESSAGE_ERROR) << "cannot find user with login " << in_login << "." << sendLog;
		id = -1;
		return -1;
	}
	id = db_id;
	login = std::string (in_login);
	email = std::string (db_email.arr);

	return loadTypes ();
}


int
Rts2User::loadTypes ()
{
	delete types;
	types = new Rts2TypeUserSet (id);
	return 0;
}


int
Rts2User::setPassword (std::string newPass)
{
	EXEC SQL BEGIN DECLARE SECTION;
	VARCHAR db_login[25];
	VARCHAR db_passwd[25];
	EXEC SQL END DECLARE SECTION;

	strncpy (db_login.arr, login.c_str (), 25);
	db_login.len = login.length ();

	if (newPass.length () > 25)
	{
		logStream (MESSAGE_ERROR) << "too long password" << sendLog;
		return -1;
	}

	strncpy (db_passwd.arr, newPass.c_str (), 25);
	db_passwd.len = newPass.length ();

	EXEC SQL UPDATE
		users
	SET
		usr_passwd = :db_passwd
	WHERE
		usr_login = :db_login;

	if (sqlca.sqlcode)
	{
		logStream (MESSAGE_ERROR) << "error updating password " << sqlca.sqlerrm.sqlerrmc << sendLog;
		EXEC SQL ROLLBACK;
		return -1;
	}

	EXEC SQL COMMIT;

	return 0;
}


int
Rts2User::setEmail (std::string newEmail)
{
	EXEC SQL BEGIN DECLARE SECTION;
	VARCHAR db_login[25];
	VARCHAR db_email[200];
	EXEC SQL END DECLARE SECTION;

	strncpy (db_login.arr, login.c_str (), 25);
	db_login.len = login.length ();

	if (newEmail.length () > 200)
	{
		logStream (MESSAGE_ERROR) << "too long email" << sendLog;
		return -1;
	}

	strncpy (db_email.arr, newEmail.c_str (), 25);
	db_email.len = newEmail.length ();

	EXEC SQL UPDATE
		users
	SET
		usr_email = :db_email
	WHERE
		usr_login = :db_login;

	if (sqlca.sqlcode)
	{
		logStream (MESSAGE_ERROR) << "error updating email " << sqlca.sqlerrm.sqlerrmc << sendLog;
		EXEC SQL ROLLBACK;
		return -1;
	}

	EXEC SQL COMMIT;

	return 0;
}


std::ostream & operator << (std::ostream & _os, Rts2User &usr)
{
	_os << std::left << std::setw (5) << usr.id
		<< " " << std::setw (25) << usr.login << std::right
		<< " " << usr.email
		<< std::endl;
	// print user set
	_os << (*(usr.types));
	return _os;
}


bool
verifyUser (std::string username, std::string pass)
{
	EXEC SQL BEGIN DECLARE SECTION;
		VARCHAR db_username[25];
		VARCHAR db_pass[25];
		int d_ret;
	EXEC SQL END DECLARE SECTION;
	db_username.len = username.length ();
	db_username.len = db_username.len > 25 ? 25 : db_username.len;
	strncpy (db_username.arr, username.c_str (), db_username.len);

	db_pass.len = pass.length ();
	db_pass.len = db_pass.len > 25 ? 25 : db_pass.len;
	strncpy (db_pass.arr, pass.c_str (), db_pass.len);

	EXEC SQL SELECT
		count (*)
		INTO
			:d_ret
		FROM
			users
		WHERE
			usr_login = :db_username
		AND usr_passwd = :db_pass;

	EXEC SQL COMMIT;

	return d_ret == 1;
}
