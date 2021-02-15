/*************************
 *  Copyright (C) 2021
 *  Aleksei Seliverstov
 *************************/

/* FILE NAME   : utils.hpp
 * PURPOSE     : Snake game utilities header module.
 * PROGRAMMER  : CSC'2021.
 *               Aleksei Seliverstov.
 * LAST UPDATE : 15.02.21.
 * NOTE        : Namespace 'snake_game'.
 *
 * No part of this file may be changed without agreement of
 * the above-mentioned authors
 */
#pragma once
#ifndef __KBUTILS_H_
#define __KBUTILS_H_
/* Multiple include guards for cross-platform capabilities */

/* Project namespace */
namespace snake_game {

#ifndef WIN32
/* Getting keyboard input without blocking function.
 * This is an alternative to the Windows _kbinit() method.
 * This implementation is sourced from this article:
 * http://www.flipcode.com/archives/_kbhit_for_Linux.shtml
 *
 * ARGUMENTS: None.
 * RETURNS: (bytesWaiting) number of bytes of input waiting to be read
 */
int _kbhit();
#endif

/* Cross-platform sleep function
 *
 * ARGUMENTS:
 *   - number of milliseconds to sleep:
 *       unsigned long milliseconds;
 * RETURNS: None.
 */
void sleepcp(unsigned long milliseconds);

/* Cross-platform terminal background clear function
 *
 * ARGUMENTS: None.
 * RETURNS: None.
 */
void clear_background();
}

#endif /* __KBUTILS_H_ */