/** collector

A full notice with attributions is provided along with this source code.

This program is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License version 2 as published by the Free Software Foundation.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program; if not, write to the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.

* In addition, as a special exception, the copyright holders give
* permission to link the code of portions of this program with the
* OpenSSL library under certain conditions as described in each
* individual source file, and distribute linked combinations
* including the two.
* You must obey the GNU General Public License in all respects
* for all of the code used other than OpenSSL.  If you modify
* file(s) with this exception, you may extend this exception to your
* version of the file(s), but you are not obligated to do so.  If you
* do not wish to do so, delete this exception statement from your
* version.
*/

#ifndef COLLECTOR_FILEDOWNLOADER_H
#define COLLECTOR_FILEDOWNLOADER_H

#include <chrono>
#include <fstream>

#include <curl/curl.h>

namespace collector {

class FileDownloader {
 public:
  enum resolve_t {
    ANY = CURL_IPRESOLVE_WHATEVER,
    IPv4 = CURL_IPRESOLVE_V4,
    IPv6 = CURL_IPRESOLVE_V6,
  };

  FileDownloader();
  ~FileDownloader();

  void SetURL(const char* const url);
  void SetURL(const std::string& url);
  void IPResolve(resolve_t version);
  void SetRetries(unsigned int times, unsigned int delay, unsigned int max_time);
  void SetConnectionTimeout(int timeout);
  void FollowRedirects(bool follow);
  void OutputFile(const char* const path);
  void OutputFile(const std::string& path);
  void CACert(const char* const path);
  void Cert(const char* const path);
  void Key(const char* const path);
  bool ConnectTo(const char* const entry);

  bool IsReady();
  bool Download();

 private:
  CURL* curl;
  curl_slist* connect_to;
  std::ofstream file;
  unsigned int retry_times;
  unsigned int retry_delay;
  std::chrono::seconds retry_max_time;
};

}  // namespace collector

#endif  // COLLECTOR_FILEDOWNLOADER_H