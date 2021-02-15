/*************************
 *  Copyright (C) 2021
 *  Aleksei Seliverstov
 *************************/

/* FILE NAME   : utils.cpp
 * PURPOSE     : Snake game utilities module.
 * PROGRAMMER  : CSC'2021.
 *               Aleksei Seliverstov.
 * LAST UPDATE : 15.02.21.
 * NOTE        : Namespace 'snake_game'.
 *
 * No part of this file may be changed without agreement of
 * the above-mentioned authors
 */

#ifndef WIN32
#include <termios.h>
/* ioctl documentation https://man7.org/linux/man-pages/man2/ioctl.2.html */
#include <sys/ioctl.h>
/* For the exec family of functions: https://pubs.opengroup.org/onlinepubs/007904875/functions/exec.html */
#include <unistd.h>
#endif

#include <cstdio>
#include <cstdlib>  /* Included for std::system */
#include <ctime>

#include "utils.hpp"

/* Getting keyboard input without blocking in POSIX systems function.
 * This is an alternative to the Windows _kbinit() method.
 * This implementation is sourced from this article:
 * http://www.flipcode.com/archives/_kbhit_for_Linux.shtml
 *
 * ARGUMENTS: None.
 * RETURNS: (bytesWaiting) number of bytes of input waiting to be read
 */
#ifndef WIN32
int snake_game::_kbhit() {
  static const int STDIN = 0;
  static bool initialized = false;
  int bytesWaiting;

  if (!initialized) {
    /* Use termios to turn off line buffering */
    termios term;
    tcgetattr(STDIN, &term);
    term.c_lflag &= static_cast<unsigned long>(~ICANON);  /* Explicit cast from int to unsigned long */
    tcsetattr(STDIN, TCSANOW, &term);
    setbuf(stdin, NULL);
    initialized = true;
  }

  ioctl(STDIN, FIONREAD, &bytesWaiting);
  return bytesWaiting;
}
#endif

/* Cross-platform sleep function
 *
 * ARGUMENTS:
 *   - number of milliseconds to sleep:
 *       unsigned long milliseconds;
 * RETURNS: None.
 */
void snake_game::sleepcp(unsigned long milliseconds) {
  clock_t time_end;
  time_end = clock() + milliseconds * CLOCKS_PER_SEC / 1000;
  while (clock() < time_end) {
  }
}

/* Cross-platform terminal background clear function
 *
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void snake_game::clear_background() {
#ifdef WIN32
  /* Todo: use safer execl */
  std::system("cls");
#else
  /* Assume POSIX */
  std::system("clear");
  //  execl("/usr/bin/clear", "clear");
#endif
}