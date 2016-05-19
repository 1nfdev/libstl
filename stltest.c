
#include <stdio.h>
#include <stdlib.h>
#include "stlfile.h"

int
main(int argc, char *argv[])
{
	FILE *fp;
	float *verts;
	uint32_t *tris;
	uint16_t *attrs;
	uint32_t ntris, nverts;
	int i;

	for(i = 0; i < 5; i++){
		if(argc > 1){
			fp = fopen(argv[1], "rb");
		} else {
			fp = stdin;
		}
		loadstl(fp, &verts, &nverts, &tris, &attrs, &ntris);
		fclose(fp);
		free(tris);
		free(verts);
		free(attrs);
	}

	return 0;
}
