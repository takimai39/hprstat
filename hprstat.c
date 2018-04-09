#include <stdio.h>
#include <stdlib.h>
#include <sys/pstat.h>
#include <string.h>
#include <errno.h>

#define BURST ((size_t)10)

void getlwp();

main(argc, argv)
int argc;
char *argv[];
{
    struct pst_status pst[BURST];
    struct lwp_status lwp;
    pid_t target_pid = 0;
    int i, count;
    int idx = 0;

    if ( argc > 2 ) {
        printf ("Usage: %s <PID>\n", argv[0] );
        exit(0);
    }
    target_pid = atoi(argv[1]);

    if ( target_pid != 0 ) { 
        memset(pst, 0, BURST*sizeof(struct pst_status));
        if( -1 == (count = pstat_getproc(pst, sizeof(pst[0]), (size_t)0, target_pid)))
            perror("pstat_getproc"), exit(1);
        printf("commmand: %s\n", pst[0].pst_ucomm);
        printf("lwpid\tpid\tpri\tstatus\tUsrTime\tSysTime\n");
        printf("-----\t---\t---\t----\t------\t------\n");
        getlwp(target_pid, pst[0].pst_nlwps);
    }
    else {
        memset(pst, 0, BURST*sizeof(struct pst_status));
        while (( count = pstat_getproc(pst, sizeof(pst[0]), BURST, idx)) > 0 ) {
            for ( i = 0 ; i < count ; i++ ) {
                printf("commmand: %s\n", pst[i].pst_ucomm);
                printf("lwpid\tpid\tpri\tstatus\tUsrTime\tSysTime\n");
                printf("-----\t---\t---\t----\t------\t------\n");
                getlwp(pst[i].pst_pid, pst[i].pst_nlwps);
            }
            idx = pst[count - 1].pst_idx +1;
            memset(pst, 0, BURST*sizeof(struct pst_status));
        }
        if ( count == -1)
            perror("pstat_getproc"), exit(1);
    }
}
void getlwp(pid_t pid, int nthreads)
{
    struct lwp_status lwpbuf;
    int count, idx;

    for ( idx = 0 ; idx < nthreads ; idx++ ) {
        memset(&lwpbuf, 0, sizeof(struct lwp_status));
        count = pstat_getlwp(&lwpbuf, sizeof(struct lwp_status), (size_t)1, idx, pid);
        if (( count == -1 ) && (errno == ESRCH )) 
            perror("pstat_getlwp(): cannot find given lwpid or pid");
        else if ( count == -1 )
            perror("pstat_getlwp()");
        else {
            printf("%lld\t%ld\t%lld\t%lld\t%lld\t%lld\n", 
                lwpbuf.lwp_lwpid, pid, lwpbuf.lwp_pri, lwpbuf.lwp_stat, 
                lwpbuf.lwp_utime, lwpbuf.lwp_stime);
        }
    }
}
