// engler, cs140e: your code to find the tty-usb device on your laptop.

#include <assert.h>

#include <fcntl.h>

#include <string.h>

#include <sys/stat.h>

#include <time.h>

#include "libunix.h"

#define _SVID_SOURCE

#include <dirent.h>

static const char *ttyusb_prefixes[] = {
    "cu.SLAB_USB", // mac os
    "ttyUSB",   // linux
// if your system uses another name, add it.
};

static int filter(const struct dirent *d) {

// scan through the prefixes, returning 1 when you find a match.

// 0 if there is no match.

//    struct dirent {

//         ino_t          d_ino;       /* inode number */

//         off_t          d_off;       /* offset to the next dirent */

//         unsigned short d_reclen;    /* length of this record */

//         unsigned char  d_type;      /* type of file; not supported

//                                     by all file system types */

//         char           d_name[256]; /* filename */

//     };

    for(int j = 0; j < 2; j++) {
        const char * ttyusb_prefix = ttyusb_prefixes[j];
        int prefix_len = strlen(ttyusb_prefix);
        int found = 1;
        for(int i = 0; i < prefix_len; i++) {
            if (*(ttyusb_prefix+i) != *(d->d_name+i)) {
                found = 0;
                break;
            }
        }
        if (found == 1) return 1;
    }
    return 0;
}

// find the TTY-usb device (if any) by using <scandir> to search for

// a device with a prefix given by <ttyusb_prefixes> in /dev

// returns:

//  - device name.

// error: panic's if 0 or more than 1 devices.

char *find_ttyusb(void) {
// use <alphasort> in <scandir>
// return a malloc'd name so doesn't corrupt.
    struct dirent **namelist;
    int n;
    char * res;
    n = scandir("/dev", &namelist, NULL, alphasort);
    if (n <= 0)
// perror("find_ttyusb n<=0");
        return "find_ttyusb n<=0";
    else {
        while (n--) {
            if (filter(namelist[n]) == 1) {
                int name_len = strlen(namelist[n]->d_name);
                res = (char *)malloc(name_len + 1);
                strcpy(res, namelist[n]->d_name);
                return res;
            }
            free(namelist[n]);
        }
        free(namelist);
    }

// perror("find_ttyusb: not found");
    return "find_ttyusb: not found";
}

// return the most recently mounted ttyusb (the one
// mounted last).  use the modification time
// returned by stat().

char *find_ttyusb_last(void) {
    struct dirent **namelist;
    int n;
    char * res = NULL;
    time_t t = {0};
    n = scandir("/dev", &namelist, NULL, alphasort);
    if (n <= 0)
        return "find_ttyusb_last n<=0";
    else {
        res = (char*)malloc(24);
        while (n--) {
            if (filter(namelist[n]) == 1) {
                // int size = strlen(namelist[n]->d_name);
                char * dest = (char *)malloc(24);
                
                strcat(dest, "/dev/");
                strcat(dest, namelist[n]->d_name);
                struct stat buf;
                stat(dest, &buf);
                if (buf.st_mtime > t) {
                    t = buf.st_mtime;
                    // int name_len = strlen(namelist[n]->d_name);
                    strcpy(res, dest);
                }
                free(dest);
            }
            // free(namelist[n]);
        }
        // free(namelist);
    }
    if (res == NULL) return "find_ttyusb_last: not found";
    return res;
}

// return the oldest mounted ttyusb (the one mounted
// "first") --- use the modification returned by
// stat()

char *find_ttyusb_first(void) {
    struct dirent **namelist;
    int n;
    char * res = NULL;
    time_t t = time(NULL);
    n = scandir("/dev", &namelist, NULL, alphasort);
    if (n <= 0)
        return "find_ttyusb_first n< 0";
    else {
        while (n--) {
            if (filter(namelist[n]) == 1) {
                int size = strlen(namelist[n]->d_name);
                char * dest = malloc(size+6);
                dest[0] = '/';
                dest[1] = 'd';
                dest[2] = 'e';
                dest[3] = 'v';
                dest[4] = '/';
                char * path = strcat(dest, namelist[n]->d_name);
                struct stat buf;
                stat(path, &buf);
                if (buf.st_mtime < t) {
                    t = buf.st_mtime;
                    int name_len = strlen(namelist[n]->d_name);
                    res = (char*)malloc(name_len + 1);
                    strcpy(res, namelist[n]->d_name);
                }
            }
            // free(namelist[n]);
        }
        free(namelist);
    }
    if (res == NULL) return "find_ttyusb_first: not found";
    return res;
}

// struct stat

// {

//     dev_t st_dev; //device 文件的设备编号

//     ino_t st_ino; //inode 文件的i-node

//     mode_t st_mode; //protection 文件的类型和存取的权限

//     nlink_t st_nlink; //number of hard links 连到该文件的硬连接数目, 刚建立的文件值为1.

//     uid_t st_uid; //user ID of owner 文件所有者的用户识别码

//     gid_t st_gid; //group ID of owner 文件所有者的组识别码

//     dev_t st_rdev; //device type 若此文件为装置设备文件, 则为其设备编号

//     off_t st_size; //total size, in bytes 文件大小, 以字节计算

//     unsigned long st_blksize; //blocksize for filesystem I/O 文件系统的I/O 缓冲区大小.

//     unsigned long st_blocks; //number of blocks allocated 占用文件区块的个数, 每一区块大小为512 个字节.

//     time_t st_atime; //time of lastaccess 文件最近一次被存取或被执行的时间, 一般只有在用mknod、utime、read、write 与tructate 时改变.

//     time_t st_mtime; //time of last modification 文件最后一次被修改的时间, 一般只有在用mknod、utime 和write 时才会改变

//     time_t st_ctime; //time of last change i-node 最近一次被更改的时间, 此参数会在文件所有者、组、权限被更改时更新

// };