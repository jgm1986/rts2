#include "rts2planset.h"
#include "../utils/libnova_cpp.h"

#include <sstream>

void
Rts2PlanSet::load (std::string in_where)
{
  EXEC SQL BEGIN DECLARE SECTION;
  char *stmp_c;

  int db_plan_id;
  EXEC SQL END DECLARE SECTION;

  std::list <int> plan_ids;
  std::ostringstream os;
  int ret;
  
  os << "SELECT "
   "plan_id"
  " FROM "
   "plan";
  if (in_where.length ())
  {
    os << " WHERE "
    << in_where;
  }
  os << " ORDER BY plan_start ASC;";

  stmp_c = new char[strlen (os.str().c_str ()) + 1];
  strcpy (stmp_c, os.str().c_str ());

  EXEC SQL PREPARE plan_stmp FROM :stmp_c;
  
  EXEC SQL DECLARE plan_cur CURSOR FOR plan_stmp;

  EXEC SQL OPEN plan_cur;
  while (1)
  {
    EXEC SQL FETCH next FROM plan_cur INTO
      :db_plan_id;
    if (sqlca.sqlcode)
      break;
    plan_ids.push_back (db_plan_id);
  }
  if (sqlca.sqlcode != ECPG_NOT_FOUND)
  {
    syslog (LOG_ERR, "Rts2PlanSet::load cannot load plan set");
    EXEC SQL CLOSE plan_cur;
    EXEC SQL ROLLBACK;
    delete[] stmp_c;
    return;
  }
  delete[] stmp_c;
  EXEC SQL CLOSE plan_cur;
  EXEC SQL ROLLBACK;

  for (std::list<int>::iterator iter = plan_ids.begin (); iter != plan_ids.end (); iter++)
  {
    Rts2Plan plan = Rts2Plan (*iter);
    ret = plan.load ();
    if (ret)
    {
      syslog (LOG_ERR, "Rts2PlanSet cannot load plan with plan_id %i", *iter);
      continue;
    }
    push_back (plan);
  }
}

Rts2PlanSet::Rts2PlanSet ()
{
  load (std::string (""));
}

Rts2PlanSet::Rts2PlanSet (int prop_id)
{
  std::ostringstream os;

  os << " prop_id = " << prop_id; 
  load (os.str());
}

Rts2PlanSet::Rts2PlanSet (time_t *t_from, time_t *t_to)
{
  std::ostringstream os;

  os << " EXTRACT (EPOCH FROM plan_start) >= " << (*t_from) 
   << " AND EXTRACT (EPOCH FROM plan_start) <= " << (*t_to);
  load (os.str());
}

Rts2PlanSet::~Rts2PlanSet (void)
{
}

std::ostream & operator << (std::ostream & _os, Rts2PlanSet & plan_set)
{
  Rts2PlanSet::iterator plan_iter;
  for (plan_iter = plan_set.begin (); plan_iter != plan_set.end (); plan_iter++)
  {
    _os << (*plan_iter);
  }
  return _os;
}
