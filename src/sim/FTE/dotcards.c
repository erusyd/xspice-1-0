
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:11:33 $
 *           $Source: //pepper/atesse_spice/spice3/FTE/RCS/dotcards.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Spice-2 compatibility stuff for .plot, .print, .four, and .width.
 */

#include "prefix.h"
#include "CPdefs.h"
#include "FTEdefs.h"
#include "FTEdata.h"
#include "FTEinp.h"
#include "suffix.h"

static wordlist *gettoks();
static void fixdotplot();
static void fixdotprint();
static char *fixem();
static bool setcplot();

bool ft_acctprint = false, ft_listprint = false;
bool ft_nodesprint = false, ft_optsprint = false;
bool ft_nopage = false;

/* Extract all the .save cards... */

void
ft_dotsaves()
{
    wordlist *card, *wl = NULL;
    char *s;

    if (!ft_curckt) /* Shouldn't happen. */
        return;

    for (card = ft_curckt->ci_commands; card; card = card->wl_next) {
        if (ciprefix(".save", card->wl_word)) {
            s = card->wl_word;
            (void) gettok(&s);
            wl = wl_append(wl, gettoks(s));
        }
    }
/*  wrd_saves = wl; */

    com_save(wl);
/* fprintf(stderr, "error, .save card code is broken\n"); */
    return;
}

/* Go through the dot cards given and make up a big "save" command with
 * all the node names mentioned. Note that if a node is requested for
 * one analysis, it is saved for all of them...
 */

void
ft_savedotargs()
{
    wordlist *w, *wl = NULL, *card;
    char *s;

    if (!ft_curckt) /* Shouldn't happen. */
        return;

    for (card = ft_curckt->ci_commands; card; card = card->wl_next) {
        s = card->wl_word;
        if (ciprefix(".print", s) || ciprefix(".plot", s) ||
                ciprefix(".four", s)) {
            (void) gettok(&s);
            (void) gettok(&s);
            if (!(w = gettoks(s)))
                fprintf(cp_err, "Warning: no nodes given: %s\n",
                        card->wl_word);
            wl = wl_append(wl, w);
        }
    }
    if (!wl) {
        /* If there weren't any nodes mentioned, put a dummy arg
         * in so that writedata will abort.
         */
        wl = alloc(wordlist);
        wl->wl_word = copy("");
    }
    com_save(wl);
    return;
}

/* Execute the .whatever cards found in the deck, after we are done running.
 * We'll be cheap and use cp_lexer to get the words... This should make us
 * spice-2 compatible.  Is terse is true then there was a rawfile, so don't
 * print lots of junk.
 */

void
ft_cktcoms(terse)
    bool terse;
{
    wordlist *coms, *command;
    char *plottype, *s;
    struct dvec *v;
    static wordlist twl = { "col", NULL, NULL } ;
    int i;

    if (!ft_curckt)
	return;
    if (!ft_curckt->ci_commands)
        goto nocmds;
    coms = ft_curckt->ci_commands;
    cp_interactive = false;

    /* Circuit name... */
    fprintf(cp_out, "Circuit: %s\nDate: %s\n\n", ft_curckt->ci_name,
            datestring());
    fprintf(cp_out, "\n");

    /* Listing if necessary... */
    if (ft_listprint)
        inp_list(cp_out, ft_curckt->ci_deck, ft_curckt->ci_options,
                LS_DECK);

    /* If there was a .op card, then we have to do the .op output. */
    if (setcplot("op")) {
        if (terse) {
            fprintf(cp_out, "\nop information in rawfile\n");
        } else {
            fprintf(cp_out, "\nOperating point information:\n\n");
            fprintf(cp_out, "\tNode\tVoltage\n");
            fprintf(cp_out, "\t----\t-------\n");
            for (v = plot_cur->pl_dvecs; v; v = v->v_next) {
                if (!isreal(v)) {
                    fprintf(cp_err, 
                "Internal error: op vector %s not real\n",
                            v->v_name);
                    continue;
                }
                if (v->v_type == SV_VOLTAGE)
                    fprintf(cp_out, "\t%s\t%s\n", v->v_name,
                        printnum(v->v_realdata[0]));
            }
            fprintf(cp_out, "\n\tSource\tCurrent\n");
            fprintf(cp_out, "\t------\t-------\n\n");
            for (v = plot_cur->pl_dvecs; v; v = v->v_next)
                if (v->v_type == SV_CURRENT)
                    fprintf(cp_out, "\t%s\t%s\n", v->v_name,
                        printnum(v->v_realdata[0]));
            fprintf(cp_out, "\n");
        }
    }

    /* Now all the . cards... */
    while (coms) {
        command = cp_lexer(coms->wl_word);
        if (!command)
            goto bad;
        if (eq(command->wl_word, ".width")) {
            do {
                command = command->wl_next;
            } while (command && !ciprefix("out", command->wl_word));
            if (command) {
                s = index(command->wl_word, '=');
                if (!s || !s[1]) {
                    fprintf(cp_err,
                        "Error: bad line %s\n",
                        coms->wl_word);
                    coms = coms->wl_next;
                    continue;
                }
                i = atoi(++s);
                cp_vset("width", VT_NUM, (char *) &i);
            }
        } else if (eq(command->wl_word, ".print")) {
            if (terse) {
                fprintf(cp_out, 
            "\n.print card ignored since rawfile was produced\n");
            } else {
                command = command->wl_next;
                if (!command) {
                    fprintf(cp_err, "Error: bad line %s\n",
                            coms->wl_word);
                    coms = coms->wl_next;
                    continue;
                }
                plottype = command->wl_word;
                command = command->wl_next;
                fixdotprint(command);
                twl.wl_next = command;
                if (setcplot(plottype)) 
                    com_print(&twl);
                else
                    fprintf(cp_err,
                        "Error: no %s plot found\n",
                        plottype);
                fprintf(cp_out, "\n\n");
            }
        } else if (eq(command->wl_word, ".plot")) {
            if (terse) {
                fprintf(cp_out, 
            "\n.plot card ignored since rawfile was produced\n");
            } else {
                command = command->wl_next;
                if (!command) {
                    fprintf(cp_err, "Error: bad line %s\n",
                            coms->wl_word);
                    coms = coms->wl_next;
                    continue;
                }
                plottype = command->wl_word;
                command = command->wl_next;
                fixdotplot(command);
                if (setcplot(plottype))
                    com_asciiplot(command);
                else
                    fprintf(cp_err,
                        "Error: no %s plot found\n",
                        plottype);
                fprintf(cp_out, "\n\n");
            }
        } else if (ciprefix(".four", command->wl_word)) {
            com_fourier(command->wl_next);
            fprintf(cp_out, "\n\n");
        } else if (!eq(command->wl_word, ".save"))
            goto bad;
        coms = coms->wl_next;
    }

nocmds:
    /* Now the node table... */
    if (ft_nodesprint)
        ;
    
    /* The options... */
    if (ft_optsprint) {
        fprintf(cp_err, "Options:\n\n");
        cp_vprint();
        (void) putc('\n', cp_out);
    }

    /* And finally the accounting info. */
    if (ft_acctprint) {
        static wordlist ww = { "everything", NULL, NULL } ;
        com_rusage(&ww);
    } else
        com_rusage((wordlist *) NULL);

    (void) putc('\n', cp_out);
    return;

bad:    fprintf(cp_err, "Internal Error: ft_cktcoms: bad commands\n");
    return;
}

/* These routines make sure that the arguments to .plot and .print in
 * spice2 decks are acceptable to spice3. The things we look for are:
 *  trailing (a,b) in .plot -> xlimit a b
 *  vm(x) -> mag(v(x))
 *  vp(x) -> ph(v(x))
 *  v(x,0) -> v(x)
 *  v(0,x) -> -v(x)
 */

static void
fixdotplot(wl)
    wordlist *wl;
{
    char buf[BSIZE], *s;
    double *d, d1, d2;

    while (wl) {
        wl->wl_word = fixem(wl->wl_word);

        /* Is this a trailing (a,b) ? Note that we require it to be
         * one word...
         */
        if (!wl->wl_next && (*wl->wl_word == '(')) /*)*/ {
            s = wl->wl_word + 1;
            d = ft_numparse(&s, false);
            if (*s != ',') {
                fprintf(cp_err, "Error bad limits %s...\n",
                        wl->wl_word);
                    return;
            }
            d1 = *d;
            s++;
            d = ft_numparse(&s, false);
            if ((*s != /*(*/ ')') || s[1]) {
                fprintf(cp_err, "Error bad limits %s...\n",
                        wl->wl_word);
                    return;
            }
            d2 = *d;
            tfree(wl->wl_word);
            wl->wl_word = copy("xlimit");
            wl->wl_next = alloc(wordlist);
            wl->wl_next->wl_prev = wl;
            wl = wl->wl_next;
            (void) strcpy(buf, printnum(d1));
            wl->wl_word = copy(buf);
            wl->wl_next = alloc(wordlist);
            wl->wl_next->wl_prev = wl;
            wl = wl->wl_next;
            (void) strcpy(buf, printnum(d2));
            wl->wl_word = copy(buf);
        }
        wl = wl->wl_next;
    }
    return;
}

static void
fixdotprint(wl)
    wordlist *wl;
{
    while (wl) {
        wl->wl_word = fixem(wl->wl_word);
        wl = wl->wl_next;
    }
    return;
}

static char *
fixem(string)
    char *string;
{
    char buf[BSIZE], *s, *t, *ss = string;

    if (ciprefix("v(", string) && index(string, ',')) {
        for (s = string; *s && (*s != ','); s++)
            ;
        *s++ = '\0';
        for (t = s; *t && (*t != ')'); t++)
            ;
        *t = '\0';
        if (eq(s, "0"))
            (void) sprintf(buf, "v(%s)", string + 2);
        else if (eq(string + 2, "0"))
            (void) sprintf(buf, "-v(%s)", s);
        else
            (void) sprintf(buf, "v(%s)-v(%s)", string + 2, s);
        tfree(ss);
        string = copy(buf);
    } else if (ciprefix("vm(", string)) {
        for (s = string; *s && (*s != ')'); s++)
            ;
        *s = '\0';
        (void) sprintf(buf, "mag(v(%s))", string + 3);
        tfree(ss);
        string = copy(buf);
    } else if (ciprefix("vp(", string)) {
        for (s = string; *s && (*s != ')'); s++)
            ;
        *s = '\0';
        (void) sprintf(buf, "ph(v(%s))", string + 3);
        tfree(ss);
        string = copy(buf);
    } else if (ciprefix("vi(", string)) {
        for (s = string; *s && (*s != ')'); s++)
            ;
        *s = '\0';
        (void) sprintf(buf, "imag(v(%s))", string + 3);
        tfree(ss);
        string = copy(buf);
    } else if (ciprefix("vr(", string)) {
        for (s = string; *s && (*s != ')'); s++)
            ;
        *s = '\0';
        (void) sprintf(buf, "real(v(%s))", string + 3);
        tfree(ss);
        string = copy(buf);
    } else if (ciprefix("vdb(", string)) {
        for (s = string; *s && (*s != ')'); s++)
            ;
        *s = '\0';
        (void) sprintf(buf, "db(v(%s))", string + 4);
        tfree(ss);
        string = copy(buf);
    } else if (ciprefix("i(", string)) {
        for (s = string; *s && (*s != ')'); s++)
            ;
        *s = '\0';
        string += 2;
        (void) sprintf(buf, "%s#branch", string);
        tfree(ss);
        string = copy(buf);
    }
    return (string);
}

/* Don't bother with ccom strangeness here... */

static bool
setcplot(name)
    char *name;
{
    struct plot *pl;

    for (pl = plot_list; pl; pl = pl->pl_next) {
        if (ciprefix(name, pl->pl_typename)) {
            plot_cur = pl;
            return (true);
        }
    }
    return (false);
}

static wordlist *
gettoks(s)
    char *s;
{
    char *t, *r, buf[64];
    wordlist *wl = NULL, *end = NULL;
    bool iflag;

    while (t = gettok(&s)) {
        if (*t == '(' /* ) */) {
            /* This is a (upper, lower) thing -- ignore. */
            continue;
        } else if (!index(t, '(' /*)*/ )) {
            if (end) {
                end->wl_next = alloc(wordlist);
                end->wl_next->wl_prev = end;
                end = end->wl_next;
            } else
                wl = end = alloc(wordlist);
            end->wl_word = copy(t);
        } else if (!index(t, ',')) {
            iflag = ((*t == 'i') || (*t == 'I')) ? true : false;
            while (*t != '(' /*)*/)
                t++;
            t++;
            for (r = t; *r && *r != /*(*/ ')'; r++)
                ;
            *r = '\0';
            if (end) {
                end->wl_next = alloc(wordlist);
                end->wl_next->wl_prev = end;
                end = end->wl_next;
            } else
                wl = end = alloc(wordlist);
            if (iflag) {
                (void) sprintf(buf, "%s#branch", t);
                t = buf;
            }
            end->wl_word = copy(t);
        } else {
            /* The painful case... */
            while (*t != '(' /*)*/)
                t++;
            t++;
            for (r = t; *r && *r != ','; r++)
                ;
            *r = '\0';
            if (end) {
                end->wl_next = alloc(wordlist);
                end->wl_next->wl_prev = end;
                end = end->wl_next;
            } else
                wl = end = alloc(wordlist);
            end->wl_word = copy(t);
            t = r + 1;
            for (r = t; *r && *r != /*(*/ ')'; r++)
                ;
            *r = '\0';
            if (end) {
                end->wl_next = alloc(wordlist);
                end->wl_next->wl_prev = end;
                end = end->wl_next;
            } else
                wl = end = alloc(wordlist);
            end->wl_word = copy(t);
        }
    }
    return (wl);
}

