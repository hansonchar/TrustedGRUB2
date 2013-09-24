/* biosdisk.h - emulate biosdisk */
/*
 *  GRUB  --  GRand Unified Bootloader
 *  Copyright (C) 2002,2007  Free Software Foundation, Inc.
 *
 *  GRUB is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  GRUB is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with GRUB.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef GRUB_BIOSDISK_MACHINE_UTIL_HEADER
#define GRUB_BIOSDISK_MACHINE_UTIL_HEADER	1

#include <grub/disk.h>
#include <grub/partition.h>
#include <sys/types.h>

#if defined (__CYGWIN__) || defined (__MINGW32__)
#include <windows.h>
typedef HANDLE grub_util_fd_t;
#define GRUB_UTIL_FD_INVALID INVALID_HANDLE_VALUE
#define GRUB_UTIL_FD_IS_VALID(x) ((x) != GRUB_UTIL_FD_INVALID)
#define GRUB_UTIL_FD_STAT_IS_FUNCTIONAL 0
#else
typedef int grub_util_fd_t;
#define GRUB_UTIL_FD_INVALID -1
#define GRUB_UTIL_FD_IS_VALID(x) ((x) >= 0)
#define GRUB_UTIL_FD_STAT_IS_FUNCTIONAL 1
#endif

grub_util_fd_t
grub_util_fd_open (const char *os_dev, int flags);
const char *
grub_util_fd_strerror (void);
void
grub_util_fd_sync (grub_util_fd_t fd);
void
grub_util_fd_close (grub_util_fd_t fd);

grub_util_fd_t
grub_util_fd_open_device (const grub_disk_t disk, grub_disk_addr_t sector, int flags,
			  grub_disk_addr_t *max);

void grub_util_biosdisk_init (const char *dev_map);
void grub_util_biosdisk_fini (void);
char *grub_util_biosdisk_get_grub_dev (const char *os_dev);
const char *grub_util_biosdisk_get_osdev (grub_disk_t disk);
int grub_util_biosdisk_is_present (const char *name);
int grub_util_biosdisk_is_floppy (grub_disk_t disk);
const char *
grub_util_biosdisk_get_compatibility_hint (grub_disk_t disk);
grub_err_t grub_util_biosdisk_flush (struct grub_disk *disk);
grub_err_t
grub_util_fd_seek (grub_util_fd_t fd, const char *name, grub_uint64_t sector);
ssize_t grub_util_fd_read (grub_util_fd_t fd, char *buf, size_t len);
ssize_t grub_util_fd_write (grub_util_fd_t fd, const char *buf, size_t len);
grub_err_t
grub_cryptodisk_cheat_mount (const char *sourcedev, const char *cheat);
void grub_util_cryptodisk_print_uuid (grub_disk_t disk);
char *
grub_util_get_ldm (grub_disk_t disk, grub_disk_addr_t start);
int
grub_util_is_ldm (grub_disk_t disk);
#ifdef GRUB_UTIL
grub_err_t
grub_util_ldm_embed (struct grub_disk *disk, unsigned int *nsectors,
		     unsigned int max_nsectors,
		     grub_embed_type_t embed_type,
		     grub_disk_addr_t **sectors);
#endif
const char *
grub_hostdisk_os_dev_to_grub_drive (const char *os_dev, int add);

grub_uint64_t
grub_util_get_fd_size (grub_util_fd_t fd, const char *name, unsigned *log_secsize);

char *
grub_util_get_os_disk (const char *os_dev);

#ifdef __linux__
int
grub_hostdisk_linux_find_partition (char *dev, grub_disk_addr_t sector);
#endif

int
grub_util_get_dm_node_linear_info (const char *dev,
				   int *maj, int *min,
				   grub_disk_addr_t *st);


/* Supplied by hostdisk_*.c.  */
grub_int64_t
grub_util_get_fd_size_os (grub_util_fd_t fd, const char *name, unsigned *log_secsize);
/* REturns partition offset in 512B blocks.  */
grub_disk_addr_t
grub_hostdisk_find_partition_start_os (const char *dev);
/* Adjust device driver parameters.  This function should be called just
   after successfully opening the device.  For now, it simply prevents the
   floppy driver from retrying operations on failure, as otherwise the
   driver takes a while to abort when there is no floppy in the drive.
   For now it's non-nop only on NetBSD.
*/
void
grub_hostdisk_configure_device_driver (grub_util_fd_t fd);
void
grub_hostdisk_flush_initial_buffer (const char *os_dev);

#if defined(__FreeBSD__) || defined(__FreeBSD_kernel__) || defined(__APPLE__) || defined(__NetBSD__) || defined (__sun__) || defined(__OpenBSD__)
#define GRUB_DISK_DEVS_ARE_CHAR 1
#else
#define GRUB_DISK_DEVS_ARE_CHAR 0
#endif

struct grub_util_hostdisk_data
{
  char *dev;
  int access_mode;
  grub_util_fd_t fd;
  int is_disk;
  int device_map;
};

#endif /* ! GRUB_BIOSDISK_MACHINE_UTIL_HEADER */
