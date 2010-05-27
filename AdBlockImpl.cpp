#include "AdBlockImpl.h"

#include <iostream>

#include "base/string_util.h"
#include "third_party/WebKit/WebKit/chromium/public/WebString.h"
#include "third_party/WebKit/WebKit/chromium/public/WebCString.h"

using namespace WebKit;
using namespace std;

AdBlockImpl::AdBlockImpl() {
  const gchar *homeDir = g_getenv("HOME");
  if (!homeDir)
    homeDir = g_get_home_dir();
  gchar *filePath;
  filePath = g_build_filename(homeDir, ".config/chromium/patterns.ini", NULL);
  loadFile(filePath);
  g_free(filePath);

  char logpath[30];
  snprintf(logpath, 30, "/tmp/chrad-log-%d", getpid());
  log.open(logpath);
}

void AdBlockImpl::addPattern(string& pat) {
  size_t delim = pat.find("$");
  if (delim == string::npos) {
    delim = pat.length();
  }
  //  else
  //    cerr << "Warning: cutting $ from " << pat << endl;
  pat.resize(delim);

  if (StartsWithASCII(pat, "/", true) && EndsWith(pat, "/", true)) {
    string pat2 = pat.substr(1, delim - 2);
    //    cerr << "Adding pattern " << pat2 << endl;
    blacklistPatterns.push_back(WebString(ASCIIToUTF16(pat2)));
    return;
  }

  string orig(pat);
  ReplaceSubstringsAfterOffset(&pat, 0, "*", ".*");
  ReplaceSubstringsAfterOffset(&pat, 0, "||", "");
  ReplaceSubstringsAfterOffset(&pat, 0, "^", "[^a-aA-Z0-9\\-.%]");
  if (StartsWithASCII(pat, "|", true))
    pat.replace(0, 1, "^");
  if (EndsWith(pat, "|", true))
    pat.replace(pat.length()-1, 1, "$");

  // if (orig.compare(pat) == 0)
  //   cerr << "Adding pattern " << pat << endl;
  // else
  //   cerr << "Changing pattern from " << orig << " to " << pat << endl;
  
  blacklistPatterns.push_back(WebString(ASCIIToUTF16(pat)));
}

bool AdBlockImpl::shouldbeBlocked(const string& url) {
  /* Build the regexps every time we are queried.
     How do we accelerate this? */
  for (size_t i = 0; i < blacklistPatterns.size(); i++) {
    WebRegularExpression patternRegExp(blacklistPatterns[i], WebTextCaseSensitive);
    int matchLength = 0;
    WebString str(ASCIIToUTF16(url));
    patternRegExp.match(str, 0 , &matchLength);
    if (matchLength > 0) {
      cout << "Filtered URL: " << url << " using Pattern: " << blacklistPatterns[i].utf8().data() << endl;
      /*
        Please uncomment the following code and help me debug it:
        The log file was created, but nothing is written.

      log << "Filtered " << url << " using " << blacklistPatterns[i].utf8().data() << endl;
      log.flush();
      */
      return true;
    }
  }
  return false;
}

void AdBlockImpl::loadFile(gchar *filePath)
{
  ifstream file(filePath);
  
  if (file) {
    string line;
    while (getline(file, line)) {
      if (StartsWithASCII(line, "@@", true)
          || line.find('#') != string::npos
          || line.find(';') != string::npos) { /* fancy stuff; not yet supported */
        //        cerr << "Ignored " << line << endl;
      }
      else if (line[0] != '!' && line[0] != '[') {
        addPattern(line);
      }
    }
    file.close();
  }
  else
    cerr << "Unable to open " << filePath << endl;
}
