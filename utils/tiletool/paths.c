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
	
	//fill up with zeros
	string zerosX = "000000000";
	char* intStrX = itoa(x);
	string strX = string(intStrX);
	int lengthX = strX.length();
	zerosX.replace(9-lengthX,lengthX,strX);
	//partition into sets of 3
	string x1 = zerosX.substr(0,3);
	string x2 = zerosX.substr(3,3);
	string x3 = zerosX.substr(6,3);
	
	//fill up with zeros
	string zerosY = "000000000";
	char* intStrY = itoa(y);
	string strY = string(intStrY);
	int lengthY = strY.length();
	zerosY.replace(9-lengthY,lengthY,strY);
	//partition into sets of 3
	string y1 = zerosY.substr(0,3);
	string y2 = zerosY.substr(3,3);
	string y3 = zerosY.substr(6,3);
	
	
	if (snprintf(buffer, size, "%s/%d/%s/%s/%s/%s/%s/%s%s", prefix, zoom, x1, x2, x3, y1, y2, y3, suffix) >= (int)size)
		return NULL;

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
