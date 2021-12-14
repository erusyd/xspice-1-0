
/*
    Manage graph data structure.

*/

#include "FTEgraph.h"
#ifndef CMS
#include "FTEextern.h"
#else  /* CMS */
#include "FTEexter.h"
#endif /* CMS */

#ifndef NULL
#define NULL 0
#endif

/* invariant:  currentgraph contains the current graph */
GRAPH *currentgraph;

/*
 *  We use a linked list rather than a circular one because we
 *    expect few links per list and we don't need to add at the
 *    end of a list (we can add at the beginning).
 */

/* linked list of graphs */
typedef struct listgraph {
      /* we use GRAPH here instead of a pointer to save a calloc */
    GRAPH graph;
    struct listgraph *next;
} LISTGRAPH;
#define NEWLISTGRAPH (LISTGRAPH *) calloc(1, sizeof(LISTGRAPH))

#define NUMGBUCKETS 16

typedef struct gbucket {
    LISTGRAPH *list;
} GBUCKET;
/* #define NEWGBUCKET (GBUCKET *) calloc(1, sizeof(GBUCKET)) */

static GBUCKET GBucket[NUMGBUCKETS];

/* note: Zero is not a valid id.  This is used in plot() in graf.c. */
static RunningId = 1;

/* initialize graph structure */
#define SETGRAPH(pgraph, id) (pgraph)->graphid = (id); \
				(pgraph)->degree = 1; \
				(pgraph)->linestyle = -1

/* returns NULL on error */
GRAPH *NewGraph()
{

    GRAPH *pgraph;
    LISTGRAPH *list;
    int BucketId = RunningId % NUMGBUCKETS;

    if (!(list = NEWLISTGRAPH)) {
      internalerror("can't allocate a listgraph");
      return((GRAPH *) NULL);
    }

    pgraph = &list->graph;
    SETGRAPH(pgraph, RunningId);

    if (!GBucket[BucketId].list) {
      GBucket[BucketId].list = list;
    } else {
      /* insert at front of current list */
      list->next = GBucket[BucketId].list;
      GBucket[BucketId].list = list;
    }

    RunningId++ ;

    return(pgraph);

}

/* Given graph id, return graph */
GRAPH *FindGraph(id)
int id;
{

    LISTGRAPH *list;

    list = GBucket[id % NUMGBUCKETS].list;

    if (!list) {
      return(NULL);
    }

    while (list) {
      if (list->graph.graphid == id) {
        /*
                Hey! That's neat.  A return in the middle of a loop eliminates
            the need for a goto or an extra if
        */
        return(&list->graph);
      }
      list = list->next;
    }

    return(NULL);

}

GRAPH *CopyGraph(graph)
GRAPH *graph;
{

    GRAPH *ret;
    struct _keyed *k;
    struct dveclist *link, *newlink;

    ret = NewGraph();
    bcopy(graph, ret, sizeof(struct graph));

    ret->graphid = RunningId - 1;   /* restore id */

    /* copy keyed */
    for (ret->keyed = NULL, k = graph->keyed; k; k = k->next) {
      SaveText(ret, k->text, k->x, k->y);
    }

    /* copy dvecs */
    ret->plotdata = NULL;
    for (link = graph->plotdata; link; link = link->next) {
      newlink = (struct dveclist *) calloc(1, sizeof(struct dveclist));
      newlink->next = ret->plotdata;
      newlink->vector = vec_copy(link->vector);
      /* vec_copy doesn't set v_color or v_linestyle */
      newlink->vector->v_color = link->vector->v_color;
      newlink->vector->v_linestyle = link->vector->v_linestyle;
      newlink->vector->v_flags |= VF_PERMANENT;
      ret->plotdata = newlink;
    }

    ret->commandline = copy(graph->commandline);
    ret->plotname = copy(graph->plotname);

    return(ret);

}

DestroyGraph(id)
int id;
{

    LISTGRAPH *list, *lastlist;
    struct _keyed *k, *nextk;
    struct dveclist *d, *nextd;

    list = GBucket[id % NUMGBUCKETS].list;
    lastlist = NULL;
    while (list) {
      if (list->graph.graphid == id) {  /* found it */

        /* adjust bucket pointers */
        if (lastlist) {
          lastlist->next = list->next;
        } else {
          GBucket[id % NUMGBUCKETS].list = list->next;
        }

        /* run through and de-allocate dynamically allocated keyed list */
        k=list->graph.keyed;
        while (k) {
          nextk = k->next;
          free(k->text);
          free(k);
          k = nextk;
        }

        /* de-allocate dveclist */
        d = list->graph.plotdata;
        while (d) {
          nextd = d->next;
          tfree(d->vector->v_name);
          if (isreal(d->vector)) {
                tfree(d->vector->v_realdata);
              } else {
                tfree(d->vector->v_compdata);
              }
          free(d->vector);
          free(d);
          d = nextd;
        }

        free(list->graph.commandline);
        free(list->graph.plotname);

        /* If device dependent space allocated, free it. */
        if (list->graph.devdep)
          free(list->graph.devdep);
        free(list);

        return(0);
      }
      lastlist = list;
      list = list->next;
    }

    internalerror("tried to destroy non-existent graph");
    return (1);

}

/* free up all dynamically allocated data structures */
FreeGraphs()
{

    GBUCKET *gbucket;
    LISTGRAPH *list, *deadl;

    for (gbucket = GBucket; gbucket < &GBucket[NUMGBUCKETS]; gbucket++) {
      list = gbucket->list;
      while (list) {
        deadl = list;
        list = list->next;
        free(deadl);
      }
    }

}

SetGraphContext(graphid)
int graphid;
{

    currentgraph = FindGraph(graphid);

}

typedef struct gcstack {
    GRAPH *pgraph;
    struct gcstack *next;
} GCSTACK;
GCSTACK *gcstacktop;
#define NEWGCSTACK (GCSTACK *) calloc(1, sizeof(GCSTACK))

/* note: This Push and Pop has tricky semantics.
    Push(graph) will push the currentgraph onto the stack
        and set currentgraph to graph.
    Pop() simply sets currentgraph to the top of the stack and pops stack.
*/
PushGraphContext(graph)
GRAPH *graph;
{

    GCSTACK *gcstack = NEWGCSTACK;

    if (!gcstacktop) {
      gcstacktop = gcstack;
    } else {
      gcstack->next = gcstacktop;
      gcstacktop = gcstack;
    }
    gcstacktop->pgraph = currentgraph;
    currentgraph = graph;

}

PopGraphContext()
{

    GCSTACK *dead;

    currentgraph = gcstacktop->pgraph;
    dead = gcstacktop;
    gcstacktop = gcstacktop->next;
    free(dead);

}
