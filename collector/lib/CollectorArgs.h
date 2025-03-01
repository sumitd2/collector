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

#ifndef _COLLECTOR_ARGS_H_
#define _COLLECTOR_ARGS_H_

#include <string>

#include <json/json.h>

#include "optionparser.h"

namespace collector {

class CollectorArgs {
 public:
  static CollectorArgs* getInstance();

  bool parse(int argc, char** argv, int& exitCode);
  void clear();

  option::ArgStatus checkCollectorConfig(const option::Option& option, bool msg);
  option::ArgStatus checkCollectionMethod(const option::Option& option, bool msg);
  option::ArgStatus checkChisel(const option::Option& option, bool msg);
  option::ArgStatus checkGRPCServer(const option::Option& option, bool msg);
  option::ArgStatus checkOptionalNumeric(const option::Option& option, bool msg);

  const Json::Value& CollectorConfig() const;
  const std::string& CollectionMethod() const;
  const std::string& Chisel() const;
  const std::string& GRPCServer() const;
  const std::string& Message() const;

 private:
  CollectorArgs();
  ~CollectorArgs();

  static CollectorArgs* instance;

  Json::Value collectorConfig;
  std::string collectionMethod;
  std::string chisel;
  std::string message;
  std::string grpcServer;
};

} /* namespace collector */

#endif /* _COLLECTOR_ARGS_H_ */
