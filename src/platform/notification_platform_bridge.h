// Copyright (c) 2022 LG Electronics, Inc.
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

#ifndef PLATFORM_NOTIFICATION_PLATFORM_BRIDGE_H_
#define PLATFORM_NOTIFICATION_PLATFORM_BRIDGE_H_

#include "neva/app_runtime/public/notification_platform_bridge.h"

class NotificationPlatformBridge
    : public neva_app_runtime::NotificationPlatformBridge {
 public:
  NotificationPlatformBridge();
  ~NotificationPlatformBridge() override;
  void Display(const neva_app_runtime::Notification& notification) override;
  void Close(const std::string& notificationId) override;
  void GetDisplayed(neva_app_runtime::GetDisplayedNotificationsCallback
                        callback) const override;
  void SetReadyCallback(neva_app_runtime::NotificationPlatformBridge::
                            NotificationBridgeReadyCallback callback) override;
};

#endif  // PLATFORM_NOTIFICATION_PLATFORM_BRIDGE_H_
