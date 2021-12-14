
/* RCS Info: $Revision: 1.4 $ on $Date: 91/12/19 16:57:59 $
 *           $Source: //pepper/atesse_spice/spice3/FTE/RCS/inp.c,v $
 * Copyright (c) 1985 Wayne A. Christopher
 *
 * Stuff for dealing with spice input decks and command scripts, and
 * the listing routines.
 */

#include "prefix.h"
#include "CPdefs.h"
#include "FTEdefs.h"
#include "FTEdata.h"
#include "FTEinp.h"

/* gtri - add - 12/12/90 - wbk - include new stuff */
#include "IPCtiein.h"
#include "util.h"
#include "ENH.h"
/* gtri - end - 12/12/90 */

#include "suffix.h"

static char *readline();
static bool doedit();

/* Read the entire input file and return  a pointer to the first line of 
 * the linked list of 'card' records in data.
 */

void
inp_readall(fp, data)
    FILE *fp;
    struct line **data;
{
    struct line *cc, *end = NULL, *prev = NULL, *working, *newcard;
    char *buffer, *s, *t;
    int line = 1;
    FILE *newfp;


/* gtri - modify - 12/12/90 - wbk - read from mailbox if ipc enabled */

    Ipc_Status_t    ipc_status;
    char            ipc_buffer[1025];  /* Had better be big enough */
    int             ipc_len;

    while (1) {

        /* If IPC is not enabled, do equivalent of what SPICE did before */
        if(! g_ipc.enabled) {
            buffer = readline(fp);
            if(! buffer)
                break;
        }
        else {
        /* else, get the line from the ipc channel. */
        /* We assume that newlines are not sent by the client */
        /* so we add them here */
            ipc_status = ipc_get_line(ipc_buffer, &ipc_len, IPC_WAIT);
            if(ipc_status == IPC_STATUS_END_OF_DECK) {
                buffer = NULL;
                break;
            }
            else if(ipc_status == IPC_STATUS_OK) {
                buffer = (void *) MALLOC(strlen(ipc_buffer) + 3);
                strcpy(buffer, ipc_buffer);
                strcat(buffer, "\n");
            }
            else {  /* No good way to report this so just die */
                exit(1);
            }
        }

/* gtri - end - 12/12/90 */

        if (*buffer == '@')
            break;
        for (s = buffer; *s && (*s != '\n'); s++)
            ;
        if (!*s) {
            fprintf(cp_err, "Warning: premature EOF...\n");
        }
        *s = '\0';      /* Zap the newline. */

        if (prefix(".include", buffer)) {
            for (s = buffer; *s && !isspace(*s); s++)
                ;
            while (isspace(*s))
                s++;
            if (!*s) {
                fprintf(cp_err, 
                "Error: .include filename missing\n");
                continue;
            }
            for (t = s; *t && !isspace(*t); t++)
                ;
            *t = '\0';
            if (*s == '~')
                s = cp_tildexpand(s);
            if (!(newfp = inp_pathopen(s, "r"))) {
                perror(s);
                continue;
            }
            inp_readall(newfp, &newcard);
            (void) fclose(newfp);

            /* Make the .include a comment... */
            *buffer = '*';
            if (end) {
                end->li_next = alloc(line);
                end = end->li_next;
            } else {
                end = cc = alloc(line);
            }
            end->li_line = copy(buffer);
            end->li_linenum = line++;
            end->li_next = newcard;

            /* Renumber the cards... */
            for (end = newcard; end->li_next; end = end->li_next)
                end->li_linenum = line++;

            /* Fix the buffer up a bit. */
            (void) strncpy(buffer + 1, "end of:", 7);
        }

        if (end) {
            end->li_next = alloc(line);
            end = end->li_next;
        } else {
            end = cc = alloc(line);
        }
        end->li_line = buffer;
        end->li_linenum = line++;
    }
    if (!end) { /* No stuff here... */
        *data = NULL;
        return;
    }

    /* Now make logical lines. */
    working = cc->li_next;      /* Skip title. */

    while (working) {
        switch (*working->li_line) {
            case '*':
            case '\0':
                prev = NULL;
                working = working->li_next;
                break;
            case '+':
                if (!prev) {
                    working->li_error = copy(
                    "Illegal continuation card: ignored.");
                    working = working->li_next;
                    break;
                }
                buffer = tmalloc(strlen(prev->li_line) + 
                        strlen(working->li_line) + 2);
                (void) sprintf(buffer, "%s %s", prev->li_line,
                        working->li_line + 1);
                s = prev->li_line;
                prev->li_line = buffer;
                prev->li_next = working->li_next;
                working->li_next = NULL;
                if (prev->li_actual) {
                    for (end = prev->li_actual;
                        end->li_next; end = end->li_next)
                        ;
                    end->li_next = working;
                    tfree(s);
                } else {
                    newcard = alloc(line);
                    newcard->li_linenum = prev->li_linenum;
                    newcard->li_line = s;
                    newcard->li_next = working;
                    prev->li_actual = newcard;
                }
                working = prev->li_next;
                break;
            default:
                prev = working;
                working = working->li_next;
                break;
        }
    }

    *data = cc;
    return;
}

/* Do a listing. Use is listing [expanded] [logical] [physical] [deck] */

void
com_listing(wl)
    wordlist *wl;
{
    int type = LS_LOGICAL;
    bool expand = false;
    char *s;

    if (ft_curckt) {
        while (wl) {
            s = wl->wl_word;
            switch (*s) {
                case 'l':
                case 'L':
                    type = LS_LOGICAL;
                    break;
                case 'p':
                case 'P':
                    type = LS_PHYSICAL;
                    break;
                case 'd':
                case 'D':
                    type = LS_DECK;
                    break;
                case 'e':
                case 'E':
                    expand = true;
                    break;
                default:
                    fprintf(cp_err,
                    "Error: bad listing type %s\n", s);
            }
            wl = wl->wl_next;
        }
        if (type != LS_DECK)
            fprintf(cp_out, "\t%s\n\n", ft_curckt->ci_name);
        inp_list(cp_out, expand ? ft_curckt->ci_deck :
                ft_curckt->ci_origdeck, ft_curckt->ci_options,
                type);
    } else
        fprintf(cp_err, "Error: no circuit loaded.\n");
    return;
}

static char *
upper(string)
    register char *string;
{

    static char buf[BSIZE];
    register char *s;

    s = buf;

    while (*string) {
#ifdef ASCIIQUOTE
        /* Let's make this really idiot-proof. */
        *s = strip(*string);
        if ((*s < ' ') && (*s != '\t'))
            *s = '_';   /* ? is bad since ? = help... */
#endif
        if (islower(*string))
            *s = toupper(*string);
        else
            *s = *string;
        string++;
        s++;
    }

    *s = '\0';

    return(buf);
}

/* Provide an input listing on the specified file of the given
 * card deck.  The listing should be of either LS_PHYSICAL or LS_LOGICAL
 * or LS_DECK lines as specified by the type parameter.
 */

void
inp_list(file, deck, extras, type)
    FILE *file;
    struct line *deck, *extras;
    int type;
{
    struct line *here;
    struct line *there;
    struct line *tmp, *next;
    bool renumber;
    bool useout = (file == cp_out);
    int i = 1;

/* gtri - wbk - 03/07/91 - Don't use 'more' type output if ipc enabled */

    if(g_ipc.enabled) {
        useout = FALSE;
    }

/* gtri - end - 03/07/91 */

#ifdef notdef
    /* splice extras into deck */
    for (here = extras, extras = NULL, next = here->li_next;
            here; here = next) {
      for (there = deck; there; there = there->li_next) {
    if (there->li_linenum < here->li_linenum &&
        (!there->li_next ||
        there->li_next->li_linenum > here->li_linenum)) {
      tmp = there->li_next;
      there->li_next = here;
      here->li_next = tmp;
      goto spliced;
    }
      }
      /* left over */
      if (extras) {
    extras->li_next = here;
      } else {
    extras = here;
      }
      here->li_next = NULL;
spliced: ;      /* dumb empty statement need */
    }
#endif

    if (useout)
        out_init();
    (void) cp_getvar("renumber", VT_BOOL, (char *) &renumber);
    if (type == LS_LOGICAL) {
top1:       for (here = deck; here; here = here->li_next) {
            if (renumber)
                here->li_linenum = i;
            i++;
            if (ciprefix(".end", here->li_line) &&
                    !isalpha(here->li_line[4]))
                continue;
            if (*here->li_line != '*') {
                if (useout) {
                    sprintf(out_pbuf, "%6d : %s\n",
                            here->li_linenum,
                            upper(here->li_line));
            out_send(out_pbuf);
/*                    out_printf("%6d : %s\n",
                            here->li_linenum,
                            upper(here->li_line)); */
                } else
                    fprintf(file, "%6d : %s\n",
                            here->li_linenum,
                            upper(here->li_line));
                if (here->li_error) {
                    if (useout) {
                        out_printf("%s\n",
                                here->li_error);
                    } else
                        fprintf(file, "%s\n",
                            here->li_error, file);
                }
            }
        }
        if (extras) {
            deck = extras;
            extras = NULL;
            goto top1;
        }
        if (useout) {
/*            out_printf("%6d : .end\n", i); */
            sprintf(out_pbuf, "%6d : .end\n", i);
        out_send(out_pbuf);
        } else
            fprintf(file, "%6d : .end\n", i);
    } else if ((type == LS_PHYSICAL) || (type == LS_DECK)) {
top2:       for (here = deck; here; here = here->li_next) {
            if ((here->li_actual == NULL) || (here == deck)) {
                if (renumber)
                    here->li_linenum = i;
                i++;
                if (ciprefix(".end", here->li_line) &&
                        !isalpha(here->li_line[4]))
                    continue;
                if (type == LS_PHYSICAL) {
                    if (useout) {
                        sprintf(out_pbuf, "%6d : %s\n",
                            here->li_linenum,
                            upper(here->li_line));
            out_send(out_pbuf);
                    } else
                        fprintf(file, "%6d : %s\n",
                            here->li_linenum,
                            upper(here->li_line));
                } else {
                    if (useout)
                        out_printf("%s\n",
                            upper(here->li_line));
                    else
                        fprintf(file, "%s\n",
                            upper(here->li_line));
                }
                if (here->li_error && (type == LS_PHYSICAL)) {
                    if (useout)
                        out_printf("%s\n",
                            here->li_error);
                    else
                        fprintf(file, "%s\n",
                            here->li_error);
                }
            } else {
                for (there = here->li_actual; there;
                        there = there->li_next) {
                    there->li_linenum = i++;
                    if (ciprefix(".end", here->li_line) &&
                            isalpha(here->li_line[4]))
                        continue;
                    if (type == LS_PHYSICAL) {
                        if (useout) {
                            sprintf("%6d : %s\n",
                                out_pbuf,
                                there->li_linenum,
                                upper(there->li_line));
                out_send(out_pbuf);
                        } else
                            fprintf(file, "%6d : %s\n",
                                there->li_linenum,
                                upper(there->li_line));
                    } else {
                        if (useout)
                            out_printf("%s\n",
                                upper(there->li_line));
                        else
                            fprintf(file, "%s\n",
                                upper(there->li_line));
                    }
                    if (there->li_error && 
                            (type == LS_PHYSICAL)) {
                        if (useout)
                            out_printf("%s\n",
                            there->li_error);
                        else
                            fprintf(file, "%s\n",
                            there->li_error);
                    }
                }
                here->li_linenum = i;
            }
        }
        if (extras) {
            deck = extras;
            extras = NULL;
            goto top2;
        }
        if (type == LS_PHYSICAL) {
            if (useout) {
                sprintf(out_pbuf, "%6d : .END\n", i);
            out_send(out_pbuf);
            } else
                fprintf(file, "%6d : .END\n", i);
        } else {
            if (useout)
                out_printf(".END\n");
            else
                fprintf(file, ".END\n");
        }
    } else
        fprintf(cp_err, "inp_list: Internal Error: bad type %d\n", 
                type);
    return;
}

void
inp_casefix(string)
    register char *string;
{

    while (*string) {
#ifdef ASCIIQUOTE
        /* Let's make this really idiot-proof. */
        *string = strip(*string);
        if ((*string < ' ') && (*string != '\t'))
            *string = '_';  /* ? is bad since ? = help... */
#endif
        if (isupper(*string))
            *string = tolower(*string);
        string++;
    }
    return;
}

/* The routine to source a spice input deck. We read the deck in, take out
 * the front-end commands, and create a CKT structure. Also we filter out
 * the following cards: .save, .width, .four, .print, and .plot, to perform
 * after the run is over.
 */

void
inp_spsource(fp, comfile, filename)
    FILE *fp;
    bool comfile;
    char *filename;
{
    struct line *deck, *dd, *ld;
    struct line *realdeck, *options;
    char *tt, name[BSIZE], *s, *t;
    bool nosubckts, commands = false;
    wordlist *wl = NULL, *end = NULL;
    wordlist *controls = NULL;
    FILE *lastin, *lastout, *lasterr;
    
    inp_readall(fp, &deck);
    if (!deck)
        return;
    
    if (!comfile)
        options = inp_getopts(deck);

    realdeck = inp_deckcopy(deck);

    if (!comfile) {
        /* Save the title before INPgetTitle gets it. */
        tt = copy(deck->li_line);
        if (!deck->li_next)
            fprintf(cp_err, "Warning: no cards in deck...\n");
    }
    (void) fclose(fp);

    /* Now save the IO context and start a new control set...  After
     * we are done with the source we'll put the old file descriptors 
     * back.  I guess we could use a FILE stack, but since this routine
     * is recursive anyway...
     */
    lastin = cp_curin;
    lastout = cp_curout;
    lasterr = cp_curerr;
    cp_curin = cp_in;
    cp_curout = cp_out;
    cp_curerr = cp_err;

    cp_pushcontrol();

    /* We should now go through the deck and execute front-end 
     * commands and remove them. Front-end commands are enclosed by
     * the cards .control and .endc, unless comfile
     * is true, in which case every line must be a front-end command.
     * There are too many problems with matching the first word on
     * the line.
     */
    ld = deck;
    if (comfile) {
        /* This is easy. */
        for (dd = deck; dd; dd = ld) {
            ld = dd->li_next;
            if ((dd->li_line[0] == '*') && (dd->li_line[1] != '#'))
                continue;
            if (!ciprefix(".control", dd->li_line) &&
                    !ciprefix(".endc", dd->li_line))
                if (dd->li_line[0] == '*')
                    (void) cp_evloop(dd->li_line + 2);
                else
                    (void) cp_evloop(dd->li_line);
            tfree(dd->li_line);
            tfree(dd);
        }   
    } else {
        for (dd = deck->li_next; dd; dd = ld->li_next) {
            if ((dd->li_line[0] == '*') &&
                    (dd->li_line[1] != '#')) {
                ld = dd;
                continue;
            }
            (void) strncpy(name, dd->li_line, BSIZE);
            for (s = name; *s && isspace(*s); s++)
                ;
            for (t = s; *t && !isspace(*t); t++)
                ;
            *t = '\0';

            if (ciprefix(".control", dd->li_line)) {
                ld->li_next = dd->li_next;
                tfree(dd->li_line);
                tfree(dd);
                if (commands)
                    fprintf(cp_err, 
                    "Warning: redundant .control card\n");
                else
                    commands = true;
            } else if (ciprefix(".endc", dd->li_line)) {
                ld->li_next = dd->li_next;
                tfree(dd->li_line);
                tfree(dd);
                if (commands)
                    commands = false;
                else
                    fprintf(cp_err, 
                    "Warning: misplaced .endc card\n");
            } else if (commands || prefix("*#", dd->li_line)) {
                wl = alloc(wordlist);
                if (controls) {
                    wl->wl_next = controls;
                    controls->wl_prev = wl;
                    controls = wl;
                } else
                    controls = wl;
                if (prefix("*#", dd->li_line))
                    wl->wl_word = copy(dd->li_line + 2);
                else
                    wl->wl_word = dd->li_line;
                ld->li_next = dd->li_next;
                tfree(dd);
            } else if (!*dd->li_line) {
                /* So blank lines in com files don't get
                 * considered as circuits.
                 */
                ld->li_next = dd->li_next;
                tfree(dd->li_line);
                tfree(dd);
            } else {
                inp_casefix(s);
                inp_casefix(dd->li_line);
                if (eq(s, ".width") || ciprefix(".four", s) ||
                        eq(s, ".plot") || 
                        eq(s, ".print") ||
                        eq(s, ".save")) {
                    if (end) {
                        end->wl_next = alloc(wordlist);
                        end->wl_next->wl_prev = end;
                        end = end->wl_next;
                    } else
                        wl = end = alloc(wordlist);
                    end->wl_word = copy(dd->li_line);
                    ld->li_next = dd->li_next;
                    tfree(dd->li_line);
                    tfree(dd);
                } else
                    ld = dd;
            }
        }
        if (deck->li_next) {
            /* There is something left after the controls. */

/* gtri - wbk - Comment out printing of circuit title */
            /* fprintf(cp_out, "\nCircuit: %s\n\n", tt);  */
/* gtri - end - Comment out printing of circuit title */

/* gtri - wbk - Translate all SPICE 2G6 polynomial type sources */
            deck->li_next = ENHtranslate_poly(deck->li_next);
/* gtri - end - Translate all SPICE 2G6 polynomial type sources */

            /* Now expand subcircuit macros. Note that we have to 
             * fix the case before we do this but after we 
             * deal with the commands.
             */
            if (!cp_getvar("nosubckt", VT_BOOL, (char *) 
                    &nosubckts))
                deck->li_next = inp_subcktexpand(deck->
                        li_next);
            deck->li_actual = realdeck;
            inp_dodeck(deck, tt, wl, false, options, filename);
        }

        /* Now that the deck is loaded, do the commands... */
        if (controls) {
            for (end = wl = wl_reverse(controls); wl;
                    wl = wl->wl_next)
                (void) cp_evloop(wl->wl_word);
            wl_free(end);
        }
    }

    /* Now reset everything.  Pop the control stack, and fix up the IO
     * as it was before the source.
     */
    cp_popcontrol();

    cp_curin = lastin;
    cp_curout = lastout;
    cp_curerr = lasterr;
    return;
}

/* This routine is cut in half here because com_rset has to do what follows
 * also. End is the list of commands we execute when the job is finished:
 * we only bother with this if we might be running in batch mode, since
 * it isn't much use otherwise.
 */

void
inp_dodeck(deck, tt, end, reuse, options, filename)
    struct line *deck;
    char *tt;
    wordlist *end;
    bool reuse;
    struct line *options;
    char *filename;
{
    struct circ *ct;
    struct line *dd;
    char *ckt, *tab, *s;
    struct variable *eev = NULL;
    wordlist *wl;
    bool noparse, ii;

    /* First throw away any old error messages there might be and fix
     * the case of the lines.
     */
    for (dd = deck; dd; dd = dd->li_next) {
        if (dd->li_error) {
            tfree(dd->li_error);
            dd->li_error = NULL;
        }
    }
    if (reuse) {
        ct = ft_curckt;
    } else {
        if (ft_curckt) {
            ft_curckt->ci_devices = cp_kwswitch(CT_DEVNAMES, 
                    (char *) NULL);
            ft_curckt->ci_nodes = cp_kwswitch(CT_NODENAMES, 
                    (char *) NULL);
        }
        ft_curckt = ct = alloc(circ);
    }
    (void) cp_getvar("noparse", VT_BOOL, (char *) &noparse);
    if (!noparse)
        ckt = if_inpdeck(deck, &tab);
    else
        ckt = NULL;

    out_init();
/* gtri - modify - 12/12/90 - wbk - add setting of ipc syntax error flag */
    for (dd = deck; dd; dd = dd->li_next)
        if (dd->li_error) {
            out_printf("Error on line %d : %s\n%s\n",
                dd->li_linenum, dd->li_line, dd->li_error);
            g_ipc.syntax_error = IPC_TRUE;
        }
/* gtri - end - 12/12/90 */

    /* Add this circuit to the circuit list. If reuse is true then 
     * use the ft_curckt structure.
     */

    if (!reuse) {
        for (dd = deck->li_next; dd; dd = dd->li_next)
            if_setndnames(dd->li_line);

        /* Be sure that ci_devices and ci_nodes are valid... */
        ft_curckt->ci_devices = cp_kwswitch(CT_DEVNAMES, 
                (char *) NULL);
        (void) cp_kwswitch(CT_DEVNAMES, ft_curckt->ci_devices);
        ft_curckt->ci_nodes = cp_kwswitch(CT_NODENAMES, (char *) NULL);
        (void) cp_kwswitch(CT_NODENAMES, ft_curckt->ci_nodes);
        ft_newcirc(ct);
        /* ft_setccirc(); */ ft_curckt = ct;
    }
    ct->ci_name = tt;
    ct->ci_deck = deck;
    ct->ci_options = options;
    if (deck->li_actual)
        ct->ci_origdeck = deck->li_actual;
    else
        ct->ci_origdeck = ct->ci_deck;
    ct->ci_ckt = ckt;
    ct->ci_symtab = tab;
    ct->ci_inprogress = false;
    ct->ci_runonce = false;
    ct->ci_commands = end;
    if (filename)
        ct->ci_filename = copy(filename);
    else
        ct->ci_filename = NULL;

    if (!noparse) {
        for (; options; options = options->li_next) {
            for (s = options->li_line; *s && !isspace(*s); s++)
                ;
            ii = cp_interactive;
            cp_interactive = false;
            wl = cp_lexer(s);
            cp_interactive = ii;
            if (!wl || !wl->wl_word || !*wl->wl_word)
                continue;
            if (eev)
                eev->va_next = cp_setparse(wl);
            else
                ct->ci_vars = eev = cp_setparse(wl);
            while (eev->va_next)
                eev = eev->va_next;
        }
        for (eev = ct->ci_vars; eev; eev = eev->va_next)
            switch (eev->va_type) {
                case VT_BOOL:
                if_option(ct->ci_ckt, eev->va_name, 
                    eev->va_type, (char *) NULL);
                break;
                case VT_NUM:
                if_option(ct->ci_ckt, eev->va_name, 
                    eev->va_type, (char *) &eev->va_num);
                break;
                case VT_REAL:
                if_option(ct->ci_ckt, eev->va_name, 
                    eev->va_type, (char *) &eev->va_real);
                break;
                case VT_STRING:
                if_option(ct->ci_ckt, eev->va_name, 
                    eev->va_type, eev->va_string);
                break;
        }
    }

    cp_addkword(CT_CKTNAMES, tt);
    return;
}

/* Edit and re-load the current input deck.  Note that if these commands are
 * used on a non-unix machine, they will leave spice.tmp junk files lying
 * around.
 */

void
com_edit(wl)
    wordlist *wl;
{
    char *filename;
    FILE *fp;
    bool inter, permfile;
    char buf[BSIZE];

    inter = cp_interactive;
    cp_interactive = false;
    if (wl) {
        if (!doedit(wl->wl_word)) {
            cp_interactive = inter;
            return;
        }
        if (!(fp = inp_pathopen(wl->wl_word, "r"))) {
            perror(wl->wl_word);
            cp_interactive = inter;
            return;
        }
        inp_spsource(fp, false, wl->wl_word);
    } else {
        /* If there is no circuit yet, then create one... */
        if (ft_curckt && ft_curckt->ci_filename) {
            filename = ft_curckt->ci_filename;
            permfile = true;
        } else {
#ifdef UNIX
            filename = mktemp("/tmp/spXXXXXX");
#else
            filename = "spice.tmp";
#endif
            permfile = false;
        }
        if (ft_curckt && !ft_curckt->ci_filename) {
#ifdef UNIX
            filename = mktemp("/tmp/spXXXXXX");
#else
            filename = "spice.tmp";
#endif
            permfile = false;
        }
        if (ft_curckt && !ft_curckt->ci_filename) {
            if (!(fp = fopen(filename, "w"))) {
                perror(filename);
                cp_interactive = inter;
                return;
            }
            inp_list(fp, ft_curckt->ci_deck, ft_curckt->ci_options,
                LS_DECK);
            fprintf(cp_err,
        "Warning: editing a temporary file -- circuit not saved\n");
            (void) fclose(fp);
        } else if (!ft_curckt) {
            if (!(fp = fopen(filename, "w"))) {
                perror(filename);
                cp_interactive = inter;
                return;
            }
            fprintf(fp, "SPICE 3 test deck\n");
            (void) fclose(fp);
        }
        if (!doedit(filename)) {
            cp_interactive = inter;
            return;
        }

#ifdef notdef           /* What do we do here?? */
        /* Blow the old circuit away. */
        if (ft_curckt) {
            /* Should free the old deck. */
            ft_curckt->ci_deck = ft_curckt->ci_origdeck =
                    ft_curckt->ci_options = NULL;
            ft_curckt->ci_commands = NULL;
            if_cktfree(ft_curckt->ci_ckt, ft_curckt->ci_symtab);
            if (ft_curckt == ft_circuits) {
                tfree(ft_curckt);
                ft_circuits = ft_curckt = ft_circuits->ci_next;
            } else {
                for (cc = ft_circuits; cc; cc = cc->ci_next)
                    if (cc->ci_next == ft_curckt) {
                        cc->ci_next =
                            ft_curckt->ci_next;
                        tfree(ft_curckt);
                        ft_curckt = cc;
                        break;
                    }
            }
        }
#endif
        if (!(fp = fopen(filename, "r"))) {
            perror(filename);
            cp_interactive = inter;
            return;
        }
        inp_spsource(fp, false, permfile ? filename : (char *) NULL);
        (void) fclose(fp);
#ifdef UNIX
        if (ft_curckt && !ft_curckt->ci_filename)
            (void) unlink(filename);
#endif
    }

    cp_interactive = inter;

    /* note: default is to run circuit after successful edit */

    fprintf(cp_out, "run circuit? ");
    fflush(cp_out);
    (void) gets(buf);
    if (buf[0] != 'n') {
      fprintf(cp_out, "running circuit\n");
      com_run(NULL);
    }

    return;
}

#ifndef DEF_EDITOR
#define DEF_EDITOR "vi"
#endif

static bool
doedit(filename)
    char *filename;
{
    char buf[BSIZE], buf2[BSIZE], *editor;

    if (cp_getvar("editor", VT_STRING, buf2)) {
        editor = buf2;
    } else {
        if (!(editor = getenv("EDITOR")))
            editor = DEF_EDITOR;
    }
    (void) sprintf(buf, "%s %s", editor, filename);
    return (system(buf) ? false : true);
}

void
com_source(wl)
    wordlist *wl;
{
    FILE *fp, *tp;
    char buf[BSIZE];
    bool inter;
    char *tempfile = NULL;
    wordlist *owl = wl;
    int i;

    inter = cp_interactive;
    cp_interactive = false;
    if (wl->wl_next) {
        /* There are several files -- put them into a temp file...  */
#ifdef UNIX
        tempfile = mktemp("/tmp/spXXXXXX");
#else
        tempfile = "spice.tmp";
#endif
        if (!(fp = inp_pathopen(tempfile, "w+"))) {
            perror(tempfile);
            cp_interactive = true;
            return;
        }
        while (wl) {
            if (!(tp = inp_pathopen(wl->wl_word, "r"))) {
                perror(wl->wl_word);
                (void) fclose(fp);
                cp_interactive = true;
#ifdef UNIX
                (void) unlink(tempfile);
#endif
                return;
            }
            while ((i = fread(buf, 1, BSIZE, tp)) > 0)
                (void) fwrite(buf, 1, i, fp);
            (void) fclose(tp);
            wl = wl->wl_next;
        }
        (void) fseek(fp, (long) 0, 0);
    } else
        fp = inp_pathopen(wl->wl_word, "r");
    if (fp == NULL) {
        perror(wl->wl_word);
        cp_interactive = true;
        return;
    }

    /* Don't print the title if this is a .spiceinit file. */
    if (ft_nutmeg || substring(".spiceinit", owl->wl_word)
            || substring("spice.rc", owl->wl_word))
        inp_spsource(fp, true, tempfile ? (char *) NULL : wl->wl_word);
    else
        inp_spsource(fp, false, tempfile ? (char *) NULL : wl->wl_word);
    cp_interactive = inter;
#ifdef UNIX
    if (tempfile)
        (void) unlink(tempfile);
#endif
    return;
}

/* Easy... */

void
inp_source(file)
    char *file;
{
    static struct wordlist wl = { NULL, NULL, NULL } ;

    wl.wl_word = file;
    com_source(&wl);
    return;
}

/*  This routine reads a line (of arbitrary length), up to a '\n' or 'EOF'
 *  and returns a pointer to the resulting null terminated string.
 *  The '\n' if found, is included in the returned string.
 *  From: jason@ucbopal.BERKELEY.EDU (Jason Venner)
 *  Newsgroups: net.sources
 */

#define STRGROW 256

static char *
readline(fd)
    FILE *fd;
{
    int c;
    int memlen;
    char *strptr;
    int strlen;
    
    strptr = NULL;
    strlen = 0;
    memlen = STRGROW; 
    strptr = tmalloc(memlen);
    memlen -= 1;          /* Save constant -1's in while loop */
    while((c = getc(fd)) != EOF) {
        strptr[strlen] = c;
        strlen++;
        if( strlen >= memlen ) {
            memlen += STRGROW;
            if( !(strptr = trealloc(strptr, memlen + 1))) {
                return (NULL);
            }
        }
        if (c == '\n') {
            break;
        }
    }
    if (!strlen) {
        free(strptr);
        return (NULL);
    }
    strptr[strlen] = '\0'; 
    /* Trim the string */
    strptr = trealloc(strptr, strlen + 1);
    return (strptr);
}

/* Look up the variable sourcepath and try everything in the list in order
 * if the file isn't in . and it isn't an abs path name.
 */

#ifdef IBMPC
#define SEPC '\\'
#endif
#ifdef UNIX
#define SEPC '/'
#endif
#ifdef VMS
#define SEPC ' '
#endif

FILE *
inp_pathopen(name, mode)
    char *name, *mode;
{
    FILE *fp;
    char buf[BSIZE];
    struct variable *v;

    /* If this is an abs pathname, or there is no sourcepath var, just
     * do an fopen.
     */
#ifdef VMS
    if (index(name, ']') || !cp_getvar("sourcepath", VT_LIST, (char *) &v))
        return (fopen(name, mode));
#else
    if (index(name, SEPC) || !cp_getvar("sourcepath", VT_LIST, (char *) &v))
        return (fopen(name, mode));
#endif

    while (v) {
        switch (v->va_type) {
            case VT_STRING:
            cp_wstrip(v->va_string);
            (void) sprintf(buf, "%s%c%s", v->va_string, SEPC, name);
            break;
            case VT_NUM:
            (void) sprintf(buf, "%d%c%s", v->va_num, SEPC, name);
            break;
            case VT_REAL:   /* This is foolish... */
            (void) sprintf(buf, "%lg%c%s", v->va_real, SEPC, name);
            break;
        }
        if (fp = fopen(buf, mode))
            return (fp);
        v = v->va_next;
    }
    return (NULL);
}

