#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#include "librtspcli.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <stdio.h>

static void signal_handler(int signo)
{
    switch (signo) {
    case SIGINT:
        exit(0);
        break;
    case SIGPIPE:
        break;
    default:
        break;
    }
    return;
}

static void init_signals(void)
{
    signal(SIGINT, signal_handler);
    signal(SIGPIPE, signal_handler);
    return;
}

static int store_audio_frame(char *data, int size)
{
    static FILE *a_fp = NULL;
    
    if (a_fp == NULL) {
        a_fp = fopen("./recv.g711u", "wb+");
        if (a_fp == NULL) {
            perror("open file error");
            goto END;
        }
    }

    if(fwrite(data, 1, size, a_fp) < 0) {
        perror("write to file error");
        goto END;
    }

    return 0;
END:
    if (a_fp) {
        fclose(a_fp);
    }

    return -1;
}

static int store_frm(struct chn_info *chnp, struct frm_info *frmp)
{
    static FILE *fp = NULL;
    if (fp == NULL) {
        fp = fopen("./recv.h264", "wb+");
        if (fp == NULL) {
            perror("open file error");
            goto END;
        }
    }

    if (fp) {
        if(fwrite(frmp->frm_buf + chnp->frm_hdr_sz, 1, frmp->frm_sz, fp) < 0) {
            perror("write to file error");
            goto END;
        }

        static int cnt = 0;
        printf("------------>[cnt=%d], size = %d, type = %d \r\n", cnt, frmp->frm_sz, frmp->frm_type);
        cnt++;
    }
    return 0;
END:
    if (fp) {
        fclose(fp);
    }

    return 0;
}

void rtsp_client(void* arg)
{
    char *uri = (char*)arg;
    unsigned long usr_id = 0;
    struct chn_info chn_info = {0};
    
    init_signals();

    register_store_audio_frm_cb(store_audio_frame);
    
    if (init_rtsp_cli(store_frm) < 0) {
        return -1;
    }
    printf("uri addrs = [%s] \r\n", uri);
    /* remote channel information */
    memset(&chn_info, 0, sizeof(chn_info));
    chn_info.frm_hdr_sz = 0;
    chn_info.usr_data = NULL;
    usr_id = open_chn(uri, &chn_info, 0);

    if (!usr_id) {
        return -1;
    }
    #if 1
    //close_chn(usr_id);
    while (1) {
        sleep(3);
    }
    #endif
    return 0;
}
