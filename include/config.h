/** @file config.h
 *
 * @brief POSIX standard macro definitions.
 *
 * Make sure we satisfy the POSIX standards and define _POSIX_C_SOURCE before
 * any standard header is included.
 *
 * @author David J. Lains (dlains)
 * @bug No known bugs.
 */
#ifndef CONFIG_H
#define CONFIG_H

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#endif // CONFIG_H
