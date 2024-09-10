// Copyright (c) 2008-2021 LG Electronics, Inc.
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

#ifndef CORE_APPLICATION_DESCRIPTION_H_
#define CORE_APPLICATION_DESCRIPTION_H_

#include <memory>
#include <optional>
#include <set>
#include <string>
#include <unordered_map>

#include "display_id.h"

class ApplicationDescription {
 public:
  enum WindowClass { kWindowClassNormal = 0x00, kWindowClassHidden = 0x01 };

  enum class ThirdPartyCookiesPolicy { kDefault, kAllow, kDeny };

  ApplicationDescription();
  virtual ~ApplicationDescription() = default;

  const std::string& Id() const { return id_; }
  const std::string& Title() const { return title_; }
  const std::string& EntryPoint() const { return entry_point_; }
  const std::string& Icon() const { return icon_; }

  bool IsTransparent() const { return transparency_; }

  bool HandlesDeeplinking() const { return !deep_linking_params_.empty(); }

  bool HandlesRelaunch() const { return handles_relaunch_; }

  const std::string& VendorExtension() const { return vendor_extension_; }

  WindowClass WindowClassValue() const { return window_class_value_; }

  const std::string& TrustLevel() const { return trust_level_; }

  const std::string& SubType() const { return sub_type_; }

  const std::string& FolderPath() const { return folder_path_; }

  const std::string& DefaultWindowType() const { return default_window_type_; }

  const std::string& EnyoBundleVersion() const { return enyo_bundle_version_; }

  const std::set<std::string>& SupportedEnyoBundleVersions() const {
    return supported_enyo_bundle_versions_;
  }

  const std::string& EnyoVersion() const { return enyo_version_; }

  const std::string& Version() const { return version_; }

  const std::string& GroupWindowDesc() const { return group_window_desc_; }

  const std::string& V8SnapshotPath() const { return v8_snapshot_path_; }

  const std::string& V8ExtraFlags() const { return v8_extra_flags_; }

  static std::unique_ptr<ApplicationDescription> FromJsonString(
      const char* json_str);

  bool IsInspectable() const { return inspectable_; }
  bool UseCustomPlugin() const { return custom_plugin_; }
  bool UseNativeScroll() const { return use_native_scroll_; }
  uint32_t SplashDismissTimeoutMs() const { return splash_dismiss_timeout_ms_; }
  bool UsePrerendering() const { return use_prerendering_; }

  bool DoNotTrack() const { return do_not_track_; }

  bool BackHistoryAPIDisabled() const { return back_history_api_disabled_; }
  void SetBackHistoryAPIDisabled(bool disabled) {
    back_history_api_disabled_ = disabled;
  }

  std::optional<int> WidthOverride() const { return width_override_; }
  std::optional<int> HeightOverride() const { return height_override_; }

  bool HandleExitKey() const { return handle_exit_key_; }
  bool SupportsAudioGuidance() const { return supports_audio_guidance_; }
  bool IsEnableBackgroundRun() const { return enable_background_run_; }
  bool AllowVideoCapture() const { return allow_video_capture_; }
  bool AllowAudioCapture() const { return allow_audio_capture_; }
  const std::set<std::string>& WebAppPermissions() const {
    return web_app_permissions_;
  }

  ThirdPartyCookiesPolicy GetThirdPartyCookiesPolicy() const {
    return third_party_cookies_policy_;
  }

  virtual bool UseVirtualKeyboard() const { return use_virtual_keyboard_; }
  // Key code is changed only for facebooklogin WebApp
  const std::unordered_map<int, std::pair<int, int>>& KeyFilterTable() const {
    return key_filter_table_;
  }

  std::optional<double> NetworkStableTimeout() const {
    return network_stable_timeout_;
  }
  bool DisallowScrollingInMainFrame() const {
    return disallow_scrolling_in_main_frame_;
  }
  std::optional<int> DelayMsForLaunchOptimization() const {
    return delay_ms_for_launch_optimization_;
  }
  bool UseUnlimitedMediaPolicy() const { return use_unlimited_media_policy_; }
  const std::string& LocationHint() const { return location_hint_; }

  struct WindowOwnerInfo {
    bool allow_anonymous = false;
    std::unordered_map<std::string, int> layers;
  };

  struct WindowClientInfo {
    std::string layer;
    std::string hint;
  };

  struct WindowGroupInfo {
    std::string name;
    bool is_owner = false;
  };

  const WindowGroupInfo GetWindowGroupInfo();
  const WindowOwnerInfo GetWindowOwnerInfo();
  const WindowClientInfo GetWindowClientInfo();

  // To support multi display
  DisplayId GetDisplayAffinity() const { return display_affinity_; }
  void SetDisplayAffinity(DisplayId display) { display_affinity_ = display; }
  std::optional<int> CustomSuspendDOMTime() const {
    return custom_suspend_dom_time_;
  }

  bool UseVideoDecodeAccelerator() const {
    return use_video_decode_accelerator_;
  }

 private:
  bool CheckTrustLevel(std::string trust_level);

  std::string id_;
  std::string title_;
  std::string entry_point_;
  std::string icon_;
  std::string requested_window_orientation_;

  bool transparency_ = false;
  std::string vendor_extension_;
  WindowClass window_class_value_ = kWindowClassNormal;
  std::string trust_level_;
  std::string sub_type_;
  std::string deep_linking_params_;
  bool handles_relaunch_ = false;
  std::string folder_path_;
  std::string default_window_type_;
  std::string enyo_bundle_version_;
  std::set<std::string> supported_enyo_bundle_versions_;
  std::string enyo_version_;
  std::string version_;
  std::string v8_snapshot_path_;
  std::string v8_extra_flags_;
  bool inspectable_ = true;
  bool custom_plugin_ = false;
  bool back_history_api_disabled_ = false;
  std::optional<int> width_override_;
  std::optional<int> height_override_;
  std::unordered_map<int, std::pair<int, int>> key_filter_table_;
  std::string group_window_desc_;
  bool do_not_track_ = false;
  bool handle_exit_key_ = false;
  bool enable_background_run_ = false;
  bool allow_video_capture_ = false;
  bool allow_audio_capture_ = false;
  bool supports_audio_guidance_ = false;
  bool use_native_scroll_ = false;
  uint32_t splash_dismiss_timeout_ms_ = 8000;
  bool use_prerendering_ = false;
  std::optional<double> network_stable_timeout_;
  bool disallow_scrolling_in_main_frame_ = true;
  std::optional<int> delay_ms_for_launch_optimization_;
  bool use_unlimited_media_policy_ = false;
  ThirdPartyCookiesPolicy third_party_cookies_policy_ =
      ThirdPartyCookiesPolicy::kDefault;
  int display_affinity_ = kUndefinedDisplayId;
  std::string location_hint_;
  bool use_virtual_keyboard_ = true;
  std::optional<int> custom_suspend_dom_time_;
  bool use_video_decode_accelerator_ = false;
  std::set<std::string> web_app_permissions_;
};

#endif  // CORE_APPLICATION_DESCRIPTION_H_
