// Copyright (c) 2014-2021 LG Electronics, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// SPDX-License-Identifier: Apache-2.0

#ifndef CORE_WEB_PROCESS_MANAGER_H_
#define CORE_WEB_PROCESS_MANAGER_H_

#include <cstdint>
#include <list>
#include <string>

namespace Json {
class Value;
}

class WebAppBase;

class WebProcessManager {
 public:
  WebProcessManager() = default;
  virtual ~WebProcessManager() = default;

  virtual std::string GetWebProcessMemSize(uint32_t pid) const;

  virtual Json::Value GetWebProcessProfiling() = 0;
  virtual uint32_t GetWebProcessPID(const WebAppBase* app) const = 0;
  virtual void ClearBrowsingData(const int remove_browsing_data_mask) = 0;
  virtual int MaskForBrowsingDataType(const char* type) = 0;

 protected:
  std::list<const WebAppBase*> RunningApps();
  WebAppBase* FindAppByInstanceId(const std::string& instance_id);
};

#endif  // CORE_WEB_PROCESS_MANAGER_H_
