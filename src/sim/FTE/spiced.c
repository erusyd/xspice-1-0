
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:12:27 $
 *           $Source: //pepper/atesse_spice/spice3/FTE/RCS/spiced.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Do remote spice jobs. The first line transmitted will be of the
 * format: "user host program". The program field may be left out,
 * in which case it defaults to SPICEPATH. The program is then
 * executed, with the single argument "-S". The remote host should
 * wait for a response from spiced before sending any data -- so
 * far the only response sent is "ok".
 * Eventually we will want to deal with load-balancing
 * of spice jobs.
 */

#include "prefix.h"
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#ifdef HPUX
#include <time.h>
#include <fcntl.h>
#else
#include <sys/wait.h>
#include <sys/time.h>
#include <sys/resource.h>
#endif
#include <sys/ioctl.h>
#include <sys/file.h>
#include <signal.h>
#include "suffix.h"

#undef DEBUG

#ifndef SPICEDLOG
#define SPICEDLOG "/dev/null"
#endif

#define MAXJOBS 2

int nrunning = 0;
int maxjobs = MAXJOBS;

extern int errno;
extern char *sys_errlist[];
extern char **environ;

/* ARGSUSED */
void
main(ac, av)
    char **av;
{
    int s, inetd = 0;
    struct servent *sp;
    struct protoent *pp;
    struct sockaddr_in from, sin;
    int g, i, len = sizeof (struct sockaddr_in);
    void dostuff();
    int sigchild();
    FILE *fp;
    char *datestring();

    /* Process arguments. */

    av++;
    while (*av) {
        if (!strcmp(*av, "-i"))
            inetd++;
        else
            maxjobs = atoi(av[1]);
        av++;
    }

    if (inetd == 0) {
        /* Find out who we are. */
        sp = getservbyname("spice", "tcp");
        if (sp == NULL) {
            fprintf(stderr, "Error: spice/tcp: unknown service\n");
            exit(1);
        }
        pp = getprotobyname("tcp");
        if (pp == NULL) {
            fprintf(stderr, "Error: tcp: unknown protocol\n");
            exit(1);
        }

        /* Create the socket. */
        s = socket(AF_INET, SOCK_STREAM, pp->p_proto);
        if (s < 0) {
            perror("spiced: socket");
            exit(1);
        }
        sin.sin_port = sp->s_port;
        if (bind(s, (struct sockaddr *) &sin, sizeof
                (struct sockaddr_in)) < 0) {
            perror("spiced: bind");
            exit(1);
        }

#ifndef DEBUG
        /* Disconnect from the controlling terminal. */
        if (fork())
            exit(0);
        for (i = 0; i < 10; i++)
            if (i != s)
                (void) close(i);
        (void) open("/", O_RDONLY);
        (void) dup2(0, 1);
        (void) dup2(0, 2);
        i = open("/dev/tty", O_RDWR);
        if (i > 0) {
            (void) ioctl(i, TIOCNOTTY, (char *) NULL);
            (void) close(i);
        }

#endif

#ifdef HPUX
        (void) signal(SIGCLD, sigchild);
#else
        (void) signal(SIGCHLD, sigchild);
#endif
        fp = fopen(SPICEDLOG, "a");
        fprintf(fp, "\n-- new daemon, pid = %d, date = %s\n\n",
                getpid(), datestring());
        (void) fclose(fp);

        /* Start (void) listening for requests. */
        (void) listen(s, 5);
        for (;;) {
            g = accept(s, (struct sockaddr *) &from, &len);
            if (g < 0) {
                if (errno != EINTR) {
                    fp = fopen(SPICEDLOG, "a");
                    fprintf(fp, "\n>>> accept: %s\n\n",
                        sys_errlist[errno]);
                    exit(1);
                }
                continue;
            }
            if (!fork()) {
                (void) close(s);
#ifdef HPUX
                (void) signal(SIGCLD, SIG_IGN);
#else
                (void) signal(SIGCHLD, SIG_IGN);
#endif
                dostuff(g);
            }
            nrunning++;
            (void) close(g);
        }
    } else {
        /* All this is already done for us. */
        dostuff(0);
    }
}

void
dostuff(s)
{
    FILE *fp;
    char *datestring();
    char buf[BUFSIZ], user[16], host[32], program[128];
    char *argv[3], *t;
    int i, rc;
#ifndef HPUX
    union wait stats;
    struct rusage ru;
#endif

    /* Should do some sort of verification... */
    i = read(s, buf, BUFSIZ);
    if (i < 0) {
        perror("spiced: read");
        exit(1);
    }
    i = sscanf(buf, "%s %s %s", user, host, program);
    if (i < 2) {
        fprintf(stderr, "Error: bad init line: %s\n", buf);
        exit(1);
    }
    if (i == 2)
        (void) strcpy(program, SPICEPATH);

    if (nrunning > maxjobs - 1) {
        /* Too many people. */
        (void) write(s, "toomany", 8);
        fp = fopen(SPICEDLOG, "a");
        fprintf(fp, "%s: %s@%s: turned down - %d jobs now\n",
            datestring(), user, host, nrunning);
        (void) fclose(fp);
        exit(0);
    }
    (void) write(s, "ok", 3);

    if ((i = fork()) == 0) {
#ifdef HPUX
        (void) close(0);
        (void) close(1);
        (void) close(2);
        fcntl(s, F_DUPFD, 0);
        fcntl(s, F_DUPFD, 1);
        fcntl(s, F_DUPFD, 2);
#else
        (void) dup2(s, 0);
        (void) dup2(s, 1);
        (void) dup2(s, 2);
#endif
        argv[0] = program;
        argv[1] = "-S";
        argv[2] = NULL;
        (void) execve(program, argv, environ);
        perror(program);
        exit(1);
    }
#ifdef HPUX
    wait(0);
    rc = 0;
#else
    if (wait3(&stats, 0, &ru) != i) {
        perror("wait");
        exit(1);
    }
    rc = stats.w_retcode;
#endif

    /* Write a log entry. */
#ifdef HPUX
    (void) strcpy(buf, "unknown");
#else
    (void) sprintf(buf, "%d:%d.%6d", ru.ru_utime.tv_sec / 60,
            ru.ru_utime.tv_sec % 60, ru.ru_utime.tv_usec);
    for (t = buf; *t; t++)
        ;
    for (t--; *t == '0'; *t-- = '\0')
        ;
#endif
    fp = fopen(SPICEDLOG, "a");
    fprintf(fp, "%s: %s@%s: %s - \n\texit %d, time %s\n",
            datestring(), user, host, program, rc,
            buf);
    (void) fclose(fp);
    exit(0);
}

/* Return the date. Return value is static data. */

#ifdef HPUX

char *
datestring()
{
    int tloc;

    time(&tloc);
    return (ctime(&tloc));
}

#else

char *
datestring()
{
    register char *tzn;
    struct tm *tp;
    static char tbuf[40];
    char *ap;
    struct timeval tv;
    struct timezone tz;
    char *timezone(), *asctime();
    int i;
    struct tm *localtime();

    (void) gettimeofday(&tv, &tz);
    tp = localtime((long *) &tv.tv_sec);
    ap = asctime(tp);
    tzn = timezone(tz.tz_minuteswest, tp->tm_isdst);
    (void) sprintf(tbuf, "%.20s", ap);
    if (tzn)
        (void) strcat(tbuf, tzn);
    (void) strcat(tbuf, ap + 19);
    i = strlen(tbuf);
    tbuf[i - 1] = '\0';
    return (tbuf);
}

#endif

/* Don't care what happens to the child... */

int
sigchild()
{
    int pid;
    FILE *fp;

    pid = wait((union wait *) NULL);
    if (pid == -1) {
        fp = fopen(SPICEDLOG, "a");
        fprintf(fp, "\n>>>spiced: wait: %s\n\n", sys_errlist[errno]);
        (void) fclose(fp);
    } else
        nrunning--;
    return;
}

