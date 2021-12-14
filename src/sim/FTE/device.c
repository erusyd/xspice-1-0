
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:11:24 $
 *           $Source: //pepper/atesse_spice/spice3/FTE/RCS/device.c,v $
 * Copyright (c) 1986 Wayne A. Christopher, U. C. Berkeley CAD Group
 * 
 * Routines to query and alter devices.
 */

#include "prefix.h"
#include "CPdefs.h"
#include "FTEdefs.h"
#include "suffix.h"

static wordlist *devexpand();

/* Display various device parameters.  The syntax of this command is 
 *   show devicelist : parmlist
 * where devicelist can be "all", the name of a device, a string like r*,
 * which means all devices with names that begin with 'r', repeated one
 * or more times.   The parms are names of parameters that are (hopefully)
 * valid for all the named devices, or "all".
 */

void
com_show(wl)
    wordlist *wl;
{
    wordlist *devs, *parms, *tw, *ww;
    struct variable *v;
    char *nn;

    if (!ft_curckt) {
        fprintf(cp_err, "Error: no circuit loaded\n");
        return;
    }
    devs = wl;
    while (wl && !eq(wl->wl_word, ":"))
        wl = wl->wl_next;
    if (!wl)
        parms = NULL;
    else {
        if (wl->wl_prev)
            wl->wl_prev->wl_next = NULL;
        parms = wl->wl_next;
        if (parms)
            parms->wl_prev = NULL;
    }

    /* Now expand the devicelist... */
    for (tw = NULL; devs; devs = devs->wl_next) {
        inp_casefix(devs->wl_word);
        tw = wl_append(tw, devexpand(devs->wl_word));
    }
    
    devs = tw;
    for (tw = parms; tw; tw = tw->wl_next)
        if (eq(tw->wl_word, "all"))
            break;
    if (tw)
        parms = NULL;

    /* This is a crock... */
    if (!devs)
        devs = cp_cctowl(ft_curckt->ci_devices);

    out_init();

    while (devs) {
        out_printf("%s:\n", devs->wl_word);
        if (parms) {
            for (tw = parms; tw; tw = tw->wl_next) {
                nn = copy(devs->wl_word);
                v = if_getparam(ft_curckt->ci_ckt,
                        &nn, tw->wl_word, 0);
                if (v) {
                    out_printf("\t%s =", tw->wl_word);
                    for (ww = cp_varwl(v); ww; ww =
                            ww->wl_next)
                        out_printf(" %s", ww->wl_word);
                    out_send("\n");
                }
            }
        } else {
            nn = copy(devs->wl_word);
            v = if_getparam(ft_curckt->ci_ckt, &nn, "all", 0);
            while (v) {
                out_printf("\t%s =", v->va_name);
                for (ww = cp_varwl(v); ww; ww = ww->wl_next)
                    out_printf(" %s", ww->wl_word);
                out_send("\n");
                v = v->va_next;
            }
        }
        devs = devs->wl_next;
    }
    return;
}

/* Alter a device parameter.  The syntax here is
 *   alter devicelist parmname value
 * where devicelist is as above, parmname is the name of the desired parm
 * and value is a string, numeric, or bool value.
 */

void
com_alter(wl)
    wordlist *wl;
{
    wordlist *devs, *tw;
    struct variable var;
    char *parm, *val;
    double *dd;

    if (!ft_curckt) {
        fprintf(cp_err, "Error: no circuit loaded\n");
        return;
    }
    devs = wl;

    /* Now snag the parm name and value -- for now the value is one word.
     * It would be a good idea to allow expressions here.
     */
    while (wl->wl_next) {
        parm = wl->wl_word;
        val = wl->wl_next->wl_word;
        wl = wl->wl_next;
    }
    wl->wl_prev->wl_next = NULL;

    /* Now expand the devicelist... */
    for (tw = NULL; devs; devs = devs->wl_next) {
        inp_casefix(devs->wl_word);
        tw = wl_append(tw, devexpand(devs->wl_word));
    }
    
    devs = tw;

    /* Now figure out what the value should be... */
    if (eq(val, "true")) {
        var.va_type = VT_BOOL;
        var.va_bool = true;
    } else if (eq(val, "false")) { 
        var.va_type = VT_BOOL;
        var.va_bool = false;
    } else if (dd = ft_numparse(&val, false)) {
        var.va_type = VT_REAL;
        var.va_real = *dd;
    } else {
        var.va_type = VT_STRING;
        var.va_string = val;
    }

    while (devs) {
        if_setparam(ft_curckt->ci_ckt, devs->wl_word, parm, &var);
        devs = devs->wl_next;
    }
    return;
}

/* Given a device name, possibly with wildcards, return the matches. */

static wordlist *
devexpand(name)
    char *name;
{
    wordlist *wl, *devices, *tw;

    if (index(name, '*') || index(name, '[') || index(name, '?')) {
        devices = cp_cctowl(ft_curckt->ci_devices);
        for (wl = NULL; devices; devices = devices->wl_next)
            if (cp_globmatch(name, devices->wl_word)) {
                tw = alloc(wordlist);
                if (wl) {
                    wl->wl_prev = tw;
                    tw->wl_next = wl;
                    wl = tw;
                } else
                    wl = tw;
                wl->wl_word = devices->wl_word;
            }
    } else if (eq(name, "all")) {
        wl = cp_cctowl(ft_curckt->ci_devices);
    } else {
        wl = alloc(wordlist);
        wl->wl_word = name;
    }
    wl_sort(wl);
    return (wl);
}

