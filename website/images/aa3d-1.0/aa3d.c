
/* 
 *     XaoS, a fast portable realtime fractal zoomer 
 *                  Copyright (C) 1996,1997 by
 *
 *      Jan Hubicka          (hubicka@paru.cas.cz)
 *      Thomas Marsh         (tmarsh@austin.ibm.com)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include <stdio.h>
#include <stdlib.h>
static char data[65536 / 2];
int main(int argc, char **argv)
{
    int x, y, s, l = 0, l2 = 0, letter;
    int skip = 12;
    int width = 80;
    int shift = 0;
    int rnd = 1;
    int digit = 0;
    char *string = NULL;
    FILE *f = NULL;
    for (s = 1; s < argc; s++) {
	if (s < argc - 1 && !strcmp(argv[s], "-w"))
	    width = atol(argv[s + 1]), s++;
	else if (s < argc - 1 && !strcmp(argv[s], "-s"))
	    skip = atol(argv[s + 1]), s++;
	else if (s < argc - 1 && !strcmp(argv[s], "-t"))
	    string = argv[s + 1], s++;
	else if (s < argc - 1 && !strcmp(argv[s], "-f")) {
	    f = fopen(argv[s + 1], "r");
	    if (f == NULL)
		perror(argv[s + 1]), exit(1);
	    s++;
	} else if (!strcmp(argv[s], "-r"))
	    rnd = 0;
	else if (!strcmp(argv[s], "-d"))
	    digit = 1;
	else {
	    printf("Unknown option %s\n\n", argv[s]);
	    printf("ASCII-art stereogram generator version 1.0 by Jan Hubicka <hubicka@freesoft.cz>\n\n"
		   "Usage: %s [options] < file\n\n"
	           "File consist from spaces and digits representing depth\n\n"
		   "Options:\n"
		   "-w width   Image width (80)\n"
		   "-s step    Size of repeating seqence (12)\n"
		   "-t text    Text used to generate background\n"
		   "-r         Disable random text placement\n"
		   "-f file    Use file as input for raster\n"
		   "-d         Use digits instead of letters\n"
		   ,argv[0]);
	    return 1;
	}
    }
    for (y = 0; !feof(stdin); y++) {
	l = 0;
	l2 = 0;
	if (rnd)
	    shift = rand();
	for (x = 0; x < width; x++) {
	    if (f != NULL) {
		if (!l2) {
		    letter = getc(f);
		    if (letter == '\n' || letter == EOF)
			letter = ' ', l2 = 1;
		} else
		    letter = ' ';
	    } else
		letter = !digit ? rand() % 32 + 'A' : rand() % 10 + '0';
	    if (!l && x > skip) {
		s = getc(stdin);
		if (s == ' ')
		    s = 0;
		else if (s == '\n' || s == EOF)
		    s = 0, l = 1;
		else if (s >= '0' && s <= '9')
		    s = '0' - s;
		else
		    s = -2;
	    } else
		s = 0;
	    s += skip;
	    s = x - s;
	    if (s < 0)
		s = string == NULL ? letter : string[(x + shift) % strlen(string)];
	    else
		s = data[s];
	    data[x] = s;
	    putc(s, stdout);
	}
	putc('\n', stdout);
	s = 'a';
	while (!l && s != EOF && s != '\n')
	    s = getc(stdin);
	s = 'a';
	if (f != NULL)
	    while (!l2 && s != EOF && s != '\n')
		s = getc(f);
    }
    return 0;
}
