/*
#             (C) 2008 Hans de Goede <j.w.r.degoede@hhs.nl>

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation; either version 2.1 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU Lesser General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#ifndef __LIBV4L2_H
#define __LIBV4L2_H

#include <stdio.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Point this to a FILE opened for writing when you want to log error and
   status messages to a file, when NULL errors will get send to stderr */
extern FILE *v4l2_log_file;

/* Just like your regular open/close/etc, except that format conversion is
   done if necessary when capturing. That is if you (try to) set a capture
   format which is not supported by the cam, but is supported by libv4lconvert,
   then the try_fmt / set_fmt will succeed as if the cam supports the format
   and on dqbuf / read the data will be converted for you and returned in
   the request format.

   Another difference is that you can make v4l2_read() calls even on devices
   which do not support the regular read() method.

   Note that libv4l2 normally does not interfere with enum_fmt, so enum_fmt
   will still return the actual formats the hardware supports, and not any
   formats which may be emulated on top of that. If you pass the
   V4L2_ENABLE_ENUM_FMT_EMULATION flag to v4l2_fd_open (as the v4l2convert.so
   wrapper does) then enum_fmt will also report support for the formats to
   which conversion is possible.

   Note the device name passed to v4l2_open must be of a video4linux2 device,
   if it is anything else (including a video4linux1 device), v4l2_open will
   fail.
*/

int v4l2_open (const char *file, int oflag, ...);
int v4l2_close(int fd);
int v4l2_dup(int fd);
int v4l2_ioctl (int fd, unsigned long int request, ...);
ssize_t v4l2_read (int fd, void* buffer, size_t n);
void *v4l2_mmap(void *start, size_t length, int prot, int flags, int fd,
  off_t offset);
int v4l2_munmap(void *_start, size_t length);


/* Misc utility functions */

/* This function takes a value of 0 - 65535, and then scales that range to
   the actual range of the given v4l control id, and then if the cid exists
   and is not locked sets the cid to the scaled value.

   Normally returns 0, even if the cid did not exist or was locked, returns
   non 0 when an other error occured. */
int v4l2_set_control(int fd, int cid, int value);

/* This function returns a value of 0 - 65535, scaled to from the actual range
   of the given v4l control id. when the cid does not exist, could not be
   accessed for some reason, or some error occured 0 is returned. */
int v4l2_get_control(int fd, int cid);


/* "low level" access functions, these functions allow somewhat lower level
   access to libv4l2 (currently there only is v4l2_fd_open here) */

/* Flags for v4l2_fd_open's v4l2_flags argument */

/* Disable all format conversion done by libv4l2 (reduces libv4l2 functionality
   to offering v4l2_read() even on devices which don't implement read()) */
#define V4L2_DISABLE_CONVERSION 0x01
/* Report not only real but also emulated formats with the ENUM_FMT ioctl */
#define V4L2_ENABLE_ENUM_FMT_EMULATION 02

/* v4l2_fd_open: open an already opened fd for further use through
   v4l2lib and possibly modify libv4l2's default behavior through the
   v4l2_flags argument.

   Returns fd on success, -1 if the fd is not suitable for use through libv4l2
   (note the fd is left open in this case). */
int v4l2_fd_open(int fd, int v4l2_flags);

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif