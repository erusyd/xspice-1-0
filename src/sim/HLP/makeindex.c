
/* from FILENAME.txt, make FILENAME.idx */

#include "prefix.h"
#include <stdio.h>
#include "CPdefs.h"
#include "HLPdefs.h"
#include "suffix.h"

main(argc, argv)
int argc;
char *argv[];
{

    FILE *fp;
    FILE *wfp;
    char buf[BSIZE];
    long fpos;
    char subject[BSIZE];
    struct hlp_index indexitem;
    char *pos;

    while (--argc) {
      if (!(fp = fopen(argv[argc], "r"))) {
        perror(argv[argc]);
        continue;
      }
      strcpy(buf, argv[argc]);
      if (((pos = rindex(buf, '.')) == 0) ||
            strcmp(pos, ".txt")) {
        fprintf(stderr, "%s does not end in .txt\n", buf);
        continue;
      }
      *++pos = 'i'; *++pos = 'd'; *++pos = 'x';
      if (!(wfp = fopen(buf, "w"))) {
        perror(buf);
        continue;
      }
      fpos = 0;
      while (fgets(buf, BSIZE, fp)) {
        if (!strncmp(buf, "SUBJECT: ", 9)) {
          strcpy(subject, &buf[9]);
          subject[strlen(subject) - 1] = '\0';  /* get rid of '\n' */
          strncpy(indexitem.subject, subject, 64);  /* zero out end */
          indexitem.fpos = fpos;
          fwrite(&indexitem, sizeof(struct hlp_index), 1, wfp);
        }
        fpos = ftell(fp);
      }
    }

}
