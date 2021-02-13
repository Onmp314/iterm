/* This software is subject to the terms of the Common Public License
   You must accept the terms of this license to use this software.

   Copyright (C) 2002, International Business Machines Corporation
   and others.  All Rights Reserved.

   Further information about Common Public License Version 0.5 is obtained 
   from url http://oss.software.ibm.com/developer/opensource/license-cpl.html */
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "fbiterm.h"		/* for iterm_struct */

void
init_tty ()
{
  struct termios t;
  write(STDOUT_FILENO, "\x1b=",2); /* to switch keypad application modes */

  /* save current parameters */
  tcgetattr (STDIN_FILENO, &(pIterm->ntio));

  /* set new keyboard control parameters for new terminal */
  t = pIterm->ntio;
  t.c_line = 0;
  t.c_lflag &= ~(ECHO | ISIG | ICANON | XCASE);
  t.c_iflag = 0;
  t.c_cflag |= CS8;
  t.c_oflag &= ~OPOST;
  t.c_cc[VMIN] = 1;
  t.c_cc[VTIME] = 0;
  tcsetattr (0, TCSAFLUSH, &t);
  return;
}

void
change_to_text (void)
{
  ioctl (0, KDSETMODE, KD_TEXT);
  return;
}

void
leave_vt ()
{
  pIterm->lock = TRUE;
  signal (SIGUSR1, leave_vt);
  ioctl (0, VT_RELDISP, 1);
}

void
change_to_graphic (void)
{
  ioctl (0, KDSETMODE, KD_GRAPHICS);
  return;
}

void
return_vt ()
{
  struct fb_var_screeninfo vscinfo;
  signal (SIGUSR2, return_vt);

  /* clear all data in frame buffer */
  /* memset (pIterm->fb->buf, 0x0, pIterm->fb->smem_len); */
  ioctl (pIterm->fb->fbfp, FBIOGET_VSCREENINFO, &vscinfo);
  vscinfo.xoffset = vscinfo.yoffset = 0;
  ioctl (pIterm->fb->fbfp, FBIOPAN_DISPLAY, &vscinfo);

  pIterm->lock = FALSE;
  VTCore_redraw (pIterm->vtcore_ptr, 0, 0, pIterm->cols, pIterm->rows);
  ioctl (0, VT_RELDISP, VT_ACKACQ);
}

void
setup_vt_handle (void)
{
  struct vt_mode vtm;

  /* setup vt switch handle */
  ioctl (0, VT_GETMODE, &vtm);

  vtm.mode = VT_PROCESS;
  vtm.relsig = SIGUSR1;
  vtm.acqsig = SIGUSR2;

  signal (SIGUSR1, leave_vt);
  signal (SIGUSR2, return_vt);

  ioctl (0, VT_SETMODE, &vtm);
  return;
}

void
clean_up ()
{
  /* set new keyboard control parameters for new terminal */
  write(STDOUT_FILENO, "\x1b>",2); /* to switch keypad numeric modes */
  tcsetattr (0, TCSAFLUSH, &(pIterm->ntio));
  fb_end (pIterm);
  change_to_text ();
  restore_keys();
  exit (0);
}

/* Initialize tty */
int
InitTerm ()
{
  char *tty_name;
  int console_fd = -1;
  int display_mode;

  if( ((console_fd = open("/dev/console",O_RDWR)) < 0) &&
      ((console_fd = open("/dev/console",O_RDONLY)) < 0) )
  {
    fprintf (stderr, "Can't open /dev/console.\r\n");
    fprintf (stderr, "I do not test if this is graphic mode or not.\r\n");
  }
  else 
  {
    ioctl(console_fd, KDGETMODE, &display_mode);
    if (display_mode != KD_TEXT)
    {
          /*
            if display_mode is not KD_TEXT.  User is trying to run
            fbiterm on X or other framebuffer terminal. So stop starting.
          */
      fprintf (stderr, "This is not text console.\r\n");
      close(console_fd);
      return -1;
    }
    close(console_fd);
  }


  if (!isatty(STDIN_FILENO))
  {
    fprintf (stderr, "This is not a tty.\r\n");
    return -1;
  }
  
  tty_name = ttyname (STDIN_FILENO);
  if (!tty_name)
  {
    fprintf (stderr, "Cannot get current tty name.\r\n");
    return -1;
  }

  if ( (strncmp ("/dev/tty", tty_name, 8) != 0) && 
       (strncmp ("/dev/vc/", tty_name, 8) != 0))
  {
    fprintf (stderr,"This is not real virtual terminal %s.\r\n",tty_name);
    return -1;
  }

  /* examining if /dev/tty[0-9]+. "/dev/tty" may be part of ttyS? or ttyp? */
  if(strlen(tty_name) < 8 || tty_name[8] < '0' || '9' < tty_name[8])
  {
    fprintf (stderr,"This is not real virtual terminal %s.\r\n", tty_name);
    return -1;
  }  

  pIterm->lock = FALSE;
  init_tty ();
  change_to_graphic ();
  setup_vt_handle ();
  return 0;
}
