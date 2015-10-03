/*
 * This file is part of MultiROM.
 *
 * MultiROM is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * MultiROM is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with MultiROM.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdlib.h>
#include <unistd.h>
#include <cutils/android_reboot.h>
#include <fcntl.h>
#include <stdio.h>
#include <sys/mount.h>
#include <string.h>
#include <errno.h>
#include <time.h>
#include "multirom_ui.h"
#include "multirom.h"
#include "framebuffer.h"
#include "log.h"
#include "version.h"
#include "util.h"

#define EXEC_MASK (S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH)
#define KEEP_REALDATA "/dev/.keep_realdata"
#define REALDATA "/realdata"


static void do_reboot(int exit)
{
  sync();
  ERROR("sync pass \n");
multirom_copy_log(NULL, "../last.txt");

  umount(REALDATA);
  if(exit & EXIT_REBOOT_RECOVERY)         android_reboot(ANDROID_RB_RESTART2, 0, "recovery");
  else if(exit & EXIT_REBOOT_BOOTLOADER)  android_reboot(ANDROID_RB_RESTART2, 0, "bootloader");
  else if(exit & EXIT_SHUTDOWN)           android_reboot(ANDROID_RB_POWEROFF, 0, 0);
  else                                    android_reboot(ANDROID_RB_RESTART, 0, 0);

  while(1);
}

static void do_kexec(void)
{
  emergency_remount_ro();
ERROR("emergency_remount \n");
multirom_copy_log(NULL, "../last.txt");

  execl("/kexec", "/kexec", "-e", NULL);

  ERROR("kexec -e failed! (%d: %s)", errno, strerror(errno));
  while(1);
}

int main(int argc, const char *argv[])
{
  int i;
  const char *rom_to_boot = NULL;

  for(i = 1; i < argc; ++i)
  {
    if(strcmp(argv[i], "-v") == 0)
    {
      printf("%d%s\n", VERSION_MULTIROM, VERSION_DEV_FIX);
      fflush(stdout);
      return 0;
    }
    else if(strncmp(argv[i], "--boot-rom=", sizeof("--boot-rom")) == 0)
    {
      rom_to_boot = argv[i] + sizeof("--boot-rom");
    }
  }

  srand(time(0));
  klog_init();

  // output all messages to dmesg,
  // but it is possible to filter out INFO messages
  klog_set_level(6);

unlink("/realdata/media/0/last.txt");
unlink("/realdata/media/0/last.txt");
unlink("/realdata/media/0/last.txt");



  for(i=0;i<argc;i++){
    ERROR("O_JAE argc %d argv  %s\n",i,argv[i]);
  }

  multirom_copy_log(NULL, "../multirom_first_log.txt");

  ERROR("Running MultiROM v%d%s\n", VERSION_MULTIROM, VERSION_DEV_FIX);

  // root is mounted read only in android and MultiROM uses
  // it to store some temp files, so remount it.
  // Yes, there is better solution to this.
  if(rom_to_boot)
    mount(NULL, "/", NULL, MS_REMOUNT, NULL);


  ERROR("O_JAE rom to boot %s \n",rom_to_boot);
  int exit = multirom(rom_to_boot);

  ERROR("O_JAE exit = %d \n rom to boot %s ",exit,rom_to_boot);


  multirom_copy_log(NULL, "../last.txt");


  if(rom_to_boot){
    mount(NULL, "/", NULL, MS_RDONLY | MS_REMOUNT, NULL);
    ERROR("rom_to_boot is true \n");
    multirom_copy_log(NULL, "../last.txt");

  }
  if(exit >= 0)
  {
    ERROR("exit over the zero %d \n",exit);
    multirom_copy_log(NULL, "../last.txt");

    if(exit & EXIT_REBOOT_MASK)
    {
      ERROR("do_reboot");
      multirom_copy_log(NULL, "../last.txt");

      do_reboot(exit);
      return 0;
    }

    if(exit & EXIT_KEXEC)
    {
      ERROR("do_kexec");
      multirom_copy_log(NULL, "../last.txt");

      do_kexec();
      return 0;
    }

    // indicates trampoline to keep /realdata mounted
    if(!(exit & EXIT_UMOUNT)){
      ERROR("indicates trampoline \n");
      multirom_copy_log(NULL, "../last.txt");

      close(open(KEEP_REALDATA, O_WRONLY | O_CREAT, 0000));
    }
  }
ERROR("ready to vt set mode");
multirom_copy_log(NULL, "../last.txt");

  vt_set_mode(0);
ERROR("finish to vt_set_mode");
multirom_copy_log(NULL, "../last.txt");
  return 0;
}

