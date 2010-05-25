/*
    Copyright (C) 2010 ruth(f.c.s) (deinococcus.radiophilus@gmail.com)
    partially based on GPL code by Jonah Sherman <sherman.jonah@gmail.com>
    that code was based on:
    AdBlockPlus by Wladimir Palant <trev@adblockplus.org>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
    Boston, MA 02110-1301, USA.
*/
#include "AdBlock.h"

using namespace std;

namespace WebCore {

static vector<String> blacklistPatterns;
gint i;

void addPattern(const String& pat)
{
	int delim = pat.find("$");
	String pattern;
	if (delim < 0) {
		delim = pat.length();
	}
	pattern = pat.left(delim);
	if (pattern.startsWith("/") && pattern.endsWith("/")) {
		pattern.insert("^.*", 0);
		pattern.append(".*$");
		blacklistPatterns.push_back(pattern);
		return;
	}
	int pos = 0;
	replace(pattern, RegularExpression("\\*", TextCaseSensitive), ".*");
	replace(pattern, RegularExpression("\\^", TextCaseSensitive), ".*$");
	replace(pattern, RegularExpression("^\\|\\|", TextCaseSensitive), "^.*");
	if (pattern.find(";") < 0 && pattern.find("|") < 0) { /* fancy stuff; not yet supported */
		blacklistPatterns.push_back(pattern);
	}
	// cout << "Pattern is: " << pattern.utf8().data() << endl;
}

void ab_loadFile(gchar *filePath)
{
	FILE *file = fopen(filePath, "r");
	if (file) {
		char buf[512];
		fgets(buf, 512, file);
		while (fgets(buf, 512, file)) {
			String line(buf);
			line.replace("\n","");
			if (line.startsWith("@@")) {
				// ab_whiteList.addPattern(line.substring(2));
			} else if (!line.startsWith("!") && !line.startsWith("[") && line.find("#") < 0){
				addPattern(line);
			}
		}
		fclose(file);
	}
	return;
}

void ab_initialize()
{
	const gchar *homeDir = g_getenv("HOME");
	if (!homeDir)
		homeDir = g_get_home_dir();
	gchar *filePath;
	filePath = g_build_filename(homeDir, ".config/chromium/patterns.ini", NULL);
	ab_loadFile(filePath);
	g_free(filePath);
}

bool shouldbeBlocked(const KURL& url)
{
	String urlString = url.string();
	if (!urlString.startsWith("http://")) {
		return false;
	}
	if (blacklistPatterns.size() == 0) {
		ab_initialize();
	}
	for (i = 0; i < blacklistPatterns.size(); i++) {
		RegularExpression patternRegExp(blacklistPatterns[i], TextCaseSensitive);
		int matchLength = 0;
		patternRegExp.match(urlString, 0 , &matchLength);
		if (matchLength > 0) {
			cout << "filtered URL: " << urlString.utf8().data() << " using Pattern: " << blacklistPatterns[i].utf8().data() << endl;
			return true;
		}
	}
	return false;
}

}
