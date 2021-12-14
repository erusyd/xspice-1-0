
/* RCS Info: $Revision: 1.1 $ on $Date: 91/04/02 12:04:31 $
 *           $Source: //pepper/atesse_spice/spice3/CP/RCS/variable.c,v $
 * Copyright (c) 1985 Wayne A. Christopher, U. C. Berkeley CAD Group
 *
 * Do variable substitution.
 */

#include "prefix.h"
#include "CPdefs.h"
#include "suffix.h"

static wordlist *vareval();
static int vcmp();

/* A variable substitution is
 * indicated by a $, and the variable name is the following string of
 * non-special characters. All variable values are inserted as a single
 * word, except for lists, which are a list of words.
 * A routine cp_usrset must be supplied by the host program to deal 
 * with variables that aren't used by cshpar -- it should be 
 * cp_usrset(var, isset), where var is a variable *, and isset is 
 * true if the variable is being set, false if unset.
 * Also required is a routine cp_enqvar(name) which returns a struct
 * variable *, which allows the host program to provide values for
 * non-cshpar variables.
 */

bool cp_noglob = true;      /* This is just a bad thing. */
bool cp_nonomatch = false;
bool cp_noclobber = false;
bool cp_ignoreeof = false;

char cp_dol = '$';

static struct variable *variables = NULL;

/* Non-alphanumeric characters that may appear in variable names. < is very
 * special...
 */

#define VALIDCHARS "$-_<#?@.()[]&"

wordlist *
cp_variablesubst(wlist)
    wordlist *wlist;
{
    wordlist *wl, *nwl;
    char *s, *t, buf[BSIZE], wbuf[BSIZE];
    int i;

    for (wl = wlist; wl; wl = wl->wl_next) {
        t = wl->wl_word;
        i = 0;
        while(s = index(t, cp_dol)) {
            while (t < s)
                wbuf[i++] = *t++;
            wbuf[i] = '\0';
            (void) strcpy(buf, ++s);
            s = buf;
            t++;
            while (*s && (isalphanum(*s) ||
                    index(VALIDCHARS, *s))) {
                /* Get s and t past the end of the var name. */
                t++;
                s++;
            }
            *s = '\0';
            nwl = vareval(buf);
            if (i) {
                (void) strcpy(buf, wbuf);
                if (nwl) {
                    (void) strcat(buf, nwl->wl_word);
                    tfree(nwl->wl_word);
                } else {
                    nwl = alloc(wordlist);
                }
                nwl->wl_word = copy(buf);
            }
            if (!(wl = wl_splice(wl, nwl)))
                return (NULL);
            /* This is bad... */
            for (wlist = wl; wlist->wl_prev; wlist = wlist->wl_prev)
                ;
            (void) strcpy(buf, wl->wl_word);
            i = strlen(buf);
            (void) strcat(buf, t);
            tfree(wl->wl_word);
            wl->wl_word = copy(buf);
            t = &wl->wl_word[i];
            s = wl->wl_word;
            for (i = 0; s < t; s++)
                wbuf[i++] = *s;
        }
    }
    return (wlist);
}

/* Evaluate a variable. */

static wordlist *
vareval(string)
    char *string;
{
    struct variable *v;
    wordlist *wl;
    char buf[BSIZE], *s;
    char *oldstring = copy(string);
    char *range = NULL;
    int i, up, low;

    cp_wstrip(string);
    if (s = index(string, '[')) {
        *s = '\0';
        range = s + 1;
    }

    switch (*string) {

        case '$':
        wl = alloc(wordlist);
#ifdef UNIX
        (void) sprintf(buf, "%d", getpid());
#else
        (void) strcpy(buf, "Sorry, this isn't UNIX...\n");
#endif
        wl->wl_word = copy(buf);
        return (wl);

        case '<':
        (void) fflush(cp_out);
        if (!fgets(buf, BSIZE, cp_in)) {
            clearerr(cp_in);
            (void) strcpy(buf, "EOF");
        }
        for (s = buf; *s && (*s != '\n'); s++)
            ;
        *s = '\0';
        wl = cp_lexer(buf);
        /* This is a hack. */
        if (!wl->wl_word)
            wl->wl_word = copy("");
        return (wl);
    
        case '?':
        wl = alloc(wordlist);
        string++;
        for (v = variables; v; v = v->va_next)
            if (eq(v->va_name, string))
                break;
        if (!v)
            v = cp_enqvar(string);
        wl->wl_word = copy(v ? "1" : "0");
        return (wl);
        
        case '#':
        wl = alloc(wordlist);
        string++;
        for (v = variables; v; v = v->va_next)
            if (eq(v->va_name, string))
                break;
        if (!v)
            v = cp_enqvar(string);
        if (!v) {
            fprintf(cp_err, "Error: %s: no such variable.\n",
                    string);
            return (NULL);
        }
        if (v->va_type == VT_LIST)
            for (v = v->va_vlist, i = 0; v; v = v->va_next)
                i++;
        else
            i = (v->va_type != VT_BOOL);
        (void) sprintf(buf, "%d", i);
        wl->wl_word = copy(buf);
        return (wl);

        case '\0':
        wl = alloc(wordlist);
        wl->wl_word = copy("$");
        return (wl);
    }

    /* The notation var[stuff] has two meanings...  If this is a real
     * variable, then the [] denotes range, but if this is a strange
     * (e.g, device parameter) variable, it could be anything...
     */
    for (v = variables; v; v = v->va_next)
        if (eq(v->va_name, string))
            break;
    if (!v && isdigit(*string)) {
        for (v = variables; v; v = v->va_next)
            if (eq(v->va_name, "argv"))
                break;
        range = string;
    }
    if (!v) {
        range = NULL;
        string = oldstring;
        v = cp_enqvar(string);
    }
    if (!v && (s = getenv(string))) {
        wl = alloc(wordlist);
        wl->wl_word = copy(s);
        return (wl);
    }
    if (!v) {
        fprintf(cp_err, "Error: %s: no such variable.\n", string);
        return (NULL);
    }
    wl = cp_varwl(v);

    /* Now parse and deal with 'range' ... */
    if (range) {
        for (low = 0; isdigit(*range); range++)
            low = low * 10 + *range - '0';
        if ((*range == '-') && isdigit(range[1]))
            for (up = 0, range++; isdigit(*range); range++)
                up = up * 10 + *range - '0';
        else if (*range == '-')
            up = wl_length(wl);
        else
            up = low;
        up--, low--;
        wl = wl_range(wl, low, up);
    }

    return (wl);
}

/* Print the values of currently defined variables. */

struct xxx {
    struct variable *x_v;
    char x_char;
} ;

void
cp_vprint()
{
    struct variable *v;
    struct variable *uv1, *uv2;
    wordlist *wl;
    int i, j;
    char *s;
    struct xxx *vars;

    cp_usrvars(&uv1, &uv2);

    for (v = uv1, i = 0; v; v = v->va_next)
        i++;
    for (v = uv2; v; v = v->va_next)
        i++;
    for (v = variables; v; v = v->va_next)
        i++;
    
    vars = (struct xxx *) tmalloc(sizeof (struct xxx) * i);

    out_init();
    for (v = variables, i = 0; v; v = v->va_next, i++) {
        vars[i].x_v = v;
        vars[i].x_char = ' ';
    }
    for (v = uv1; v; v = v->va_next, i++) {
        vars[i].x_v = v;
        vars[i].x_char = '*';
    }
    for (v = uv2; v; v = v->va_next, i++) {
        vars[i].x_v = v;
        vars[i].x_char = '+';
    }

    qsort((char *) vars, i, sizeof (struct xxx), vcmp);

    for (j = 0; j < i; j++) {
        if (j && eq(vars[j].x_v->va_name, vars[j - 1].x_v->va_name))
            continue;
        v = vars[j].x_v;
        if (v->va_type == VT_BOOL) {
/*             out_printf("%c %s\n", vars[j].x_char, v->va_name); */
             sprintf(out_pbuf, "%c %s\n", vars[j].x_char, v->va_name);
         out_send(out_pbuf);
        } else {
            out_printf("%c %s\t", vars[j].x_char, v->va_name);
            wl = vareval(v->va_name);
            s = wl_flatten(wl);
            if (v->va_type == VT_LIST) {
                out_printf("( %s )\n", s);
            } else
                out_printf("%s\n", s);
        }
    }
    
    return;
}

static int
vcmp(v1, v2)
    struct xxx *v1, *v2;
{
    int i;

    if (i = strcmp(v1->x_v->va_name, v2->x_v->va_name))
        return (i);
    else
        return (v1->x_char - v2->x_char);
}

wordlist *
cp_varwl(var)
    struct variable *var;
{
    wordlist *wl = NULL, *w, *wx = NULL;
    char buf[BSIZE];
    struct variable *vt;

    switch(var->va_type) {
        case VT_BOOL:
            /* Can't ever be false. */
            (void) sprintf(buf, "%s", var->va_bool ? "true" :
                    "false");
            break;
        case VT_NUM:
            (void) sprintf(buf, "%d", var->va_num);
            break;
        case VT_REAL:
            /* This is a case where printnum isn't too good... */
            (void) sprintf(buf, "%G", var->va_real);
            break;
        case VT_STRING:
            (void) strcpy(buf, cp_unquote(var->va_string));
            break;
        case VT_LIST:   /* The tricky case. */
            for (vt = var->va_vlist; vt; vt = vt->va_next) {
                w = cp_varwl(vt);
                if (wl == NULL)
                    wl = wx = w;
                else {
                    wx->wl_next = w;
                    w->wl_prev = wx;
                    wx = w;
                }
            }
            return (wl);
        default:
            fprintf(cp_err, 
            "cp_varwl: Internal Error: bad variable type %d\n", 
                var->va_type);
            return (NULL);
    }
    wl = alloc(wordlist);
    wl->wl_next = wl->wl_prev = NULL;
    wl->wl_word = copy(buf);
    return (wl);
}

/* Set a variable. */

void
cp_vset(varname, type, value)
    char *varname;
    char type;
    char *value;
{
    struct variable *v;
    int i;
    bool alreadythere = false;

/* for (v = variables; v; v = v->va_next) ; printf("ok while setting %s\n", 
        varname);*/
    varname = cp_unquote(varname);
    for (v = variables; v; v = v->va_next)
        if (eq(varname, v->va_name)) {
            alreadythere = true;
            break;
        }
    if (!v) {
        v = alloc(variable);
        v->va_name = copy(varname);
    }
    switch (type) {
        case VT_BOOL:
        if (* ((bool *) value) == false) {
            cp_remvar(varname);
            return;
        } else
            v->va_bool = true;
        break;

        case VT_NUM:
        v->va_num = * (int *) value;
        break;

        case VT_REAL:
        v->va_real = * (double *) value;
        break;

        case VT_STRING:
        v->va_string = copy(value);
        break;

        case VT_LIST:
        v->va_vlist = (struct variable *) value;
        break;

        default:
        fprintf(cp_err, 
            "cp_vset: Internal Error: bad variable type %d.\n", 
                type);
        return;
    }
    v->va_type = type;

    /* Now, see if there is anything interesting going on. We recognise
     * these special variables: noglob, nonomatch, history, echo,
     * noclobber, prompt, and verbose. cp_remvar looks for these variables
     * too. The host program will get any others.
     */

    if (eq(varname, "noglob"))
        cp_noglob = true;
    else if (eq(varname, "nonomatch"))
        cp_nonomatch = true;
    else if (eq(varname, "history") && (type == VT_NUM))
        cp_maxhistlength = v->va_num;
    else if (eq(varname, "history") && (type == VT_REAL))
        cp_maxhistlength = v->va_real;
    else if (eq(varname, "noclobber"))
        cp_noclobber = true;
    else if (eq(varname, "prompt") && (type == VT_STRING))
        cp_promptstring = copy(v->va_string);
    else if (eq(varname, "ignoreeof"))
        cp_ignoreeof = true;
    else if (eq(varname, "cpdebug")) {
        cp_debug = true;
#ifndef CPDEBUG
        fprintf(cp_err, 
        "Warning: program not compiled with cshpar debug messages\n");
#endif
    }

    switch (i = cp_usrset(v, true)) {

        case US_OK:
        /* Normal case. */
        if (!alreadythere) {
            v->va_next = variables;
            variables = v;
        }
        break;

        case US_DONTRECORD:
        /* Do nothing... */
        if (alreadythere) {
            fprintf(cp_err, 
    "cp_vset: Internal Error: %s already there, but 'dont record'\n",
                    v->va_name);
        }
        break;
    
        case US_READONLY:
        /* Badness... */
        fprintf(cp_err, "Error: %s is a read-only variable.\n",
                v->va_name);
        if (alreadythere)
            fprintf(cp_err, 
        "cp_vset: Internal Error: it was already there too!!\n");
        break;

        default:
        fprintf(cp_err, "cp_vset: Internal Error: bad US val %d\n", i);
        break;
    }

    return;
}

void
cp_remvar(varname)
    char *varname;
{
    struct variable *v, *lv = NULL;
    bool found = true;
    int i;

    for (v = variables; v; v = v->va_next) {
        if (eq(v->va_name, varname))
            break;
        lv = v;
    }
    if (!v) {
        /* Gotta make up a var struct for cp_usrset()... */
        v = alloc(variable);
        v->va_name = varname;
        found = false;
    }

    /* Note that 'unset history' doesn't do anything here... Causes
     * trouble...
     */
    if (eq(varname, "noglob"))
        cp_noglob = false;
    else if (eq(varname, "nonomatch"))
        cp_nonomatch = false;
    else if (eq(varname, "noclobber"))
        cp_noclobber = false;
    else if (eq(varname, "prompt"))
        cp_promptstring = "";
    else if (eq(varname, "cpdebug"))
        cp_debug = false;
    else if (eq(varname, "ignoreeof"))
        cp_ignoreeof = false;

    switch (i = cp_usrset(v, false)) {

        case US_OK:
        /* Normal case. */
        if (found) {
            if (lv)
                lv->va_next = v->va_next;
            else
                variables = v->va_next;
        }
        break;

        case US_DONTRECORD:
        /* Do nothing... */
        if (found)
            fprintf(cp_err,
        "cp_remvar: Internal Error: var %d in list but shouldn't be\n",
                    varname);
        break;
    
        case US_READONLY:
        /* Badness... */
        fprintf(cp_err, "Error: %s is a read-only variable.\n",
                v->va_name);
        if (found)
            fprintf(cp_err,
        "cp_remvar: Internal Error: var %d in list but shouldn't be\n",
                    varname);
        break;

        default:
        fprintf(cp_err, "cp_remvar: Internal Error: bad US val %d\n",
                i);
        break;
    }

    tfree(v);
    return;
}

/* The set command. Syntax is 
 * set [opt ...] [opt = val ...]. Val may be a string, an int, a float,
 * or a list of the form (elt1 elt2 ...).
 */


void
com_set(wl)
    wordlist *wl;
{
    struct variable *vars;
    char *s;

    if (wl == NULL) {
        cp_vprint();
        return;
    }
    vars = cp_setparse(wl);

    /* This is sort of a hassle... */
    while (vars) { 
        switch (vars->va_type) {
            case VT_BOOL:
            s = (char *) &vars->va_bool;
            break;
            case VT_NUM:
            s = (char *) &vars->va_num;
            break;
            case VT_REAL:
            s = (char *) &vars->va_real;
            break;
            case VT_STRING:
            s = vars->va_string;
            break;
            case VT_LIST:
            s = (char *) vars->va_vlist;
            break;
        }
        cp_vset(vars->va_name, vars->va_type, s);
        vars = vars->va_next;
    }
    return;
}

struct variable *
cp_setparse(wl)
    wordlist *wl;
{
    char *name, *val, *s, *ss;
    double *td;
    struct variable *listv = NULL, *vv, *lv = NULL;
    struct variable *vars = NULL;
    int balance;

    while (wl) {
        name = cp_unquote(wl->wl_word);
        wl = wl->wl_next;
        if (((wl == NULL) || (*wl->wl_word != '=')) && 
                index(name, '=') == NULL) {
            vv = alloc(variable);
            vv->va_name = copy(name);
            vv->va_type = VT_BOOL;
            vv->va_bool = true;
            vv->va_next = vars;
            vars = vv;
            continue;
        }
        if (wl && eq(wl->wl_word, "=")) {
            wl = wl->wl_next;
            if (wl == NULL) {
                fprintf(cp_err, "Error: bad set form.\n");
                return (NULL);
            }
            val = wl->wl_word;
            wl = wl->wl_next;
        } else if (wl && (*wl->wl_word == '=')) {
            val = wl->wl_word + 1;
            wl = wl->wl_next;
        } else if (s = index(name, '=')) {
            val = s + 1;
            *s = '\0';
            if (*val == '\0') {
                if (!wl) {
                    fprintf(cp_err,
                        "Error:  %s equals what?.\n",
                        name);
                    return (NULL);
                } else {
                    val = wl->wl_word;
                    wl = wl->wl_next;
                }
            }
        } else {
            fprintf(cp_err, "Error: bad set form.\n");
            return (NULL);
        }
        val = cp_unquote(val);
        if (eq(val, "(")) { /* ) */
            /* The beginning of a list... We have to walk down
             * the list until we find a close paren... If there
             * are nested ()'s, treat them as tokens...
             */
            balance = 1;
            while (wl) {
                if (eq(wl->wl_word, "(")) { /* ) ( */
                    balance++;
                } else if (eq(wl->wl_word, ")")) {
                    if (!--balance)
                        break;
                }
                vv = alloc(variable);
                ss = cp_unquote(wl->wl_word);
                td = ft_numparse(&ss, false);
                if (td) {
                    vv->va_type = VT_REAL;
                    vv->va_real = *td;
                } else {
                    vv->va_type = VT_STRING;
                    vv->va_string = copy(ss);
                }
                if (listv) {
                    lv->va_next = vv;
                    lv = vv;
                } else
                    listv = lv = vv;
                wl = wl->wl_next;
            }
            if (balance && !wl) {
                fprintf(cp_err, "Error: bad set form.\n");
                return (NULL);
            }
            
            vv = alloc(variable);
            vv->va_name = copy(name);
            vv->va_type = VT_LIST;
            vv->va_vlist = listv;
            vv->va_next = vars;
            vars = vv;

            wl = wl->wl_next;
            continue;
        }

        ss = cp_unquote(val);
        td = ft_numparse(&ss, false);
        vv = alloc(variable);
        vv->va_name = copy(name);
        vv->va_next = vars;
        vars = vv;
        if (td) {
            /*** We should try to get VT_NUM's... */
            vv->va_type = VT_REAL;
            vv->va_real = *td;
        } else {
            vv->va_type = VT_STRING;
            vv->va_string = copy(val);
        }
    }
    return (vars);
}

void
com_unset(wl)
    wordlist *wl;
{
    register char *name;
    struct variable *var, *nv;

    if (eq(wl->wl_word, "*")) {
        for (var = variables; var; var = nv) {
            nv = var->va_next;
            cp_remvar(var->va_name);
        }
        wl = wl->wl_next;
    }
    while (wl != NULL) {
        name = wl->wl_word;
        cp_remvar(name);
        wl = wl->wl_next;
    }
    return;
}

/* Shift a list variable, by default argv, one to the left (or more if a
 * second argument is given.
 */

void
com_shift(wl)
    wordlist *wl;
{
    struct variable *v, *vv;
    char *n = "argv";
    int num = 1;

    if (wl) {
        n = wl->wl_word;
        wl = wl->wl_next;
    }
    if (wl)
        num = scannum(wl->wl_word);
    
    for (v = variables; v; v = v->va_next)
        if (eq(v->va_name, n))
            break;
    if (!v) {
        fprintf(cp_err, "Error: %s: no such variable\n", n);
        return;
    }
    if (v->va_type != VT_LIST) {
        fprintf(cp_err, "Error: %s not of type list\n", n);
        return;
    }
    for (vv = v->va_vlist; vv && (num > 0); num--)
        vv = vv->va_next;
    if (num) {
        fprintf(cp_err, "Error: variable %s not long enough\n", n);
        return;
    }

    v->va_vlist = vv;
    return;
}

/* Determine the value of a variable.  Fail if the variable is unset,
 * and if the type doesn't match, try and make it work...
 */

bool
cp_getvar(name, type, retval)
    int type;
    char *name, *retval;
{
    struct variable *v;

    for (v = variables; v; v = v->va_next)
        if (eq(name, v->va_name))
            break;
    if (v == NULL) {
        if (type == VT_BOOL)
            * (bool *) retval = false; 
        return (false);
    }
    if (v->va_type == type) {
        switch (type) {
            case VT_BOOL:
                * (bool *) retval = true; 
                break;
            case VT_NUM: {
                int *i;
                i = (int *) retval;
                *i = v->va_num;
                break;
            }
            case VT_REAL: {
                double *d;
                d = (double *) retval;
                *d = v->va_real;
                break;
            }
            case VT_STRING: { /* Gotta be careful to have room. */
                char *s;
                s = cp_unquote(v->va_string);
                cp_wstrip(s);
                (void) strcpy(retval, s);
                break;
            }
            case VT_LIST: { /* Funny case... */
                struct variable **tv;
                tv = (struct variable **) retval;
                *tv = v->va_vlist;
                break;
            }
            default:
                fprintf(cp_err, 
                "cp_getvar: Internal Error: bad var type %d.\n",
                        type);
                break;
        }
        return (true);
    } else {
        /* Try to coerce it.. */
        if ((type == VT_NUM) && (v->va_type == VT_REAL)) {
            int *i;
            i = (int *) retval;
            *i = (int) v->va_real;
            return (true);
        } else if ((type == VT_REAL) && (v->va_type == VT_NUM)) {
            double *d;
            d = (double *) retval;
            *d = (double) v->va_num;
            return (true);
        } else if ((type == VT_STRING) && (v->va_type == VT_NUM)) {
            (void) sprintf(retval, "%d", v->va_num);
            return (true);
        } else if ((type == VT_STRING) && (v->va_type == VT_REAL)) {
            (void) sprintf(retval, "%f", v->va_real);
            return (true);
        }
        return (false);
    }
}

