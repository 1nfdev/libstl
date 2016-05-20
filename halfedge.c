#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "hash64.h"

static uint32_t
edge(uint32_t *edges, uint32_t nedges, uint32_t *eht, uint32_t ehtcap, uint32_t *e)
{
	uint32_t *eip, ei;
	uint32_t hash;
	uint32_t i;

	hash = final64(e[0], e[1]);
	for(i = 0; i < ehtcap; i++){
		eip = eht + ((hash + i) & (ehtcap - 1));
		ei = *eip;
		if(ei == 0){
			*eip = nedges+1;
			return nedges;
		}
		ei--;
		if(cmp64(e, edges + 2*ei) == 0)
			return ei;
	}
	return ~(uint32_t)0;
}

int
halfedges(uint32_t *tris, uint32_t ntris, uint32_t **edgep, uint32_t *nedgep)
{
	uint32_t *edges, *eht;
	uint32_t nedges, ehtcap;
	uint32_t i, ti;

	nedges = 0;
	edges = malloc(3*ntris * 2*sizeof edges[0]);
	ehtcap = nextpow2(3*ntris);
	eht = malloc(ehtcap * sizeof edges[0]);
	memset(eht, 0, ehtcap * sizeof edges[0]);
	for(i = 0; i < ntris; i++){
		for(ti = 0; ti < 3; ti++){
			uint32_t v0, v1, e[2];
			uint32_t ei;
			v0 = tris[3*i+ti];
			v1 = tris[3*i+((1<<ti)&3)];
			e[0] = v0 < v1 ? v0 : v1; // min
			e[1] = v0 < v1 ? v1 : v0; // the other
			ei = edge(edges, nedges, eht, ehtcap, e);
			if(ei == ~(uint32_t)0){
				fprintf(stderr, "halfedge: hash full at triangle %d/%d cap %d\n", i, ntris, ehtcap);
				goto exit_fail;
			}
			if(ei == nedges){
				copy64(edges + 2*nedges, e);
				nedges++;
			} else {
			}
		}
	}
	free(eht);
	edges = realloc(edges, nedges * 2*sizeof edges[0]);
	*edgep = edges;
	*nedgep = nedges;
	fprintf(stderr, "halfedge: found %d edges\n", nedges);
	return 0;

exit_fail:
	free(eht);
	free(edges);
	return -1;
}
