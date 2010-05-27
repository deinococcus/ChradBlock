#ifndef CHROME_BROWSER_RENDERER_HOST_AD_BLOCK_IMPL_H_
#define CHROME_BROWSER_RENDERER_HOST_AD_BLOCK_IMPL_H_

#include <string>
#include <vector>
#include <fstream>

#include <glib.h>

#include "third_party/WebKit/WebKit/chromium/public/WebRegularExpression.h"
#include "third_party/WebKit/WebKit/chromium/public/WebString.h"

class AdBlockImpl {
 public:
  AdBlockImpl();
  bool shouldbeBlocked(const std::string& url, int type);

 private:
  /* WebRegularExpression is NonCopyable, so we cannot create a vector of regexps. */
  std::vector<WebKit::WebString> blacklistPatterns;
  void addPattern(std::string& pat);
  void loadFile(gchar *filePath);
  std::ofstream log;
};

#endif  // CHROME_BROWSER_RENDERER_HOST_AD_BLOCK_IMPL_H_
