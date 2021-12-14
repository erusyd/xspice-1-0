
/* RCS Info: $Revision: 1.2 $ on $Date: 91/12/19 16:58:07 $
 *           $Source: //pepper/atesse_spice/spice3/FTE/RCS/runcoms.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Circuit simulation commands.
 */

#include "prefix.h"
#include "CPdefs.h"
#include "FTEdefs.h"
#include "FTEdata.h"

/* gtri - add - 12/12/90 - wbk - include ipc stuff */
#include "IPCtiein.h"
/* gtri - end - 12/12/90 */

#include "suffix.h"

static void dosim();

/* Routines for the commands op, tran, ac, dc, listing, device, state,
 * resume, stop, trace, run, end.  Op, tran, ac, and dc cause the action
 * to be performed immediately, and run causes whatever actions were
 * present in the deck to be carried out. End has the effect of stopping
 * any simulations in progress, as opposed to ending te input deck as
 * the .end card does.
 */

static FILE *rawfileFp;
static bool rawfileBinary;

void
com_scirc(wl)
    wordlist *wl;
{
    struct circ *p;
    int i, j = 0;
    char buf[BSIZE];

    if (ft_circuits == NULL) {
        fprintf(cp_err, "Error: there aren't any circuits loaded.\n");
        return;
    }
    if (wl == NULL) {
        fprintf(cp_out, 
            "\tType the number of the desired circuit:\n\n");
        for (p = ft_circuits; p; p = p->ci_next) {
            if (ft_curckt == p)
                fprintf(cp_out, "Current");
            fprintf(cp_out, "\t%d\t%s\n", ++j, p->ci_name);
        }
        fprintf(cp_out, "? ");
        (void) fflush(cp_out);
        (void) fgets(buf, BSIZE, cp_in);
        clearerr(cp_in);
        if ((sscanf(buf, " %d ", &i) != 1) || (i < 0) || (i > j))
            return;
        for (p = ft_circuits; --i > 0; p = p->ci_next);
    } else {
        for (p = ft_circuits; p; p = p->ci_next)
            if (ciprefix(wl->wl_word, p->ci_name))
                break;
        if (p == NULL) {
            fprintf(cp_err, "Warning: no such circuit as %s...\n",
                    wl->wl_word);
            return;
        }
        fprintf(cp_out, "\t%s\n", p->ci_name);
    }
    if (ft_curckt) {
        /* Actually this can't be false... */
        ft_curckt->ci_devices = 
                cp_kwswitch(CT_DEVNAMES, p->ci_devices);
        ft_curckt->ci_nodes = cp_kwswitch(CT_NODENAMES, p->ci_nodes);
    }
    ft_curckt = p;
    return;
}

void
com_pz(wl)
    wordlist *wl;
{
    dosim("pz", wl);
    return;
}

void
com_op(wl)
    wordlist *wl;
{
    dosim("op", wl);
    return;
}

void
com_dc(wl)
    wordlist *wl;
{
    dosim("dc", wl);
    return;
}

void
com_ac(wl)
    wordlist *wl;
{
    dosim("ac", wl);
    return;
}

void
com_tran(wl)
    wordlist *wl;
{
    dosim("tran", wl);
    return;
}

void
com_sens(wl)
    wordlist *wl;
{
    dosim("sens", wl);
    return;
}

static void
dosim(what, wl)
    char *what;
    wordlist *wl;
{
    wordlist *ww;
    bool dofile = false;
    char buf[BSIZE];
    struct circ *ct;
    bool ascii = true;
/*  bool oct = false;   */

    if (eq(what, "run") && wl)
        dofile = true;
    if (!dofile) {
        ww = alloc(wordlist);
        ww->wl_next = wl;
        if (wl)
            wl->wl_prev = ww;
        ww->wl_word = copy(what);
    }

    if (cp_getvar("filetype", VT_STRING, buf)) {
        if (eq(buf, "binary"))
            ascii = false;
#ifdef notdef
        else if (eq(buf, "oct"))
            oct = true;
#endif
        else if (!eq(buf, "ascii"))
            fprintf(cp_err, "Warning: strange file type %s\n",
                    buf);
    }

    if (!ft_curckt) {
        fprintf(cp_err, "Error: there aren't any circuits loaded.\n");
        return;
    } else if (ft_curckt->ci_ckt == NULL) { /* Set noparse? */
        fprintf(cp_err, "Error: circuit not parsed.\n");
        return;
    }
    for (ct = ft_circuits; ct; ct = ct->ci_next)
        if (ct->ci_inprogress && (ct != ft_curckt)) {
            fprintf(cp_err, 
                "Warning: losing old state for circuit '%s'\n",
                ct->ci_name);
            ct->ci_inprogress = false;
        }
    if (ft_curckt->ci_inprogress && !eq(what, "resume")) {
        ft_setflag = true;
        ft_intrpt = false;
        fprintf(cp_err, "Warning: resuming run in progress.\n");
        com_resume((wordlist *) NULL);
        ft_setflag = false;
        return;
    }
    if (ft_curckt->ci_runonce)
        com_rset((wordlist *) NULL);

    /* From now on until the next prompt, an interrupt will just
     * set a flag and let spice finish up, then control will be
     * passed back to the user.
     */
    ft_setflag = true;
    ft_intrpt = false;
    if (dofile) {
        if (!(rawfileFp = fopen(wl->wl_word, "w"))) {
            perror(wl->wl_word);
            ft_setflag = false;
            return;
        }
        rawfileBinary = !ascii;
    } else {
        rawfileFp = NULL;
        plot_num++; /* There should be a better way... */
    }

    /* Spice calls wrd_init and wrd_end itself... */
    ft_curckt->ci_inprogress = true;
    if(eq(what,"sens")){
       if (if_sens_run(ft_curckt->ci_ckt, ww, ft_curckt->ci_symtab) == 1) {
        /* The circuit was interrupted somewhere. */
        fprintf(cp_err, "%s simulation interrupted\n", what);

        /* gtri - add - 12/12/90 - wbk - record error and return errchk */
        g_ipc.run_error = IPC_TRUE;
        if(g_ipc.enabled)
            ipc_send_errchk();
        /* gtri - end - 12/12/90 */

        }
            else
        ft_curckt->ci_inprogress = false;
        }
    else{
        if (if_run(ft_curckt->ci_ckt, what, ww, ft_curckt->ci_symtab) == 1) {
        /* The circuit was interrupted somewhere. */
        fprintf(cp_err, "%s simulation interrupted\n", what);

        /* gtri - add - 12/12/90 - wbk - record error and return errchk */
        g_ipc.run_error = IPC_TRUE;
        if(g_ipc.enabled)
            ipc_send_errchk();
        /* gtri - end - 12/12/90 */

         } 
         else
         ft_curckt->ci_inprogress = false;
        }
        if (rawfileFp)
        (void) fclose(rawfileFp);
    ft_curckt->ci_runonce = true;
    ft_setflag = false;
    return;
}

/* Continue a simulation. If there is non in progress, this is the
 * equivalent of "run".
 */

/* ARGSUSED */
void
com_resume(wl)
    wordlist *wl;
{
    if (ft_curckt->ci_inprogress == false) {
        fprintf(cp_err, "Warning: run starting...\n");
        com_run((wordlist *) NULL);
        return;
    }
    ft_curckt->ci_inprogress = true;
    if (if_run(ft_curckt->ci_ckt, "resume", (wordlist *) NULL,
            ft_curckt->ci_symtab)) {
        /* The circuit was interrupted somewhere. */
        fprintf(cp_err, "simulation interrupted\n");
    } else
        ft_curckt->ci_inprogress = false;
    return;
}

/* Usage is run [filename] Do the wrd_{open,run} and wrd_(void) close
 * here, and let the CKT stuff do wrd_init and wrd_end.
 */

void
com_run(wl)
    wordlist *wl;
{
/*    ft_getsaves(); */
    dosim("run", wl);
    return;
}

void
ft_dorun(file)
    char *file;
{
    static wordlist wl = { NULL, NULL, NULL } ;

    wl.wl_word = file;
    if (file)
        dosim("run", &wl);
    else
        dosim("run", (wordlist *) NULL);
    return;
}

/* Throw out the circuit struct and recreate it from the deck.  This command
 * should be obsolete.
 */

/* ARGSUSED */
void
com_rset(wl)
    wordlist *wl;
{
    if (ft_curckt == NULL) {
        fprintf(cp_err, "Error: there is no circuit loaded.\n");
        return;
    }

    if_cktfree(ft_curckt->ci_ckt, ft_curckt->ci_symtab);
    inp_dodeck(ft_curckt->ci_deck, ft_curckt->ci_name, (wordlist *) NULL,
            true, ft_curckt->ci_options, ft_curckt->ci_filename);
    return;
}

/* ARGSUSED */ /* until the else clause gets put back */
bool
ft_getOutReq(fpp, plotp, binp, name, title)
    FILE **fpp;
    struct plot **plotp;
    bool *binp;
    char *name, *title;
{
    /*struct plot *pl;*/

    if (rawfileFp) {
        *fpp = rawfileFp;
        *binp = rawfileBinary;
        return (true);
    } else {
/*
        pl = plot_alloc(name);
        pl->pl_title = copy(title);
        pl->pl_name = copy(name);
        pl->pl_date = copy(datestring());

        pl->pl_next = plot_list;
        plot_list = pl;
        plot_cur = pl;

        *plotp = pl;
*/
        return (false);
    }
}

