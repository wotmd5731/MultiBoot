
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <assert.h>
#include <pthread.h>
#include <dirent.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mount.h>
#include <libbootimg.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/wait.h>
#include <sys/mount.h>
#include <linux/loop.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include <dirent.h>
#include <fcntl.h>
#include "multirom_ui.h"
#include "multirom.h"
#include "animation.h"
#include "framebuffer.h"
#include "input.h"
#include "log.h"
#include "listview.h"
#include "util.h"
#include "button.h"
#include "version.h"
#include "pong.h"
#include "progressdots.h"
#include "multirom_ui_themes.h"
#include "workers.h"
#include "hooks.h"
#include "containers.h"
#include "notification_card.h"
#include "tabview.h"

#define _DOWN_PATH_  "/realdata/media/0/Download/romms"
#define _BB_ "/realdata/media/0/multirom/busybox"
static struct multirom_status *mrom_status = NULL;
static struct multirom_rom *selected_rom = NULL;
static volatile int exit_ui_code = -1;
static volatile int loop_act = 0;
static multirom_themes_info *themes_info = NULL;
static multirom_theme *cur_theme = NULL;

static pthread_mutex_t exit_code_mutex = PTHREAD_MUTEX_INITIALIZER;

fb_img *__img;
fb_rect *__rect; 

int fb_save_screenshot();
int read_directory(const char *path);
int __CALL__INSTALL_BUTTON(struct multirom_status *s);
int rect_touch_check(int start_x, int start_y, int rect_width, int rect_height,int cx, int cy);

int rect_touch(int *cx,int* cy );

static struct auto_boot_data
{
  ncard_builder *b;
  int seconds;
  int destroy;
  pthread_mutex_t mutex;
} auto_boot_data = {
  .b = NULL,
  .seconds = 0,
  .destroy = 0,
  .mutex = PTHREAD_MUTEX_INITIALIZER,
};

uint32_t CLR_PRIMARY = LBLUE;
uint32_t CLR_SECONDARY = LBLUE2;

#define LOOP_UPDATE_USB 0x01
#define LOOP_START_PONG 0x02
#define LOOP_CHANGE_CLR 0x04
#define INTERNAL_ROM_NAME "Internal"

/*****************************************************************************
 *
 *	__JAE__ install function 
 *
 *****************************************************************************/
#if 1
enum
{
  CMPR_GZIP   = 0,
  CMPR_LZ4    = 1,
  CMPR_LZMA   = 2,
};

int decompressRamdisk(const char *src, const char* dest)
{
  FILE *f = fopen(src, "r");
  if(!f)
  {
    ERROR("Failed to open initrd\n");
    return -1;
  }

  char m[4];
  if(fread(m, 1, sizeof(m), f) != sizeof(m))
  {
    ERROR("Failed to read initrd magic\n");
    fclose(f);
    return -1;
  }
  fclose(f);


  ERROR("src:%s dst:%s \n\n ", src,dest);


  //ERROR("test system %d \n",system("ls -al > /realdata/media/0/multirom/ls_test"));



  char cmd[256];
  // gzip
  if(*((uint16_t*)m) == 0x8B1F)
  {
    errno=0;
    ERROR("Ramdisk uses GZIP compression\n");
    //sprintf(cmd, "cd \"%s\" && gzip -d -c \"%s\" | cpio -i", dest, src);
    //system(cmd);
    /*char *pcmd[] = {"/realdata/media/0/multirom/busybox","cd",dest,"&&","gzip","-d","-c",src,"|","cpio","-i",NULL };
      run_cmd(pcmd);		
      return CMPR_GZIP;
     */




    /*
       errno=0;	
       char *cmd2[] = {_BB_,"chmod","777","/realdata/media/0/multirom/sss.sh",NULL };
       run_cmd(cmd2);
       ERROR("sh perm errno %s \n",strerror(errno));



       errno=0;
       char *p1cmd[] = {_BB_,"sh","/realdata/media/0/multirom/sss.sh",NULL };
       run_cmd(p1cmd);
       ERROR("sh errno %s \n",strerror(errno));
       read_directory("/realdata/media/0/multirom/roms/cmm_JAE/boot/");


       errno=0;
       char *p2cmd[] = {"/system/bin/zcat",src,"|","cpio","-idv",NULL };
       run_cmd(p2cmd);
       ERROR("errno %s \n",strerror(errno));
       read_directory("/realdata/media/0/multirom/roms/cmm_JAE/boot/");

       errno=0;
       char *pcmd[] = {_BB_,"zcat",src,"|","cpio","-idv",NULL };
       run_cmd(pcmd);
       ERROR("errno %s \n",strerror(errno));
       read_directory("/realdata/media/0/multirom/roms/cmm_JAE/boot/");
     */

    /*

       read_directory("/realdata/media/0/multirom/roms/cmm_JAE/boot/");
       ERROR("busybox call \n");
       errno=0;	
       char *pcmd[] = {"zcat",src,"|","cpio","-idv",NULL };
       bb_cmd(pcmd);	
       ERROR("errno %s \n",strerror(errno));errno=0;	
       read_directory("/realdata/media/0/multirom/roms/cmm_JAE/boot/");

       ERROR("busybox call zcat \n");
       errno=0;	
       char *pdcmd[] = {"zcat",src,">","/realdata/media/0/multirom/roms/cmm_JAE/boot/in.cpio",NULL };
       bb_cmd(pdcmd);	
       ERROR("errno %s \n",strerror(errno));

       read_directory("/realdata/media/0/multirom/roms/cmm_JAE/boot/");

       ERROR("busybox call cpio\n");
       errno=0;	
       char *dsddcmd[] = {"cpio","-idv","<","/realdata/media/0/multirom/roms/cmm_JAE/boot/in.cpio",NULL };
       bb_cmd(dsddcmd);
       ERROR("errno %s \n",strerror(errno));
       read_directory("/realdata/media/0/multirom/roms/cmm_JAE/boot/");






       ERROR("internal busybox call \n");

    //char *dcmd[] = {"/realdata/media/0/multirom/roms/cmm_JAE/busybox","cd","/realdata/media/0/multirom/roms/cmm_JAE/boot/",NULL };
    //run_cmd(dcmd);

    char *ddcmd[] = {"zcat",src,">","/realdata/media/0/multirom/roms/cmm_JAE/boot/in.cpio",NULL };
    bb_cmd(ddcmd);
    char *dddcmd[] = {"cpio","-idv","<","/realdata/media/0/multirom/roms/cmm_JAE/boot/in.cpio",NULL };
    bb_cmd(dddcmd);
    read_directory("/realdata/media/0/multirom/roms/cmm_JAE/boot/");
     */
    sync();
    return CMPR_GZIP;

  }
  // lz4
  else if(*((uint32_t*)m) == 0x184C2102)
  {
    ERROR("Ramdisk uses LZ4 compression\n");
    //sprintf(cmd, "cd \"%s\" && lz4 -d \"%s\" stdout | cpio -i", dest, src);
    //system(cmd);
    char *pcmd[] = {"/realdata/media/0/multirom/busybox","cd",dest,"&&","lz4","-d",src,"|","cpio","-i",NULL };
    run_cmd(pcmd);
    return CMPR_LZ4;
  }
  // lzma
  else if(*((uint32_t*)m) == 0x0000005D || *((uint32_t*)m) == 0x8000005D)
  {
    ERROR("Ramdisk uses LZMA compression\n");
    sprintf(cmd, "cd \"%s\" && lzma -d -c \"%s\" | cpio -i", dest, src);
    system(cmd);
    return CMPR_LZMA;
  }
  else
    ERROR("Unknown ramdisk compression (%X %X %X %X)\n", m[0], m[1], m[2], m[3]);
  return -1;
}



int extractBootForROM(char* base)
{
  char path[256];
  struct bootimg img;
  char buf[100];


  //bbcmd(p2cmd);
  ERROR("sys xbin su %d \n",system("/system/xbin/su"));
  system("/system/bin/su");
  ERROR("SUSUUSSUSUSUSUSUSUSU\n");






  errno=0;
  sprintf(buf,"%s/boot.img",base);
  ERROR("Extracting contents of boot.img...\n");
  if(libbootimg_init_load(&img, buf, LIBBOOTIMG_LOAD_RAMDISK) < 0)
  {
    ERROR("Failed to load bootimg!\n");
    return -1;
  }

  //system_args("rm -r \"%s/boot/\"*", base.c_str());

  sprintf(buf,"%s/boot/*",base);
  char *pcmd[] = {"/realdata/media/0/multirom/busybox","rm","-r",buf,NULL };
  run_cmd(pcmd);

  sprintf(buf,"%s/boot/initrd.img",base);

  if(libbootimg_dump_ramdisk(&img,buf) < 0)
  {
    ERROR("Failed to dump ramdisk\n");
    libbootimg_destroy(&img);
    return -1;
  }

  libbootimg_destroy(&img);

  //system("rm -r /tmp/boot");
  //system("mkdir /tmp/boot");


  sprintf(buf,"%s/boot/initrd.img",base);
  int rd_cmpr = decompressRamdisk(buf, "/tmp/boot");
  if(rd_cmpr == -1 || access("/tmp/boot/init", F_OK) < 0)
  {
    ERROR("Failed to extract ramdisk!\n");
    return -1;
  }

  ERROR("decompress complete\n");

  // copy needed files
  static const char *cp_f[] = {
    "*.rc", "default.prop", "init", "main_init", "fstab.*",
    // Since Android 4.3 - for SELinux
    "file_contexts", "property_contexts", "seapp_contexts", "sepolicy",
    NULL
  };
  /*
     for(int i = 0; cp_f[i]; ++i){
     system_args("cp -a /tmp/boot/%s \"%s/boot/\"", cp_f[i], base.c_str());

     }

  // check if main_init exists
  sprintf(path, "%s/boot/main_init", base);
  //if(access(path, F_OK) < 0)
  //system_args("mv \"%s/boot/init\" \"%s/boot/main_init\"", base.c_str(), base.c_str());

  //system("rm -r /tmp/boot");
  system_args("cd \"%s/boot\" && rm cmdline ramdisk.gz zImage", base.c_str());

  if (DataManager::GetIntValue("tw_multirom_share_kernel") == 0)
  {
  gui_print("Injecting boot.img..\n");
  if(!injectBoot(base + "/boot.img") != 0)
  return 0;
  }
  else
  system_args("rm \"%s/boot.img\"", base.c_str());
  return 1;
   */
  return 1;
}
#endif

/*************************************************************************
 *
 *		__JAE__CALL_INSTALL
 *
 **************************************************************************/
void __Call_Install(struct multirom_status *s,char *downpath){

  //  mkdir("/tmp",0777);
  //
  //  char *p41cmd[] = {_BB_,"cp","/realdata/tmp/initrd.img","/tmp",NULL };
  //  run_cmd(p41cmd);
  //
  //  char *p411cmd[] = {_BB_,"chmod","777","/tmp/initrd.img",NULL };
  //  run_cmd(p411cmd);
  //

  //  //"cd \"%s\" && gzip -d -c \"%s\" | cpio -i"
  //  pid_t pid;
  //  if(!(pid=vfork())){
  //    int fd = open("/tmp/in.cpio",O_RDWR | O_CREAT, 0777);
  //    dup2(fd,1);
  //    execl(_BB_,_BB_,"gzip","-d","-c","/tmp/initrd.img",NULL);
  //    close(fd);
  //    exit(127);
  //  }else {
  //    int stat =0;
  //    waitpid(pid,&stat,0);
  //  }
  //
  //read_directory("/tmp");
  //
  //  if(!(pid=vfork())){
  //    int fd = open("/tmp/in.cpio",O_RDONLY, 0777);
  //    if(fd<0){
  //      exit(0);
  //    }
  //    dup2(0);
  //    execl(_BB_,_BB_,"cpio","-idv",NULL);
  //    close(fd);
  //    exit(127);
  //  }else {
  //    int stat =0;
  //    waitpid(pid,&stat,0);
  //  }

  char bu[100];
  sprintf(bu,_DOWN_PATH_"/%s",downpath);
  ERROR("downpath %s \n",bu);
  char *cmdd0[] = {_BB_,"tar","-xvzf",bu,"-C","/realdata/media/0/multirom/roms/",NULL};



  pid_t pID = vfork();
  if(pID == 0)
  {      
    execve(cmdd0[0], cmdd0, NULL);
    _exit(127);
  }
  else
  {
    //loading animation


    int i=1;
    int status;
    fb_img * install_animation;
    fb_img * loading = fb_add_text(fb_width/2-60*DPI_MUL, fb_height/2+45*DPI_MUL, 0xFFEAEAEA, SIZE_NORMAL ,"Installing...");;
    fb_rect *install_load_bg = fb_add_rect_lvl(100, 0, 0, fb_width, fb_height, 0xCC070707);
    fb_rect *hide1 = fb_add_rect_lvl(100, fb_width/2-90*DPI_MUL, fb_height/2-90*DPI_MUL, 200*DPI_MUL, 200*DPI_MUL, 0xFF212121);
    fb_rect *hide2 = fb_add_rect_lvl(100, fb_width/2-90*DPI_MUL, fb_height/2-90*DPI_MUL, 200*DPI_MUL, 200*DPI_MUL, 0xCC070707);
    char ch[100];

    fb_fill(0xFF212121);
    draw_refresh(s);
    fb_draw_rect(install_load_bg);
    while(1)
    {
      if(i>11)
        i=1;
      sprintf(ch,"/realdata/media/0/multirom/icons/loading%d.png",i);
      install_animation = fb_add_png_img_lvl(LEVEL_PNG, fb_width/2-70*DPI_MUL, fb_height/2-110*DPI_MUL, 150*DPI_MUL, 150*DPI_MUL, ch);
      fb_draw_img(install_animation);
      fb_draw_img(loading);
      fb_update();

      usleep(50000);

      fb_draw_rect(hide1);
      fb_draw_rect(hide2);
      i++;

      if(waitpid(pID, &status, WNOHANG) == pID) 
      {
        ERROR("waitpid %d \n",i);
        break;
      }

    }
  }







  DIR *dir;
  struct dirent *ent;

  dir = opendir("/realdata/media/0/multirom/roms/");
  char name[20];
  strncpy(name,downpath,4);
  name[4]='_';
  name[5]='\0';

  ERROR("naem make:%s\n",name);

  while((ent=readdir(dir))!=NULL){
    ERROR("ent %s\n",ent->d_name);
    if(strcmp(name,ent->d_name)==0){
      ERROR("find same name\n");
      int len = strlen(name);
      name[len]='A';
      name[len+1]='\0';

      rewinddir(dir);
    }
  }

  ERROR("endof name\n");

  char qwer[40];
  strncpy(qwer,downpath,4);
  char bbu[40];
  sprintf(bbu,"/realdata/media/0/multirom/roms/%s",qwer);
  sprintf(bu,"/realdata/media/0/multirom/roms/%s",name);
  ERROR("bbu %s bu %s \n",bbu,bu);
  char *cmdd1[] ={_BB_,"mv",bbu,bu,NULL};
  run_cmd(cmdd1);
  ERROR("end of run\n");
  sync();
  read_directory("/realdata/media/0/multirom/roms");
  multirom_free_status(s);

  multirom_load_status(s);

 // sleep(1);
  fb_fill(0xFF212121);
  draw_refresh(s);


}


/*************************************************************************
 *
 *     _install_list draw
 *
 *
 **************************************************************************/

int draw_install_list(char** down_ent)
{
  int i,j;
  ERROR("install_list call\n");
  // multirom_copy_log(NULL, "../ins_log.txt");

  //install_popup
  fb_rect *install_popup1 = fb_add_rect_lvl(100, 140*DPI_MUL, 200*DPI_MUL, 500*DPI_MUL, 850*DPI_MUL, 0xFFEAEAEA);
  fb_draw_rect(install_popup1);

  //install_popup_bar (color->gray)
  fb_rect *install_popup_bar = fb_add_rect_lvl(100, 140*DPI_MUL, 200*DPI_MUL, 500*DPI_MUL, 70*DPI_MUL, 0xFF8C8C8C);
  fb_draw_rect(install_popup_bar);

  //plus icon in bar
  fb_img *install_popup_icon = fb_add_png_img_lvl(110, 157*DPI_MUL, 211*DPI_MUL, 50*DPI_MUL, 50*DPI_MUL,"/realdata/media/0/multirom/icons/multi_os_install_black.png");
  fb_draw_img(install_popup_icon);

  //install text
  fb_img * install_popup1_text = fb_add_text(220*DPI_MUL, 230*DPI_MUL, BLACK, SIZE_SMALL ,"OS Install");
  fb_draw_img(install_popup1_text);

  for(i=0,j=0; down_ent[i]!=NULL; i++,j+=70) //draw os list
  {   
    ERROR("i %d\n",i);

    //multirom_copy_log(NULL, "../ins_log.txt");
    //draw rect of os list
    fb_rect *install_os_list = fb_add_rect_lvl(100, 140*DPI_MUL, (275+j)*DPI_MUL, 500*DPI_MUL, 65*DPI_MUL, 0x00BDBDBD);
    fb_draw_rect(install_os_list);

    //draw OS icon of os list
    fb_img *install_icon = fb_add_png_img_lvl(110, 160*DPI_MUL, (280+j)*DPI_MUL, 50*DPI_MUL, 50*DPI_MUL, "/realdata/media/0/multirom/icons/multi_logo.png");
    fb_draw_img(install_icon);

    //draw OS name of os list
    fb_img * install_os_name = fb_add_text(223*DPI_MUL, (300+j)*DPI_MUL, 0xFF212121, SIZE_SMALL ,down_ent[i]);
    fb_draw_img(install_os_name);

    //draw division line(rect) of os list
    fb_rect *install_os_list_rect = fb_add_rect_lvl(100, 150*DPI_MUL, (342+j)*DPI_MUL, 470*DPI_MUL, 1*DPI_MUL, 0xCC353535);
    fb_draw_rect(install_os_list_rect);
    //fb_update();
  }   

  fb_rect *install_back = fb_add_rect_lvl(100, 325*DPI_MUL, 970*DPI_MUL, 130*DPI_MUL, 55*DPI_MUL, 0xFF5D5D5D);
  fb_draw_rect(install_back);
  fb_img * install_back_text = fb_add_text(362*DPI_MUL, 990*DPI_MUL, WHITE, SIZE_SMALL ,"BACK");
  fb_draw_img(install_back_text);


  //fb_update();
  return 0;
}


/************************************************************
 *
 *	__CALL_INSTALL_BUTTON
 *
 **************************************************************/

int __CALL__INSTALL_BUTTON(struct multirom_status *s)
{
  int i,j,cy,cx;
  int cnt2=-1;
  char** down_ent;
  down_ent = (char**)malloc(sizeof(char*)*30);
  for(i=0;i<30;i++){
    down_ent[i]=NULL;
  }
  DIR *dir;
  struct dirent *ent;
  dir = opendir(_DOWN_PATH_ );
  i=0;
  ERROR("into install\n"); 
  //multirom_copy_log(NULL, "../ins_log.txt");

  while((ent=readdir(dir))!=NULL){
    if(i>29)
      break;
    if((strcmp(ent->d_name,".")==0)||(strcmp(ent->d_name,"..")==0))
      continue;
    ERROR("data %c\n",ent->d_name[strlen(ent->d_name)-1]);

    //    if(ent->d_name[strlen(ent->d_name)-1]!='p')
    //      continue;
    //    if(ent->d_name[strlen(ent->d_name)-2]!='i')
    //      continue;
    //    if(ent->d_name[strlen(ent->d_name)-3]!='z')
    //      continue;

    down_ent[i]=(char*)malloc((strlen(ent->d_name)+1)*sizeof(char));
    strcpy(down_ent[i],ent->d_name);
    ERROR("install ent :%s \n",down_ent[i]);

    //  multirom_copy_log(NULL, "../ins_log.txt");

    i++;

  }
  closedir(dir);

  //backgroud color change
  fb_rect *bg1 = fb_add_rect_lvl(100, 0, 0, fb_width, fb_height, 0xDD191919);
  fb_draw_rect(bg1);
  //install call();



  draw_install_list(down_ent);
  fb_update();
  ERROR("fb_update \n");

  //  multirom_copy_log(NULL, "../ins_log.txt");

  while(1)
  {
    cx=0;cy=0;
    rect_touch(&cx,&cy);
    //touch install list

    for(i=0,j=0; down_ent[i]!=NULL; i++,j+=70) 
    {  
      if(rect_touch_check(140*DPI_MUL, (275+j)*DPI_MUL, 500*DPI_MUL, 65*DPI_MUL,cx,cy) == 1) 
      {
        if(cnt2 == i) //second same touch -> enter the selected OS
        {

          __img = fb_add_text(100*DPI_MUL, 200*DPI_MUL, LBLUE, SIZE_BIG  ,"SELECT");
          fb_draw_img(__img);
          fb_update();
          sleep(1);
          __Call_Install(s,down_ent[i]);
          for(i=0; down_ent[i]!=NULL; i++){
            free(down_ent[i]);
          }

          free(down_ent);
          return 0; 
        }
        else if((cnt2 != i) && (cnt2 != -1)) //second diff touch -> change color selected os list button
        {

          draw_install_list(down_ent);

          cnt2 = -1;
        }
        if(cnt2 == -1) //first touch -> change color selected os list button
        {   
          cnt2 = i ;
          fb_rect *install_os_list1 = fb_add_rect_lvl(100, 146*DPI_MUL, (275+j)*DPI_MUL, 489*DPI_MUL, 65*DPI_MUL, 0xFFFAECC5);
          fb_draw_rect(install_os_list1);

          fb_img *install_bg_icon1 = fb_add_png_img_lvl(110, 160*DPI_MUL, (280+j)*DPI_MUL, 50*DPI_MUL, 50*DPI_MUL, "/realdata/media/0/multirom/icons/multi_logo.png");
          fb_draw_img(install_bg_icon1);

          fb_img * install_bg_os_name1 = fb_add_text(223*DPI_MUL, (300+j)*DPI_MUL, 0xFF212121, SIZE_SMALL, down_ent[i]);
          fb_draw_img(install_bg_os_name1);

          fb_img *install_bg_icon2 = fb_add_png_img_lvl(110, 582*DPI_MUL, (286+j)*DPI_MUL, 40*DPI_MUL, 40*DPI_MUL, "/realdata/media/0/multirom/icons/multi_os_install_black.png");
          fb_draw_img(install_bg_icon2);

          fb_update();
        }

      }
    }

    //install -> Back button
    if(rect_touch_check(325*DPI_MUL, 970*DPI_MUL, 130*DPI_MUL, 55*DPI_MUL,cx,cy) == 1)
    {  
      //fb_save_screenshot();
      fb_fill(0xFF212121);  
      draw_refresh(s);
      //down_ent  free

      for(i=0; down_ent[i]!=NULL; i++){
        free(down_ent[i]);
      }
      free(down_ent);

      break;   
    }
  }     
  return 0;
}
/*************************************************************************
 *
 *			_list_block
 *
 **************************************************************************/

static void list_block(char *path, int rec)
{
  ERROR("Listing %s", path);
  DIR *d = opendir(path);
  if(!d)
  {
    ERROR("Failed to open %s", path);
    return;
  }

  struct dirent *dr;
  struct stat info;
  while((dr = readdir(d)))
  {
    if(dr->d_name[0] == '.')
      continue;

    ERROR("%s/%s (%d)", path, dr->d_name, dr->d_type);
    if(dr->d_type == 4 && rec)
    {
      char name[256];
      sprintf(name, "%s/%s", path, dr->d_name);
      list_block(name, 1);
    }
  }

  closedir(d);
}

static void reveal_rect_alpha_step(void *data, float interpolated)
{
  fb_rect *r = data;
  interpolated = 1.f - interpolated;
  r->color = (r->color & ~(0xFF << 24)) | (((int)(0xFF*interpolated)) << 24);
  fb_request_draw();
}

int rect_touch(int *cx,int* cy ){

  if(O_JAE_touch(cx,cy)){
    // ERROR("O_JAE yes touch \n");
  }else{
    // ERROR("O_JAE NO touch\n");
    return 0;
  }


  //touch install button in tab bar, 25*DPI_MUL = (110*DPI_MUL/2-90*DPI_MUL/2)+15*DPI_MUL
  return 1;
}


//int O_JAE_touch(int *x, int *y)  call-> blocking -> touch complete -> return 1 , x y change 
int rect_touch_check(int start_x, int start_y, int rect_width, int rect_height,int cx, int cy)
{

  if((cx>=start_x) && (cx<=(rect_width+start_x)))
  {
    if((cy>=start_y) && (cy<=(rect_height+start_y)))
    {
      return 1;   
    }
  }   
  return 0;

}

int remove_directory(const char *path)
{
  DIR *d = opendir(path);
  size_t path_len = strlen(path);
  int r = -1;
  ERROR("rmdir path:%s\n",path);
  chmod(path,0777);
  if (d)
  {
    struct dirent *p;
    r = 0;
    while (!r && (p=readdir(d)))
    {
      int r2 = -1;
      char *buf;
      size_t len;

      /* Skip the names "." and ".." as we don't want to recurse on them. */
      if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
      {
        continue;
      }

      len = path_len + strlen(p->d_name) + 2; 
      buf = malloc(len);

      if (buf)
      {
        struct stat statbuf;
        snprintf(buf, len, "%s/%s", path, p->d_name);
        ERROR("rm:%s\n",buf);

        if(chmod(buf,0777)==-1){
          ERROR("chmod ERROR\n");
          ERROR("chmod : %s \n",strerror(errno));
        }
        if (!stat(buf, &statbuf))
        {
          if (S_ISDIR(statbuf.st_mode))
          {

            r2 = remove_directory(buf);
            if(r2==-1){
              ERROR("recall rm \n");
              r2 = remove(buf);
              //"ERROR : %d, %s\n",errno,strerror(errno));
              ERROR("rmerror:%s",strerror(errno));
            }
          }
          else
          {

            r2 = unlink(buf);
            if(r2==-1){
              ERROR("recall rm \n");
              r2 = remove(buf);
              //"ERROR : %d, %s\n",errno,strerror(errno));
              ERROR("rmerror:%s",strerror(errno));
            }	
          }
        }
        free(buf);
      }
      r = r2;
    }
    closedir(d);
  }
  if (!r)
  {
    r = rmdir(path);
  }
  return r;
}
int read_directory(const char *path)
{ 
  DIR *dir = opendir(path); // open the current directory
  ERROR("read_dir:%s\n",path);
  if (!dir)
  {
    return -1;
  }

  struct dirent *entry;
  while ((entry = readdir(dir))!=NULL) // notice the single '='
  {
    ERROR("entry:%s\n",entry->d_name);
  }
  closedir(dir);

  return 0;
}

  int
dirUnlinkHierarchy(const char *path)
{
  struct stat st;
  DIR *dir;
  struct dirent *de;
  int fail = 0;

  /* is it a file or directory? */
  if (lstat(path, &st) < 0) {
    return -1;
  }

  /* a file, so unlink it */
  if (!S_ISDIR(st.st_mode)) {
    return unlink(path);
  }

  /* a directory, so open handle */
  dir = opendir(path);
  if (dir == NULL) {
    return -1;
  }

  /* recurse over components */
  errno = 0;
  while ((de = readdir(dir)) != NULL) {
    //TODO: don't blow the stack
    char dn[PATH_MAX];
    if (!strcmp(de->d_name, "..") || !strcmp(de->d_name, ".")) {
      continue;
    }
    snprintf(dn, sizeof(dn), "%s/%s", path, de->d_name);
    if (dirUnlinkHierarchy(dn) < 0) {
      fail = 1;
      break;
    }
    errno = 0;
  }
  /* in case readdir or unlink_recursive failed */
  if (fail || errno < 0) {
    int save = errno;
    closedir(dir);
    errno = save;
    return -1;
  }

  /* close directory handle */
  if (closedir(dir) < 0) {
    return -1;
  }

  /* delete target directory */
  return rmdir(path);
}




void draw_refresh(struct multirom_status *s)
{

  int i=0,j=80;

  //draw rect of tab bar
  fb_rect *tab_bar_rect = fb_add_rect_lvl(LEVEL_RECT, 0, 0, fb_width, 110*DPI_MUL, 0xFFBA942B);
  fb_draw_rect(tab_bar_rect);

  //draw text in tab bar
  fb_img * tab_text = fb_add_text(120*DPI_MUL, 45*DPI_MUL, 0xFFEAEAEA, SIZE_NORMAL ,"ARGOS - Multi Booting System");
  fb_draw_img(tab_text);

  //draw our logo in tab bar
  fb_img *tab_logo = fb_add_png_img_lvl(110, 15*DPI_MUL, 110*DPI_MUL/2 - 90*DPI_MUL/2, 85*DPI_MUL, 85*DPI_MUL, "/realdata/media/0/multirom/icons/multi_logo.png");
  fb_draw_img(tab_logo);

  //draw move button in tab bar
  fb_img *tab_move = fb_add_png_img_lvl(110, fb_width-90*DPI_MUL, (110*DPI_MUL/2 - 90*DPI_MUL/2)+15*DPI_MUL, 50*DPI_MUL, 50*DPI_MUL, "/realdata/media/0/multirom/icons/multi_footstep.png");
  fb_draw_img(tab_move);
  fb_update();

  for(i=0,j=80; s->roms[i]!=NULL; i++,j+=110) //draw os list
  {   
    //draw rect of os list
    fb_rect *os_list = fb_add_rect_lvl(100, 25*DPI_MUL, (45+j)*DPI_MUL, fb_width-40*DPI_MUL, 90*DPI_MUL, 0xFF212121);
    fb_draw_rect(os_list);

    //draw OS icon of os list
    fb_img *bg_icon = fb_add_png_img_lvl(110, 30*DPI_MUL, (50+j)*DPI_MUL, 80*DPI_MUL, 80*DPI_MUL, s->roms[i]->icon_path);
    fb_draw_img(bg_icon);

    //draw OS name of os list
    fb_img * bg_os_name = fb_add_text(140*DPI_MUL, (80+j)*DPI_MUL, 0xFFF6F6F6, SIZE_NORMAL ,s->roms[i]->name);
    fb_draw_img(bg_os_name);


    if(strcmp(s->roms[i]->base_path,"/realdata/media/0/multirom/roms/Internal") != 0)
    {
      //draw OS delete button of os list
      fb_img *tab_delete = fb_add_png_img_lvl(110, fb_width-90*DPI_MUL, (60+j)*DPI_MUL, 50*DPI_MUL, 50*DPI_MUL, "/realdata/media/0/multirom/icons/multi_os_delete2.png");
      fb_draw_img(tab_delete);
    }

    //draw division line(rect) of os list
    fb_rect *os_list_rect = fb_add_rect_lvl(100, 10*DPI_MUL, (141+j)*DPI_MUL, fb_width-20*DPI_MUL, 1*DPI_MUL, 0xCCA6A6A6);
    fb_draw_rect(os_list_rect);
    fb_update();
  }


  //draw install button
  fb_img *tab_install = fb_add_png_img_lvl(110, fb_width-150*DPI_MUL, fb_height-160*DPI_MUL, 100*DPI_MUL, 100*DPI_MUL, "/realdata/media/0/multirom/icons/multi_os_install.png");
  fb_draw_img(tab_install);
  fb_update();
}





int boot_popup(struct multirom_status *s)
{

  fb_rect *boot_popup1 = fb_add_rect_lvl(100, 100*DPI_MUL, 400*DPI_MUL, 520*DPI_MUL, 450*DPI_MUL, 0xFFD9E5FF);
  fb_draw_rect(boot_popup1);

  fb_rect *boot_popup2 = fb_add_rect_lvl(100, 100*DPI_MUL, 400*DPI_MUL, 520*DPI_MUL, 80*DPI_MUL, 0xFFCCA63D);
  fb_draw_rect(boot_popup2);

  fb_img * boot_popup2_text = fb_add_text(117*DPI_MUL, 430*DPI_MUL, WHITE, SIZE_NORMAL ,"  Booting...");
  fb_draw_img(boot_popup2_text);

  fb_img *boot_popup1_icon = fb_add_png_img_lvl(110, 130*DPI_MUL, 500*DPI_MUL, 120*DPI_MUL, 120*DPI_MUL, "/realdata/media/0/multirom/icons/multi_logo.png");
  fb_draw_img(boot_popup1_icon);

  //fb_img * boot_popup1_text = fb_add_text(160*DPI_MUL, 760*DPI_MUL, BLACK, SIZE_NORMAL, "It will boot into Android \nafter 5 seconds");
  //fb_draw_img(boot_popup1_text);


  //select button of boot popup -> YES or NO
  fb_rect *boot_yes = fb_add_rect_lvl(100, 170*DPI_MUL, 646*DPI_MUL, 390*DPI_MUL, 70*DPI_MUL, 0xFFCCA63D);
  fb_draw_rect(boot_yes);
  fb_img * boot_yes_text = fb_add_text(269*DPI_MUL, 667*DPI_MUL, WHITE, SIZE_NORMAL ,"Android Boot");
  fb_draw_img(boot_yes_text);

  fb_rect *boot_no = fb_add_rect_lvl(100, 170*DPI_MUL, 736*DPI_MUL, 390*DPI_MUL, 70*DPI_MUL, 0xFFCCA63D);
  fb_draw_rect(boot_no);      
  fb_img * boot_no_text = fb_add_text(315*DPI_MUL, 756*DPI_MUL, WHITE, SIZE_NORMAL ,"Cancel");
  fb_draw_img(boot_no_text);            
  //fb_update();

  return 0;

}



int boot_popup_touch(struct multirom_status *s)
{
  int i;
  int cx=0,cy=0;

  fb_rect *boot_bg = fb_add_rect_lvl(100, 0, 0, fb_width, fb_height, 0x99191919);
  fb_draw_rect(boot_bg);
  //boot_popup(s);

  while(1)
  {

    for(i=500; i>=0; i--)
    {
      rect_touch(&cx,&cy);
      if(i%100 == 0)
      {
        boot_popup(s);
        fb_img * boot_popup1_text = fb_add_text(270*DPI_MUL, 535*DPI_MUL, BLACK, SIZE_NORMAL, "It will boot into Android \nafter %d seconds",i/100);
        fb_draw_img(boot_popup1_text);
        fb_update();
      }

      //boot button -> YES button
      if(rect_touch_check(170*DPI_MUL, 646*DPI_MUL, 390*DPI_MUL, 70*DPI_MUL,cx,cy) == 1) 
      {
        return 1;
      }

      //boot button -> NO button
      if(rect_touch_check(170*DPI_MUL, 736*DPI_MUL, 390*DPI_MUL, 70*DPI_MUL,cx,cy) == 1)
      { 

        //fb_save_screenshot();
        return 0;
      }
      usleep(10000);


    }    
    break;
  }
  return 1;
}


int multirom_ui(struct multirom_status *s, struct multirom_rom **to_boot)
{

  //-------------------------------------------------------------------------------------------------------
  int i,j;
  char roms_path[256];



  O_JAE_init();
  if(fb_open(0)<0)
  {
    ERROR("GOD_GOONG fb_open error \n");
    return -1;
  }


  /*
     fb_fill(0xFFFFFFFF);
     fb_update();
     int acx=0,acy=0;
     while(1){
     usleep(100000);
     if(rect_touch(&acx,&acy)==1){
     break;
     }
     }
     while(1){
     usleep(100000);
     if(rect_touch(&acx,&acy)==1){
     break;
     }
     }
   *to_boot = s->roms[0];
   return UI_EXIT_BOOT_ROM;
   */




  //intro
  fb_fill(BLACK);
  fb_update();
  sleep(1);

  fb_fill(0xFF212121);
  draw_refresh(s);
  fb_update();

  if(boot_popup_touch(s) == 1)
  {
    *to_boot = s->roms[0];
    return UI_EXIT_BOOT_ROM;
  }
  fb_fill(0xFF212121);
  draw_refresh(s);
  fb_update();

  char move_list[4][30] = {"BOOT LOADER", "RECOVERY", "SHUT DOWN", "REBOOT"};
  int cnt=-1;
  int cnt2=-1;
  int cx=0,cy=0;
first_go:
  while(1)
  {
    int i=0, j=0;
    cx=0,cy=0;
    rect_touch(&cx,&cy);
    //    ERROR("O_JAE touch point x:%d y:%d \n",cx,cy);

    //touch logo button in tab bar
    if(rect_touch_check(15*DPI_MUL, 110*DPI_MUL/2 - 90*DPI_MUL/2, 85*DPI_MUL, 85*DPI_MUL,cx,cy) == 1)
    {
      /*
      if(fb_save_screenshot() == 0)
      {
        //fb_img * screen_text = fb_add_text(fb_width/2, fb_height/2, LBLUE, SIZE_NORMAL ,"Screenshot OK");
        //fb_draw_img(screen_text);
        //fb_update();
        //sleep(1);
        //fb_fill(0xFF212121);
        //draw_refresh(s);
        //fb_update();
      }
      else
      {
        //fb_img * screen_text = fb_add_text(fb_width/2, fb_height/2, DRED, SIZE_NORMAL ,"Screenshot OK");
        //fb_draw_img(screen_text);
        //fb_update();
      }*/
    }


    //touch move button in tab bar
    if(rect_touch_check(fb_width-90*DPI_MUL, (110*DPI_MUL/2 - 90*DPI_MUL/2)+15*DPI_MUL, 62*DPI_MUL, 62*DPI_MUL,cx,cy) == 1)
    {
      //move call();
      //draw move popup
      fb_rect *bg1 = fb_add_rect_lvl(100, 0, 0, fb_width, fb_height, 0xCC191919);
      fb_draw_rect(bg1);

      fb_rect *move_popup1 = fb_add_rect_lvl(100, 140*DPI_MUL, 400*DPI_MUL, 450*DPI_MUL, 550*DPI_MUL, 0xFFEAEAEA);
      fb_draw_rect(move_popup1);

      fb_rect *move_popup2 = fb_add_rect_lvl(100, 140*DPI_MUL, 400*DPI_MUL, 450*DPI_MUL, 70*DPI_MUL, 0xFFF15F5F);
      fb_draw_rect(move_popup2);

      fb_img *move_popup_icon = fb_add_png_img_lvl(110, 157*DPI_MUL, 411*DPI_MUL, 50*DPI_MUL, 50*DPI_MUL,"/realdata/media/0/multirom/icons/multi_footstep.png");
      fb_draw_img(move_popup_icon);

      fb_img * move_popup1_text = fb_add_text(240*DPI_MUL, 425*DPI_MUL, WHITE, SIZE_NORMAL ,"MOVE");
      fb_draw_img(move_popup1_text);

      for(i=0,j=0; i<4; i++,j+=90) //draw os list
      {   
        //draw rect of move list
        fb_rect *os_list = fb_add_rect_lvl(100, 180*DPI_MUL, (490+j)*DPI_MUL, 400*DPI_MUL, 70*DPI_MUL, 0x00FFA7A7);
        fb_draw_rect(os_list);

        //draw name of move list
        fb_img * move_popup1_list = fb_add_text(210*DPI_MUL, (505+j)*DPI_MUL, BLACK, SIZE_NORMAL, "%s", move_list[i]);
        fb_draw_img(move_popup1_list);

        //draw division line(rect) of move list
        fb_rect *move_list_rect = fb_add_rect_lvl(100, 150*DPI_MUL, (565+j)*DPI_MUL, 430*DPI_MUL, 2*DPI_MUL, 0xFFA6A6A6);
        fb_draw_rect(move_list_rect);
        fb_update();
      }                     
      fb_rect *move_no = fb_add_rect_lvl(100, 280*DPI_MUL, 860*DPI_MUL, 160*DPI_MUL, 70*DPI_MUL, 0xFFF15F5F);
      fb_draw_rect(move_no);
      fb_img * move_no_text = fb_add_text(320*DPI_MUL, 885*DPI_MUL, WHITE, SIZE_NORMAL ,"Back");
      fb_draw_img(move_no_text);


      fb_update();


      //touch the select button of move popup
      while(1)
      {
        rect_touch(&cx,&cy);
        //touch move list
        for(i=0,j=0; i<4; i++,j+=90) // raw os list
        {  
          if(rect_touch_check(180*DPI_MUL, (490+j)*DPI_MUL, 400*DPI_MUL, 70*DPI_MUL,cx,cy) == 1) 
          {
            switch(i)
            {								
              case 0:
                *to_boot = 0;
                return UI_EXIT_REBOOT_BOOTLOADER;
                break;
              case 1:
                *to_boot = 0;
                return UI_EXIT_REBOOT_RECOVERY;
                break;
              case 2:
                *to_boot = 0;
                return UI_EXIT_SHUTDOWN;
                break;
              case 3:
                *to_boot = 0;
                return UI_EXIT_REBOOT;
                break;
            }
          }
        }

        //move -> Back button
        if(rect_touch_check(280*DPI_MUL, 860*DPI_MUL, 160*DPI_MUL, 70*DPI_MUL,cx,cy) == 1)
        {  
          fb_fill(0xFF212121);  
          draw_refresh(s);
          cnt=-1;
          break;   
        }
      }


    }


    //touch install button in tab bar
    if(rect_touch_check(fb_width-150*DPI_MUL, fb_height-160*DPI_MUL, 100*DPI_MUL, 100*DPI_MUL,cx,cy) == 1)
    {

      __CALL__INSTALL_BUTTON(s);


    }


    for(i=0,j=80; s->roms[i]!=NULL; i++,j+=110)
    {
      //touch os_list
      if(rect_touch_check(25*DPI_MUL, (45+j)*DPI_MUL, fb_width-140*DPI_MUL, 93*DPI_MUL,cx,cy) == 1)
      {

        if(cnt == i) //second same touch -> enter the selected OS
        {
          *to_boot = s->roms[i];
          return UI_EXIT_BOOT_ROM;
        }
        else if((cnt != i) && (cnt != -1)) //second diff touch -> change color selected os list button
        {
          draw_refresh(s);
          cnt = -1;
        }
        if(cnt == -1) //first touch -> change color selected os list button
        {   
          cnt = i ;
          fb_rect *os_list1 = fb_add_rect_lvl(100, 25*DPI_MUL, (45+j)*DPI_MUL, fb_width-45*DPI_MUL, 90*DPI_MUL, 0xFFC3C3C3);
          fb_draw_rect(os_list1);

          fb_img *bg_icon1 = fb_add_png_img_lvl(110, 30*DPI_MUL, (50+j)*DPI_MUL, 80*DPI_MUL, 80*DPI_MUL, s->roms[i]->icon_path);
          fb_draw_img(bg_icon1);

          fb_img * bg_os_name1 = fb_add_text(140*DPI_MUL, (80+j)*DPI_MUL, BLACK, SIZE_NORMAL ,s->roms[i]->name);
          fb_draw_img(bg_os_name1);

          if(strcmp(s->roms[i]->base_path,"/realdata/media/0/multirom/roms/Internal") != 0)
          {
            fb_img *tab_delete1 = fb_add_png_img_lvl(110, fb_width-90*DPI_MUL, (60+j)*DPI_MUL, 50*DPI_MUL, 50*DPI_MUL, "/realdata/media/0/multirom/icons/multi_os_delete.png");
            fb_draw_img(tab_delete1);
          }
          fb_update();
        }
      }


      //touch delete button in OS list
      if(rect_touch_check(fb_width-90*DPI_MUL, (60+j)*DPI_MUL, 50*DPI_MUL, 50*DPI_MUL,cx,cy) == 1)
      {
        if(strcmp(s->roms[i]->base_path,"/realdata/media/0/multirom/roms/Internal") != 0)
        {
          //draw delete popup
          fb_rect *bg = fb_add_rect_lvl(100, 0, 0, fb_width, fb_height, 0xCC191919);
          fb_draw_rect(bg);

          fb_rect *delete_popup1 = fb_add_rect_lvl(100, 100*DPI_MUL, 400*DPI_MUL, 520*DPI_MUL, 340*DPI_MUL, 0xFFD9E5FF);
          fb_draw_rect(delete_popup1);

          fb_rect *delete_popup2 = fb_add_rect_lvl(100, 100*DPI_MUL, 400*DPI_MUL, 520*DPI_MUL, 70*DPI_MUL, 0xFF6799FF);
          fb_draw_rect(delete_popup2);

          fb_img * delete_popup2_text = fb_add_text(140*DPI_MUL, 425*DPI_MUL, WHITE, SIZE_NORMAL ,"DELETE");
          fb_draw_img(delete_popup2_text);

          fb_img *delete_popup1_icon = fb_add_png_img_lvl(110, 140*DPI_MUL, 510*DPI_MUL, 80*DPI_MUL, 80*DPI_MUL, s->roms[i]->icon_path);
          fb_draw_img(delete_popup1_icon);

          fb_img * delete_popup1_text = fb_add_text(240*DPI_MUL, 530*DPI_MUL, BLACK, SIZE_NORMAL, "Are you sure you want to \ndelete this Platform?");
          fb_draw_img(delete_popup1_text);


          //select button of delete popup -> YES or NO
          fb_rect *delete_yes = fb_add_rect_lvl(100, 190*DPI_MUL, 645*DPI_MUL, 150*DPI_MUL, 60*DPI_MUL, 0xFF6799FF);
          fb_draw_rect(delete_yes);
          fb_img * delete_yes_text = fb_add_text(233*DPI_MUL, 665*DPI_MUL, WHITE, SIZE_NORMAL ,"YES");
          fb_draw_img(delete_yes_text);

          fb_rect *delete_no = fb_add_rect_lvl(100, 410*DPI_MUL, 645*DPI_MUL, 150*DPI_MUL, 60*DPI_MUL, 0xFF6799FF);
          fb_draw_rect(delete_no);      
          fb_img * delete_no_text = fb_add_text(459*DPI_MUL, 665*DPI_MUL, WHITE, SIZE_NORMAL ,"NO");
          fb_draw_img(delete_no_text);            
          fb_update();


          //touch the select button of delete popup
          while(1)
          {
            rect_touch(&cx,&cy);
            //delete button -> YES button
            if(rect_touch_check(190*DPI_MUL, 645*DPI_MUL, 150*DPI_MUL, 60*DPI_MUL,cx,cy) == 1) 
            {
              int eerr= dirUnlinkHierarchy(s->roms[i]->base_path);
              if(eerr==0){
                __img = fb_add_text(100*DPI_MUL, 100*DPI_MUL, LBLUE, SIZE_NORMAL ,"toast : SUCCESS \n");
                multirom_free_rom(s->roms[i]);
                s->roms[i]=NULL;
                multirom_free_status(s);

                multirom_load_status(s);
                multirom_save_status(s);

              }else{
                __img = fb_add_text(100*DPI_MUL, 100*DPI_MUL, LBLUE, SIZE_NORMAL ,"toast : err %d, %s\n",errno,strerror(errno));
              }
              fb_draw_img(__img);            
              fb_update();
              sleep(1);
              fb_fill(0xFF212121);
              draw_refresh(s);
              cnt=-1;
              goto first_go;
              break; 
            }

            //delete button -> NO button
            if(rect_touch_check(410*DPI_MUL, 645*DPI_MUL, 150*DPI_MUL, 60*DPI_MUL,cx,cy) == 1)
            {  
              fb_fill(0xFF212121);  
              draw_refresh(s);
              cnt=-1;
              break;   
            }
          }
        }

      }
    }
  }


  //-------------------------------------------------------------------------------------------------------


}


void multirom_ui_init_theme(int tab)
{
  memset(themes_info->data, 0, sizeof(multirom_theme_data));
  themes_info->data->selected_tab = -1;

  multirom_ui_init_header();
  fb_set_background(C_BACKGROUND);

  themes_info->data->tabs->on_page_changed_by_swipe = multirom_ui_switch;
  themes_info->data->tabs->on_pos_changed = multirom_ui_change_header_selector_pos;

  int i;
  for(i = 0; i < TAB_COUNT; ++i)
  {
    tabview_add_page(themes_info->data->tabs, -1);
    switch(i)
    {
      case TAB_USB:
      case TAB_INTERNAL:
        themes_info->data->tab_data[i] = multirom_ui_tab_rom_init(i);
        break;
      case TAB_MISC:
        themes_info->data->tab_data[i] = multirom_ui_tab_misc_init();
        break;
    }
  }
  add_touch_handler(&tabview_touch_handler, themes_info->data->tabs);
  tabview_update_positions(themes_info->data->tabs);
  multirom_ui_switch(tab);
}

void multirom_ui_destroy_theme(void)
{
  cur_theme->destroy(themes_info->data);

  tabview_destroy(themes_info->data->tabs);
  themes_info->data->tabs = NULL;

  int i;
  for(i = 0; i < TAB_COUNT; ++i)
  {
    button_destroy(themes_info->data->tab_btns[i]);
    themes_info->data->tab_btns[i] = NULL;
    multirom_ui_destroy_tab(i);
  }

  fb_clear();
}

void multirom_ui_init_header(void)
{
  cur_theme->init_header(themes_info->data);
}

void multirom_ui_change_header_selector_pos(float pos)
{
  cur_theme->header_set_tab_selector_pos(themes_info->data, pos);
}

void multirom_ui_destroy_tab(int tab)
{
  /* switch(tab)
     {
     case -1:
     break;
     case TAB_USB:
     case TAB_INTERNAL:
     multirom_ui_tab_rom_destroy(themes_info->data->tab_data[tab]);
     break;
     case TAB_MISC:
     multirom_ui_tab_misc_destroy(themes_info->data->tab_data[tab]);
     break;
     default:
     assert(0);
     break;
     }*/
  themes_info->data->tab_data[tab] = NULL;
}

void multirom_ui_switch(int tab)
{
  if(tab == themes_info->data->selected_tab)
    return;

  tabview_set_active_page(themes_info->data->tabs, tab,
      themes_info->data->selected_tab == -1 ? 0 : 200);
  themes_info->data->selected_tab = tab;
}

void multirom_ui_fill_rom_list(listview *view, int mask)
{
  int i;
  struct multirom_rom *rom;
  void *data;
  char part_desc[64];
  for(i = 0; mrom_status->roms && mrom_status->roms[i]; ++i)
  {
    rom = mrom_status->roms[i];

    if(!(M(rom->type) & mask))
      continue;

    if(rom->partition)
      sprintf(part_desc, "%s (%s)", rom->partition->name, rom->partition->fs);

    if(rom->type == ROM_DEFAULT && mrom_status->hide_internal)
      continue;

    data = rom_item_create(rom->name, rom->partition ? part_desc : NULL, rom->icon_path);
    listview_add_item(view, rom->id, data);
  }
}

static void multirom_ui_destroy_auto_boot_data(void)
{
  if(auto_boot_data.b)
  {
    ncard_destroy_builder(auto_boot_data.b);
    auto_boot_data.b = NULL;
  }
  auto_boot_data.destroy = 1;
}

static void multirom_ui_auto_boot_hidden(void *data)
{
  pthread_mutex_lock(&auto_boot_data.mutex);
  multirom_ui_destroy_auto_boot_data();
  pthread_mutex_unlock(&auto_boot_data.mutex);
}

static void multirom_ui_auto_boot_now(void *data)
{
  multirom_ui_auto_boot_hidden(data);

  pthread_mutex_lock(&exit_code_mutex);
  selected_rom = mrom_status->auto_boot_rom;
  exit_ui_code = UI_EXIT_BOOT_ROM;
  pthread_mutex_unlock(&exit_code_mutex);
}

static void multirom_ui_auto_boot_tick(void *data)
{
  char buff[128];

  pthread_mutex_lock(&auto_boot_data.mutex);

  if(auto_boot_data.destroy)
  {
    pthread_mutex_unlock(&auto_boot_data.mutex);
    return;
  }

  if(--auto_boot_data.seconds == 0)
  {
    multirom_ui_destroy_auto_boot_data();
    pthread_mutex_unlock(&auto_boot_data.mutex);

    pthread_mutex_lock(&exit_code_mutex);
    selected_rom = mrom_status->auto_boot_rom;
    exit_ui_code = UI_EXIT_BOOT_ROM;
    pthread_mutex_unlock(&exit_code_mutex);
  }
  else
  {
    call_anim *a = call_anim_create(NULL, NULL, 1000, INTERPOLATOR_LINEAR);
    a->duration = 1000; // in call_anim_create, duration is multiplied by coef - we don't want that here
    a->on_finished_call = multirom_ui_auto_boot_tick;
    call_anim_add(a);

    snprintf(buff, sizeof(buff), "\n<b>ROM:</b> <y>%s</y>\n\nBooting in %d second%s.",
        mrom_status->auto_boot_rom->name, auto_boot_data.seconds, auto_boot_data.seconds != 1 ? "s" : "");
    ncard_set_text(auto_boot_data.b, buff);
    ncard_show(auto_boot_data.b, 0);
  }

  pthread_mutex_unlock(&auto_boot_data.mutex);
}

void multirom_ui_auto_boot(void)
{
  ncard_builder *b = ncard_create_builder();
  auto_boot_data.b = b;
  auto_boot_data.seconds = mrom_status->auto_boot_seconds + 1;
  auto_boot_data.destroy = 0;

  ncard_set_pos(b, NCARD_POS_CENTER);
  ncard_set_cancelable(b, 1);
  ncard_set_title(b, "Auto-boot");
  ncard_add_btn(b, BTN_NEGATIVE, "Cancel", ncard_hide_callback, NULL);
  ncard_add_btn(b, BTN_POSITIVE, "Boot now", multirom_ui_auto_boot_now, NULL);
  ncard_set_on_hidden(b, multirom_ui_auto_boot_hidden, NULL);
  ncard_set_from_black(b, 1);

  multirom_ui_auto_boot_tick(NULL);
}

void multirom_ui_refresh_usb_handler(void)
{
  pthread_mutex_lock(&exit_code_mutex);
  loop_act |= LOOP_UPDATE_USB;
  pthread_mutex_unlock(&exit_code_mutex);
}

void multirom_ui_start_pong(int action)
{
  pthread_mutex_lock(&exit_code_mutex);
  loop_act |= LOOP_START_PONG;
  pthread_mutex_unlock(&exit_code_mutex);
}

void *multirom_ui_tab_rom_init(int tab_type)
{
  tab_data_roms *t = mzalloc(sizeof(tab_data_roms));
  themes_info->data->tab_data[tab_type] = t;

  t->list = mzalloc(sizeof(listview));
  t->list->item_draw = &rom_item_draw;
  t->list->item_hide = &rom_item_hide;
  t->list->item_height = &rom_item_height;
  t->list->item_destroy = &rom_item_destroy;
  t->list->item_confirmed = &multirom_ui_tab_rom_confirmed;

  cur_theme->tab_rom_init(themes_info->data, t, tab_type);

  listview_init_ui(t->list);
  tabview_add_item(themes_info->data->tabs, tab_type, t->list);

  if(tab_type == TAB_INTERNAL)
    multirom_ui_fill_rom_list(t->list, MASK_INTERNAL);

  listview_update_ui(t->list);

  int has_roms = (int)(t->list->items == NULL);
  multirom_ui_tab_rom_set_empty((void*)t, has_roms);

  if(tab_type == TAB_USB)
  {
    multirom_set_usb_refresh_handler(&multirom_ui_refresh_usb_handler);
    multirom_set_usb_refresh_thread(mrom_status, 1);
  }
  return t;
}

void multirom_ui_tab_rom_destroy(void *data)
{
  multirom_set_usb_refresh_thread(mrom_status, 0);
  pthread_mutex_lock(&exit_code_mutex);
  loop_act &= ~(LOOP_UPDATE_USB);
  pthread_mutex_unlock(&exit_code_mutex);

  tab_data_roms *t = (tab_data_roms*)data;

  list_clear(&t->buttons, &button_destroy);
  list_clear(&t->ui_elements, &fb_remove_item);

  listview_destroy(t->list);

  if(t->usb_prog)
    progdots_destroy(t->usb_prog);

  free(t);
}

void multirom_ui_tab_rom_confirmed(listview_item *it)
{
  multirom_ui_tab_rom_boot_btn(0);
}
void multirom_ui_tab_rom_boot_btn(int action)
{
  int cur_tab = themes_info->data->selected_tab;
  if(!themes_info->data->tab_data[cur_tab])
    return;

  tab_data_roms *t = themes_info->data->tab_data[cur_tab];
  if(!t->list->selected)
    return;

  struct multirom_rom *rom = multirom_get_rom_by_id(mrom_status, t->list->selected->id);
  if(!rom)
    return;

  int error = 0;
  ncard_builder *b = ncard_create_builder();
  ncard_set_pos(b, NCARD_POS_CENTER);
  ncard_add_btn(b, BTN_NEGATIVE, "ok", ncard_hide_callback, NULL);
  ncard_set_cancelable(b, 1);
  ncard_set_title(b, "Error");

  int m = M(rom->type);
  if(m & MASK_UNSUPPORTED)
  {
    ncard_set_text(b, "Unsupported ROM type, see XDA thread for more info!");
    error = 1;
  }
  else if (((m & MASK_KEXEC) || ((m & MASK_ANDROID) && rom->has_bootimg)) &&
      multirom_has_kexec() != 0)
  {
    ncard_set_text(b, "Kexec-hardboot support is required to boot this ROM.\n\nInstall kernel with kexec-hardboot support to your Internal ROM!");
    error = 1;
  }
  else if((m & MASK_KEXEC) && strchr(rom->name, ' '))
  {
    ncard_set_text(b, "ROM's name contains spaces. Please remove spaces from this ROM's name");
    error = 1;
  }

  if(error)
  {
    ncard_show(b, 1);
    return;
  }
  else
    ncard_destroy_builder(b);

  pthread_mutex_lock(&exit_code_mutex);
  selected_rom = rom;
  exit_ui_code = UI_EXIT_BOOT_ROM;
  pthread_mutex_unlock(&exit_code_mutex);
}

void multirom_ui_tab_rom_update_usb(void)
{
  tab_data_roms *t = (tab_data_roms*)themes_info->data->tab_data[TAB_USB];
  listview_clear(t->list);

  multirom_ui_fill_rom_list(t->list, MASK_USB_ROMS);
  listview_update_ui(t->list);

  multirom_ui_tab_rom_set_empty(t, (int)(t->list->items == NULL));
  fb_request_draw();
}

void multirom_ui_tab_rom_refresh_usb(int action)
{
  multirom_update_partitions(mrom_status);
}

void multirom_ui_tab_rom_set_empty(void *data, int empty)
{
  assert(empty == 0 || empty == 1);

  tab_data_roms *t = (tab_data_roms*)data;

  // if(t->boot_btn)
  //     button_enable(t->boot_btn, !empty);
  /*
     if(empty && !t->usb_text)
     {
     fb_text_proto *p = fb_text_create(0, 0, C_TEXT, SIZE_NORMAL, "This list is refreshed automagically, just plug in the USB drive and wait.");
     p->wrap_w = t->list->w - 100*DPI_MUL;
     p->justify = JUSTIFY_CENTER;
     t->usb_text = fb_text_finalize(p);
     list_add(&t->ui_elements, t->usb_text);
     tabview_add_item(themes_info->data->tabs, TAB_USB, t->usb_text);

     center_text(t->usb_text, t->list->x, -1, t->list->w, -1);
     t->usb_text->y = t->list->y + t->list->h*0.2;

     int x = t->list->x + ((t->list->w/2) - (PROGDOTS_W/2));
     t->usb_prog = progdots_create(x, t->usb_text->y+100*DPI_MUL);
     tabview_add_item(themes_info->data->tabs, TAB_USB, t->usb_prog->rect);
     tabview_add_item(themes_info->data->tabs, TAB_USB, t->usb_prog);
     }
     else if(!empty && t->usb_text)
     {
     tabview_rm_item(themes_info->data->tabs, TAB_USB, t->usb_prog->rect);
     tabview_rm_item(themes_info->data->tabs, TAB_USB, t->usb_prog);
     progdots_destroy(t->usb_prog);
     t->usb_prog = NULL;

     tabview_rm_item(themes_info->data->tabs, TAB_USB, t->usb_text);
     list_rm(&t->ui_elements, t->usb_text, &fb_remove_item);
     t->usb_text = NULL;
     }*/
}

void *multirom_ui_tab_misc_init(void)
{
  tab_data_misc *t = mzalloc(sizeof(tab_data_misc));
  cur_theme->tab_misc_init(themes_info->data, t, mrom_status->colors);
  return t;
}

void multirom_ui_tab_misc_destroy(void *data)
{
  tab_data_misc *t = (tab_data_misc*)data;

  list_clear(&t->ui_elements, &fb_remove_item);
  list_clear(&t->buttons, &button_destroy);

  free(t);
}

void multirom_ui_tab_misc_change_clr(int clr)
{
  if((loop_act & LOOP_CHANGE_CLR) || mrom_status->colors == clr)
    return;

  pthread_mutex_lock(&exit_code_mutex);
  mrom_status->colors = clr;
  loop_act |= LOOP_CHANGE_CLR;
  pthread_mutex_unlock(&exit_code_mutex);
}

void multirom_ui_reboot_btn(int action)
{
  pthread_mutex_lock(&exit_code_mutex);
  exit_ui_code = action;
  pthread_mutex_unlock(&exit_code_mutex);
}

void multirom_ui_tab_misc_copy_log(int action)
{
  multirom_dump_status(mrom_status);

  int res = multirom_copy_log(NULL, "../multirom_log.txt");

  static const char *text[] = { "Failed to copy log to sdcard!", "Error log was saved to:\n\n<s>/sdcard/multirom_log.txt</s>" };

  ncard_builder *b = ncard_create_builder();
  ncard_set_pos(b, NCARD_POS_CENTER);
  ncard_add_btn(b, BTN_NEGATIVE, "ok", ncard_hide_callback, NULL);
  ncard_set_title(b, "Save error log");
  ncard_set_text(b, text[res+1]);
  ncard_set_cancelable(b, 1);
  ncard_show(b, 1);
}
