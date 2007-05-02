#ifndef __RTS2_MIRROR__
#define __RTS2_MIRROR__

#include "../utils/rts2block.h"
#include "../utils/rts2device.h"

class Rts2DevMirror:public Rts2Device
{
public:
  Rts2DevMirror (int argc, char **argv);
    virtual ~ Rts2DevMirror (void);
  virtual int idle ();

  virtual int startOpen ()
  {
    return maskState (MIRROR_MASK, MIRROR_A_B, "moving A->B");
  }
  virtual int isOpened ()
  {
    return -1;
  }
  virtual int endOpen ()
  {
    return maskState (MIRROR_MASK, MIRROR_B, "moved A->B");
  }

  virtual int startClose ()
  {
    return maskState (MIRROR_MASK, MIRROR_B_A, "moving B->A");
  }
  virtual int isClosed ()
  {
    return -1;
  }
  virtual int endClose ()
  {
    return maskState (MIRROR_MASK, MIRROR_A, "moved B->A");
  }

  // return 1, when mirror is (still) moving
  virtual int isMoving ()
  {
    return ((getState () & MIRROR_MASK) == MIRROR_A_B
	    || (getState () & MIRROR_MASK) == MIRROR_B_A);
  }

  virtual int ready ()
  {
    return -1;
  }

  int startOpen (Rts2Conn * conn);
  int startClose (Rts2Conn * conn);

  virtual int commandAuthorized (Rts2Conn * conn);
};

#endif /* ! __RTS2_MIRROR__ */
