
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:04:15 $
 *           $Source: //pepper/atesse_spice/spice3/CP/RCS/glob.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Expand global characters.
 */

#include "prefix.h"
#include "CPdefs.h"
#ifdef UNIX
#include <sys/types.h>
#include <sys/dir.h>
#include <pwd.h>
#endif
#include "suffix.h"

static wordlist *brac1();
static wordlist *brac2();
static wordlist *bracexpand();
static wordlist *globexpand();
static bool noglobs();
static char *pcanon();
static int sortcmp();

#ifdef OLDDIRSTUFF

#include <fcntl.h>

/* Have to hack up the directory stuff... What a pain. */

typedef int DIR;

DIR *
opendir(path)
{
    static int i;

    i = open(path, O_RDONLY);
    if (i == -1)
        return (NULL);
    else
        return (&i);
}

struct direct *
readdir(i)
    DIR *i;
{
    static struct direct d;

    if (read(*i, &d, sizeof (struct direct)) < sizeof (struct direct))
        return (NULL);
    else
        return (&d);
}

void
closedir(i)
    DIR *i;
{
    (void) close (*i);
    return;
}

#endif

char cp_comma = ',';
char cp_ocurl = '{';
char cp_ccurl = '}';
char cp_huh = '?';
char cp_star = '*';
char cp_obrac = '[';
char cp_cbrac = ']';
char cp_til = '~';

/* For each word, go through two steps: expand the {}'s, and then do ?*[]
 * globbing in them. Sort after the second phase but not the first...
 */

wordlist *
cp_doglob(wlist)
    wordlist *wlist;
{
    wordlist *wl, *w, *nwl;
    char *s;

    /* Expand {a,b,c} */

    for (wl = wlist; wl; wl = wl->wl_next) {
        w = bracexpand(wl->wl_word);
        if (!w) {
            wlist->wl_word = NULL;
            return (wlist);
        }
        nwl = wl_splice(wl, w);
        if (wlist == wl)
            wlist = w;
        wl = nwl;
    }

    /* Do tilde expansion. */

    for (wl = wlist; wl; wl = wl->wl_next)
        if (*wl->wl_word == cp_til) {
            s = cp_tildexpand(wl->wl_word);
            if (!w) {
                wlist->wl_word = NULL;
                return (wlist);
            }
            wl->wl_word = s;
        }

    /* Now, expand *?[] for each word. unset * and unalias * mean 
     * something special...
     */

    if ((cp_noglob == true) || eq(wlist->wl_word, "unset") ||
            eq(wlist->wl_word, "unalias"))
        return (wlist);

    for (wl = wlist; wl; wl = wl->wl_next) {
        if (noglobs(wl->wl_word))
            continue;
        w = globexpand(wl->wl_word);
        if (w == NULL)
            continue;
        nwl = wl_splice(wl, w);
        if (wlist == wl)
            wlist = w;
        wl = nwl;
    }
    return (wlist);
}

static wordlist *
bracexpand(string)
    char *string;
{
    wordlist *wl, *w;
    char *s;

    if (!string)
        return (NULL);
    wl = brac1(string);
    if (!wl)
        return (NULL);
    for (w = wl; w; w = w->wl_next) {
        s = w->wl_word;
        w->wl_word = copy(s);
        tfree(s);
    }
    return (wl);
}

/* Given a string, returns a wordlist of all the {} expansions. This is
 * called recursively by cp_brac2(). All the words here will be of size
 * BSIZE, so it is a good idea to copy() and free() the old words.
 */

static wordlist *
brac1(string)
    char *string;
{
    wordlist *words, *wl, *w, *nw, *nwl, *newwl;
    char *s;
    int nb;

    words = alloc(wordlist);
    words->wl_word = tmalloc(BSIZE);
    for (s = string; *s; s++) {
        if (*s == cp_ocurl) {
            nwl = brac2(s);
            nb = 0;
            for (;;) {
                if (*s == cp_ocurl)
                    nb++;
                if (*s == cp_ccurl)
                    nb--;
                if (*s == '\0') {   /* { */
                    fprintf(cp_err, "Error: missing }.\n");
                    return (NULL);
                }
                if (nb == 0)
                    break;
                s++;
            }
            /* Add nwl to the rest of the strings in words. */
            newwl = NULL;
            for (wl = words; wl; wl = wl->wl_next)
                for (w = nwl; w; w = w->wl_next) {
                    nw = alloc(wordlist);
                    nw->wl_word = tmalloc(BSIZE);
                    (void) strcpy(nw->wl_word, wl->wl_word);
                    (void) strcat(nw->wl_word, w->wl_word);
                    newwl = wl_append(newwl, nw);
                }
            wl_free(words);
            words = newwl;
        } else
            for (wl = words; wl; wl = wl->wl_next)
                appendc(wl->wl_word, *s);
    }
    return (words);
}

/* Given a string starting with a {, return a wordlist of the expansions
 * for the text until the matching }.
 */

static wordlist *
brac2(string)
    char *string;
{
    wordlist *wlist = NULL, *nwl;
    char buf[BSIZE], *s;
    int nb;
    bool eflag = false;

    string++;   /* Get past the first open brace... */
    for (;;) {
        (void) strcpy(buf, string);
        nb = 0;
        s = buf;
        for (;;) {
            if ((*s == cp_ccurl) && (nb == 0)) {
                eflag = true;
                break;
            }
            if ((*s == cp_comma) && (nb == 0))
                break;
            if (*s == cp_ocurl)
                nb++;
            if (*s == cp_ccurl)
                nb--;
            if (*s == '\0') {       /* { */
                fprintf(cp_err, "Error: missing }.\n");
                return (NULL);
            }
            s++;
        }
        *s = '\0';
        nwl = brac1(buf);
        wlist = wl_append(wlist, nwl);
        string += s - buf + 1;
        if (eflag)
            return (wlist);
    }
}

/* Return a wordlist, with *?[] expanded and sorted. This is the idea: set
 * up an array with possible matches, and go through each path component
 * and search the appropriate directories for things that match, and add
 * those that do to the array.
 */

static wordlist *
globexpand(string)
    char *string;
{
#ifdef UNIX
    char *poss[MAXWORDS];
    DIR *wdir;
    struct direct *de;
    char buf[BSIZE];
    char *s;
    char *point;    /* Where we are at in the pathname. */
    int i, j, num = 0;
    bool found;
    wordlist *wlist = NULL, *wl, *lwl = NULL;   /* Make lint shut up. */

    bzero((char *) poss, MAXWORDS * sizeof (char *));
    string = pcanon(string);
    point = string;
    if (*point == '/') {
        point++;
        poss[0] = copy("/");
    } else
        poss[0] = copy(".");
nextcomp:
    (void) strcpy(buf, point);
    s = index(buf, '/');
    if (s)
        *s = '\0';
    for (i = 0; i < MAXWORDS; i++) {
        if (!poss[i] || (poss[i][0] == '\0'))
            continue;
        found = false;
        wdir = opendir(poss[i]);
        if (wdir == NULL) {
            if (cp_nonomatch) {
                wl = alloc(wordlist);
                wl->wl_word = copy(string);
                return (wl);
            } else {
                fprintf(cp_err, "%s: no match.\n", string);
                return (NULL);
            }
        }
        while ((de = readdir(wdir)) != NULL)
            if (cp_globmatch(buf, de->d_name)) {
                found = true;
                for (j = 0; j < MAXWORDS; j++)
                    if (!poss[j])
                        break;
                if (j == MAXWORDS) {
                    fprintf(cp_err, 
                        "Too many arguments.\n");
                    goto err;
                }
                poss[j] = tmalloc(BSIZE);
                (void) strcpy(poss[j] + 1, poss[i]);
                (void) strcat(poss[j] + 1, "/");
                (void) strcat(poss[j] + 1, de->d_name);
            }
        tfree(poss[i]);
        poss[i] = NULL;
        (void) closedir(wdir);
        if (!found) {
            if (cp_nonomatch) {
                wl = alloc(wordlist);
                wl->wl_word = copy(string);
                return (wl);
            } else {
                fprintf(cp_err, "%s: no match.\n", string);
                return (NULL);
            }
        }
    }
    /* Hide the newly found words from the globbing process by making 
     * the first byte a '\0'.
     */
    for (i = 0; i < MAXWORDS; i++)
        if (poss[i])
            poss[i]++;
    if (index(point, '/')) {
        point = index(point, '/') + 1;
        goto nextcomp;
    }

    /* Compact everything properly. */

    for (i = 0; i < MAXWORDS; i++) {
        if (!poss[i])
            continue;
        j = i - 1;
        while (!poss[j] && (j > -1))
            j--;
        j++;
        if (!poss[j] && (i != j)) {
            poss[j] = poss[i];
            poss[i] = NULL;
        }
        num++;
    }

    /* Now, sort the stuff and make it into wordlists. */

    qsort((char *) poss, num, sizeof (char *), sortcmp);

    for (i = 0; i < num; i++) {
        if (!poss[i])
            continue;
        wl = alloc(wordlist);
        wl->wl_word = copy(pcanon(poss[i]));
        if (((int) poss[i]) % 2)
            poss[i]--;
        tfree(poss[i]);
        if (wlist == NULL)
            wlist = wl;
        else {
            wl->wl_prev = lwl;
            lwl->wl_next = wl;
        }
        lwl = wl;
    }
    return (wlist);
err:
    for (i = 0; i < MAXWORDS; i++)
        if (poss[i])
            tfree(poss[i]);
    return (NULL);
#else
    wordlist *wl = alloc(wordlist);

    wl->wl_word = copy(string);
    return (wl);
#endif
}

/* Normalize filenames (get rid of extra ///, .///... etc.. ) */

static char *
pcanon(string)
    char *string;
{
    char *p, *s;
    
    s = p = tmalloc(strlen(string) + 1);

bcomp:
    if ((*string == '.') && (*(string + 1) == '/')) {
        string += 2;
        goto bcomp;
    }
morew:
    if (*string == '/') {
        *s++ = '/';
        while (*++string == '/');
        goto bcomp;
    }
    if (!*string) {
        if (*(s - 1) == '/')
            s--;
        *s = '\0';
        return (p);
    }
    *s++ = *string++;
    goto morew;
}

static int
sortcmp(s1, s2)
    char **s1, **s2;
{
    register char *a, *b;

    a = *s1;
    b = *s2;
    for (;;) {
        if (*a > *b)
            return (1);
        if (*a < *b)
            return (-1);
        if (*a == '\0')
            return (0);
        a++;
        b++;
    }
}

/* Expand tildes. */

char *
cp_tildexpand(string)
    char *string;
{
#ifdef UNIX
    struct passwd *pw;
    char *rest, buf[BSIZE], *sss = copy(string);

    if (*string != cp_til)
        return (string);
    
    string = copy(string);

    if (string[1] == '/') {
        pw = getpwuid(getuid());
        if (pw == NULL) {
            fprintf(cp_err, "Hey, neat trick...\n");
            return ("error");
        }
        rest = string + 2;
    } else if (string[1] == '\0') {
        pw = getpwuid(getuid());
        rest = NULL;
    } else {
        rest = index(string, '/');
        if (rest)
            *rest++ = '\0';
        if (*string == '\0')
            pw = getpwuid(getuid());
        else
            pw = getpwnam(++string);
        if (!pw) {
            if (cp_nonomatch) {
                return (sss);
            } else {
                tfree(sss);
                fprintf(cp_err, "Error: no such user %s\n",
                            string);
                return (NULL);
            }
        }
    }
    (void) strcpy(buf, pw->pw_dir);
    if (rest) {
        (void) strcat(buf, "/");
        (void) strcat(buf, rest);
    }
    tfree(sss);
    return (copy(buf));
#else
    return (copy(string));
#endif
}

/* Say whether the pattern p can match the string s. */

bool
cp_globmatch(p, s)
    char *p, *s;
{
    char schar, pchar, bc, fc;
    bool bchar, except;

    if ((*s == '.') && ((*p == cp_huh) || (*p == cp_star)))
        return (false);

    for (;;) {
        schar = strip(*s++);
        pchar = *p++;
        if (pchar == cp_star) {
            if (*p == '\0')
                return (true);
            for (s--; *s != '\0'; s++)
                if (cp_globmatch(p, s))
                    return (true);
            return (false);
        } else if (pchar == cp_huh) {
            if (schar == '\0')
                return (false);
            continue;
        } else if (pchar == cp_obrac) {
            bchar = false;
            if (*p == '^') {
                except = true;
                p++;
            } else
                except = false;
            fc = -1;
            while (bc = *p++) {
                if (bc == cp_cbrac) {
                    if ((bchar && !except) || 
                        (!bchar && except))
                        break;
                    else
                        return (false);
                }
                if (bc == '-') {
                    if (fc <= schar && schar <= *p++)
                        bchar = true;
                } else {
                    fc = bc;
                    if (fc == schar)
                        bchar = true;
                }
            }
            if (bc == '\0') {
                fprintf(cp_err, "Error: missing ].\n");
                return (false);
            }
            continue;
        } else if (pchar == '\0') {
            if (schar == '\0')
                return (true);
            else
                return (false);
        } else {
            if (strip(pchar) != schar)
                return (false);
            continue;
        }
    }
}

static bool
noglobs(string)
    char *string;
{
    if (index(string, cp_star) || index(string, cp_huh) || 
            index(string, cp_obrac))
        return (false);
    else
        return (true);
}

