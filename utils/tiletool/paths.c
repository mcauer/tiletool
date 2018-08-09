/*
 * Copyright (C) 2012 Dmitry Marakasov
 *
 * This file is part of tiletool.
 *
 * tiletool is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * tiletool is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with tiletool.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef PATHS_H
#define PATHS_H

#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>

/* windows has different mkdir() in different include file */
#ifdef __WIN32__
#	include <io.h>
#	define mkdir(a, b) mkdir(a)
#endif

char* get_tile_path_r(char* buffer, size_t size, const char* prefix, int x, int y, int zoom, const char* suffix) {
	
        //fill up x with zeros
        char zerosXc [10];
        snprintf(zerosXc, 10, "%09d", x);
	zerosXc[9] = '\0';

	//split into chunks of 3
	char x1[4];
	memcpy( x1, &zerosXc[0], 3 );
	x1[3] = '\0';

	char x2[4];
	memcpy( x2, &zerosXc[3], 3 );
	x2[3] = '\0';

	char x3[4];
	memcpy( x3, &zerosXc[6], 3 );
	x3[3] = '\0';

        //fill up y with zeros
        char zerosYc [10];
        snprintf(zerosYc, 10, "%09d", y);
	zerosYc[9] = '\0';

	//split into chunks of 3
	char y1[4];
	memcpy( y1, &zerosYc[0], 3 );
	y1[3] = '\0';

	char y2[4];
	memcpy( y2, &zerosYc[3], 3 );
	y2[3] = '\0';

	char y3[4];
	memcpy( y3, &zerosYc[6], 3 );
	y3[3] = '\0';

	// fill up zoom with zeros
	char zerosZoom[3];
	snprintf(zerosZoom, 3, "%02d", zoom);
	zerosZoom[2] = '\0';

        if (snprintf(buffer, size, "%s/%s/%s/%s/%s/%s/%s/%s%s", prefix, zerosZoom, x1, x2, x3, y1, y2, y3, suffix) >= (int)size)
                return NULL;
printf("%s", buffer);
        return buffer;
}

char* get_tile_path(const char* prefix, int x, int y, int zoom, const char* suffix) {
	static char buffer[FILENAME_MAX];
	return get_tile_path_r(buffer, FILENAME_MAX, prefix, x, y, zoom, suffix);
}

int create_directories(char* path) {
	char* lastslash = strrchr(path, '/');
	if (lastslash == NULL)
		return 0;

	*lastslash = '\0';

	int ret = mkdir(path, 0777);
	if (ret == -1 && errno == ENOENT) {
		if ((ret = create_directories(path)) == 0)
			ret = mkdir(path, 0777);
	}

	*lastslash = '/';

	if (ret == -1 && errno == EEXIST)
		ret = 0;

	return ret;
}

#endif
