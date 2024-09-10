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

#ifndef PLUGIN_WEB_APP_FACTORY_LUNA_H_
#define PLUGIN_WEB_APP_FACTORY_LUNA_H_

#include "web_app_factory_interface.h"

class WebAppFactoryLuna : public WebAppFactoryInterface {
 public:
  WebAppBase* CreateWebApp(const std::string& win_type,
                           const ApplicationDescription& desc) override;
  WebAppBase* CreateWebApp(const std::string& win_type,
                           WebPageBase* page,
                           const ApplicationDescription& desc) override;
  WebPageBase* CreateWebPage(const wam::Url& url,
                             const ApplicationDescription& desc,
                             const std::string& launch_params = {}) override;
};

#endif  // PLUGIN_WEB_APP_FACTORY_LUNA_H_
