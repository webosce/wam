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

#include "web_app_wayland.h"

#include <sstream>
#include <unordered_map>

#include <json/json.h>
#include "webos/common/webos_constants.h"
#include "webos/window_group_configuration.h"

#include "application_description.h"
#include "log_manager.h"
#include "utils.h"
#include "web_app_wayland_window.h"
#include "web_app_window_impl.h"
#include "web_page_base.h"
#include "web_page_blink.h"
#include "window_types.h"

namespace {

static int kLaunchFinishAssureTimeoutMs = 5000;

const std::unordered_map<std::string, webos::WebOSKeyMask>& GetKeyMaskTable() {
  static const std::unordered_map<std::string, webos::WebOSKeyMask> map_table{
      {"KeyMaskNone", static_cast<webos::WebOSKeyMask>(0)},
      {"KeyMaskHome", webos::WebOSKeyMask::KEY_MASK_HOME},
      {"KeyMaskBack", webos::WebOSKeyMask::KEY_MASK_BACK},
      {"KeyMaskExit", webos::WebOSKeyMask::KEY_MASK_EXIT},
      {"KeyMaskLeft", webos::WebOSKeyMask::KEY_MASK_LEFT},
      {"KeyMaskRight", webos::WebOSKeyMask::KEY_MASK_RIGHT},
      {"KeyMaskUp", webos::WebOSKeyMask::KEY_MASK_UP},
      {"KeyMaskDown", webos::WebOSKeyMask::KEY_MASK_DOWN},
      {"KeyMaskOk", webos::WebOSKeyMask::KEY_MASK_OK},
      {"KeyMaskNumeric", webos::WebOSKeyMask::KEY_MASK_NUMERIC},
      {"KeyMaskRed", webos::WebOSKeyMask::KEY_MASK_REMOTECOLORRED},
      {"KeyMaskGreen", webos::WebOSKeyMask::KEY_MASK_REMOTECOLORGREEN},
      {"KeyMaskYellow", webos::WebOSKeyMask::KEY_MASK_REMOTECOLORYELLOW},
      {"KeyMaskBlue", webos::WebOSKeyMask::KEY_MASK_REMOTECOLORBLUE},
      {"KeyMaskProgramme", webos::WebOSKeyMask::KEY_MASK_REMOTEPROGRAMMEGROUP},
      {"KeyMaskPlayback", webos::WebOSKeyMask::KEY_MASK_REMOTEPLAYBACKGROUP},
      {"KeyMaskTeletext", webos::WebOSKeyMask::KEY_MASK_REMOTETELETEXTGROUP},
      {"KeyMaskDefault", webos::WebOSKeyMask::KEY_MASK_DEFAULT},
  };
  return map_table;
}

webos::WebOSKeyMask GetKeyMask(const std::string& key) {
  static const auto& map_table = GetKeyMaskTable();
  auto iter = map_table.find(key);
  return iter != map_table.end() ? iter->second
                                 : static_cast<webos::WebOSKeyMask>(0);
}

}  // namespace

WebAppWayland::WebAppWayland(const std::string& type,
                             std::optional<int> width,
                             std::optional<int> height,
                             int display_id,
                             const std::string& location_hint)
    : window_type_(type),
      display_id_(display_id),
      location_hint_(location_hint) {
  Init(width, height);
}

WebAppWayland::WebAppWayland(const std::string& type,
                             WebAppWaylandWindow* window,
                             std::optional<int> width,
                             std::optional<int> height,
                             int display_id,
                             const std::string& location_hint)
    : app_window_(std::make_unique<WebAppWindowImpl>(
          std::unique_ptr<WebAppWaylandWindow>(window))),
      window_type_(type),
      display_id_(display_id),
      location_hint_(location_hint) {
  Init(width, height);
}

WebAppWayland::WebAppWayland(const std::string& type,
                             std::unique_ptr<WebAppWindowFactory> factory,
                             std::optional<int> width,
                             std::optional<int> height,
                             int display_id,
                             const std::string& location_hint)
    : window_type_(type),
      display_id_(display_id),
      location_hint_(location_hint),
      window_factory_(std::move(factory)) {
  Init(width, height);
}

static webos::WebAppWindowBase::LocationHint GetLocationHintFromString(
    const std::string& value) {
  std::map<std::string, webos::WebAppWindowBase::LocationHint> hints = {
      {"north", webos::WebAppWindowBase::LocationHint::kNorth},
      {"west", webos::WebAppWindowBase::LocationHint::kWest},
      {"south", webos::WebAppWindowBase::LocationHint::kSouth},
      {"east", webos::WebAppWindowBase::LocationHint::kEast},
      {"center", webos::WebAppWindowBase::LocationHint::kCenter},
      {"northwest", webos::WebAppWindowBase::LocationHint::kNorthWest},
      {"northeast", webos::WebAppWindowBase::LocationHint::kNorthEast},
      {"southwest", webos::WebAppWindowBase::LocationHint::kSouthWest},
      {"southeast", webos::WebAppWindowBase::LocationHint::kSouthEast}};

  webos::WebAppWindowBase::LocationHint hint =
      webos::WebAppWindowBase::LocationHint::kUnknown;
  if (hints.contains(value)) {
    hint = hints[value];
  }
  return hint;
}

void WebAppWayland::Init(std::optional<int> width, std::optional<int> height) {
  if (!app_window_) {
    if (window_factory_) {
      app_window_ =
          std::unique_ptr<WebAppWindow>(window_factory_->CreateWindow());
    } else {
      app_window_ = std::make_unique<WebAppWindowImpl>(
          std::unique_ptr<WebAppWaylandWindow>(WebAppWaylandWindow::Take()));
    }
  }
  if (width.has_value() && height.has_value()) {
    SetUiSize(width.value(), height.value());
    app_window_->InitWindow(width.value(), height.value());
  } else {
    SetUiSize(app_window_->DisplayWidth(), app_window_->DisplayHeight());
    app_window_->InitWindow(app_window_->DisplayWidth(),
                            app_window_->DisplayHeight());
  }

  webos::WebAppWindowBase::LocationHint location_hint =
      GetLocationHintFromString(location_hint_);
  if (location_hint != webos::WebAppWindowBase::LocationHint::kUnknown) {
    app_window_->SetLocationHint(location_hint);
  }

  app_window_->SetWebApp(this);

  // set compositor window type
  SetWindowProperty("_WEBOS_WINDOW_TYPE", window_type_);
  LOG_DEBUG("App created window [%s]", window_type_.c_str());

  if (display_id_ != kUndefinedDisplayId) {
    SetWindowProperty("displayAffinity", std::to_string(display_id_));
    LOG_DEBUG("App window for display[%d]", display_id_);
  }

  int timeout = util::StrToIntWithDefault(
      util::GetEnvVar("LAUNCH_FINISH_ASSURE_TIMEOUT"), 0);
  if (timeout != 0) {
    kLaunchFinishAssureTimeoutMs = timeout;
  }

  if (!webos::WebOSPlatform::GetInstance()->GetInputPointer()) {
    // Create InputManager instance.
    InputManager::Instance();
  }
}

void WebAppWayland::StartLaunchTimer() {
  if (!GetHiddenWindow()) {
    LOG_DEBUG("APP_LAUNCHTIME_CHECK_STARTED [appId:%s]", AppId().c_str());
    elapsed_launch_timer_.Start();
  }
}

void WebAppWayland::OnDelegateWindowFrameSwapped() {
  if (elapsed_launch_timer_.IsRunning()) {
    last_swapped_time_ = elapsed_launch_timer_.ElapsedMs();

    launch_timeout_timer_.Stop();
    launch_timeout_timer_.StartWithReceiver(kLaunchFinishAssureTimeoutMs, this,
                                            &WebAppWayland::OnLaunchTimeout);
  }
}

void WebAppWayland::OnLaunchTimeout() {
  if (elapsed_launch_timer_.IsRunning()) {
    launch_timeout_timer_.Stop();
    elapsed_launch_timer_.Stop();
    LOG_DEBUG("APP_LAUNCHTIME_CHECK_ALL_FRAMES_DONE [appId:%s time:%d]",
              AppId().c_str(), last_swapped_time_);
  }
}

void WebAppWayland::ForwardWebOSEvent(WebOSEvent* event) const {
  Page()->ForwardEvent(event);
}

void WebAppWayland::Attach(WebPageBase* page) {
  WebAppBase::Attach(page);

  SetWindowProperty("appId", AppId());
  SetWindowProperty("instanceId", InstanceId());
  SetWindowProperty("launchingAppId", LaunchingAppId());
  SetWindowProperty("title", GetAppDescription()->Title());
  SetWindowProperty("icon", GetAppDescription()->Icon());
  SetWindowProperty("subtitle", std::string());
  SetWindowProperty("_WEBOS_WINDOW_CLASS",
                    std::to_string(static_cast<int>(
                        GetAppDescription()->WindowClassValue())));
  SetWindowProperty(
      "_WEBOS_ACCESS_POLICY_KEYS_BACK",
      GetAppDescription()->BackHistoryAPIDisabled() ? "true" : "false");
  SetWindowProperty("_WEBOS_ACCESS_POLICY_KEYS_EXIT",
                    GetAppDescription()->HandleExitKey() ? "true" : "false");
  SetKeyMask(webos::WebOSKeyMask::KEY_MASK_BACK,
             GetAppDescription()->BackHistoryAPIDisabled());
  SetKeyMask(webos::WebOSKeyMask::KEY_MASK_EXIT,
             GetAppDescription()->HandleExitKey());

  if (GetAppDescription()->WidthOverride().has_value() &&
      GetAppDescription()->HeightOverride().has_value() &&
      !GetAppDescription()->IsTransparent()) {
    float scale_x = static_cast<float>(app_window_->DisplayWidth()) /
                    GetAppDescription()->WidthOverride().value();
    float scale_y = static_cast<float>(app_window_->DisplayHeight()) /
                    GetAppDescription()->HeightOverride().value();
    scale_factor_ = (scale_x < scale_y) ? scale_x : scale_y;
    static_cast<WebPageBlink*>(page)->SetAdditionalContentsScale(scale_x,
                                                                 scale_y);
  }

  DoAttach();

  static_cast<WebPageBlink*>(this->Page())->SetObserver(this);
}

WebPageBase* WebAppWayland::Detach() {
  static_cast<WebPageBlink*>(Page())->SetObserver(nullptr);
  return WebAppBase::Detach();
}

void WebAppWayland::SuspendAppRendering() {
  OnStageDeactivated();
  app_window_->Hide();
}

void WebAppWayland::ResumeAppRendering() {
  app_window_->Show();
  OnStageActivated();
}

bool WebAppWayland::IsFocused() const {
  return is_focused_;
}

void WebAppWayland::Resize(int width, int height) {
  app_window_->Resize(width, height);
}

bool WebAppWayland::IsActivated() const {
  return app_window_->GetWindowHostState() == webos::NATIVE_WINDOW_FULLSCREEN ||
         app_window_->GetWindowHostState() == webos::NATIVE_WINDOW_MAXIMIZED ||
         app_window_->GetWindowHostState() == webos::NATIVE_WINDOW_DEFAULT;
}

bool WebAppWayland::IsMinimized() {
  return app_window_->GetWindowHostState() == webos::NATIVE_WINDOW_MINIMIZED;
}

bool WebAppWayland::IsNormal() {
  return app_window_->GetWindowHostState() == webos::NATIVE_WINDOW_DEFAULT;
}

void WebAppWayland::OnStageActivated() {
  if (GetCrashState()) {
    LOG_INFO(MSGID_WEBAPP_STAGE_ACITVATED, 4,
             PMLOGKS("APP_ID", AppId().c_str()),
             PMLOGKS("INSTANCE_ID", InstanceId().c_str()),
             PMLOGKFV("PID", "%d", Page()->GetWebProcessPID()),
             PMLOGKS("getCrashState()", "true; Reload default Page"), "");
    Page()->ReloadDefaultPage();
    SetCrashState(false);
  }

  Page()->ResumeWebPageAll();

  Page()->SetVisibilityState(
      WebPageBase::WebPageVisibilityState::kWebPageVisibilityStateVisible);

  SetActiveInstanceId(InstanceId());

  app_window_->Show();

  LOG_INFO(MSGID_WEBAPP_STAGE_ACITVATED, 3, PMLOGKS("APP_ID", AppId().c_str()),
           PMLOGKS("INSTANCE_ID", InstanceId().c_str()),
           PMLOGKFV("PID", "%d", Page()->GetWebProcessPID()), "");
}

void WebAppWayland::OnStageDeactivated() {
  Page()->SuspendWebPageMedia();
  Unfocus();
  Page()->SetVisibilityState(
      WebPageBase::WebPageVisibilityState::kWebPageVisibilityStateHidden);
  Page()->SuspendWebPageAll();
  SetHiddenWindow(true);

  LOG_INFO(MSGID_WEBAPP_STAGE_DEACITVATED, 3,
           PMLOGKS("APP_ID", AppId().c_str()),
           PMLOGKS("INSTANCE_ID", InstanceId().c_str()),
           PMLOGKFV("PID", "%d", Page()->GetWebProcessPID()), "");
  did_activate_stage_ = false;
}

void WebAppWayland::ConfigureWindow(const std::string& type) {
  window_type_ = type;
  app_window_->SetWebApp(this);

  SetWindowProperty("_WEBOS_WINDOW_TYPE", type);
  SetWindowProperty("appId", AppId());
  SetWindowProperty("instanceId", InstanceId());
  SetWindowProperty("launchingAppId", LaunchingAppId());
  SetWindowProperty("title", GetAppDescription()->Title());
  SetWindowProperty("icon", GetAppDescription()->Icon());
  SetWindowProperty("subtitle", std::string());
  SetWindowProperty("_WEBOS_WINDOW_CLASS",
                    std::to_string(static_cast<int>(
                        GetAppDescription()->WindowClassValue())));
  SetWindowProperty(
      "_WEBOS_ACCESS_POLICY_KEYS_BACK",
      GetAppDescription()->BackHistoryAPIDisabled() ? "true" : "false");
  SetWindowProperty("_WEBOS_ACCESS_POLICY_KEYS_EXIT",
                    GetAppDescription()->HandleExitKey() ? "true" : "false");
  SetKeyMask(webos::WebOSKeyMask::KEY_MASK_BACK,
             GetAppDescription()->BackHistoryAPIDisabled());
  SetKeyMask(webos::WebOSKeyMask::KEY_MASK_EXIT,
             GetAppDescription()->HandleExitKey());

  ApplicationDescription* app_desc = GetAppDescription();
  if (!app_desc->GroupWindowDesc().empty()) {
    SetupWindowGroup(app_desc);
  }
}

void WebAppWayland::SetupWindowGroup(ApplicationDescription* desc) {
  if (!desc) {
    return;
  }

  ApplicationDescription::WindowGroupInfo group_info =
      desc->GetWindowGroupInfo();
  if (group_info.name.empty()) {
    return;
  }

  if (group_info.is_owner) {
    ApplicationDescription::WindowOwnerInfo owner_info =
        desc->GetWindowOwnerInfo();
    webos::WindowGroupConfiguration config(group_info.name);
    config.SetIsAnonymous(owner_info.allow_anonymous);

    auto iter = owner_info.layers.begin();
    while (iter != owner_info.layers.end()) {
      config.AddLayer(
          webos::WindowGroupLayerConfiguration(iter->first, iter->second));
      iter++;
    }
    app_window_->CreateWindowGroup(config);
    LOG_INFO(MSGID_CREATE_SURFACEGROUP, 3, PMLOGKS("APP_ID", AppId().c_str()),
             PMLOGKS("INSTANCE_ID", InstanceId().c_str()),
             PMLOGKFV("PID", "%d", Page()->GetWebProcessPID()), "");
  } else {
    ApplicationDescription::WindowClientInfo client_info =
        desc->GetWindowClientInfo();
    app_window_->AttachToWindowGroup(group_info.name, client_info.layer);
    LOG_INFO(MSGID_ATTACH_SURFACEGROUP, 4, PMLOGKS("APP_ID", AppId().c_str()),
             PMLOGKS("OWNER_ID", group_info.name.c_str()),
             PMLOGKS("INSTANCE_ID", InstanceId().c_str()),
             PMLOGKFV("PID", "%d", Page()->GetWebProcessPID()), "");
  }
}

bool WebAppWayland::IsKeyboardVisible() {
  return app_window_->IsKeyboardVisible();
}

void WebAppWayland::SetKeyMask(webos::WebOSKeyMask key_mask, bool value) {
  app_window_->SetKeyMask(key_mask, value);
}

void WebAppWayland::ApplyInputRegion() {
  if (!enable_input_region_ && !input_region_.empty()) {
    enable_input_region_ = true;
    app_window_->SetInputRegion(input_region_);
  }
}

void WebAppWayland::SetInputRegion(const Json::Value& value) {
  input_region_.clear();

  if (value.isArray()) {
    for (const auto& region : value) {
      input_region_.emplace_back(
          gfx::Rect(region["x"].asInt() * scale_factor_,
                    region["y"].asInt() * scale_factor_,
                    region["width"].asInt() * scale_factor_,
                    region["height"].asInt() * scale_factor_));
    }
  }

  app_window_->SetInputRegion(input_region_);
}

void WebAppWayland::SetWindowProperty(const std::string& name,
                                      const std::string& value) {
  webos::WebOSKeyMask mask = static_cast<webos::WebOSKeyMask>(0);
  if (name == "_WEBOS_ACCESS_POLICY_KEYS_BACK") {
    mask = webos::WebOSKeyMask::KEY_MASK_BACK;
  } else if (name == "_WEBOS_ACCESS_POLICY_KEYS_EXIT") {
    mask = webos::WebOSKeyMask::KEY_MASK_EXIT;
  }
  // if mask is not set, not need to call setKeyMask
  if (mask != static_cast<webos::WebOSKeyMask>(0)) {
    SetKeyMask(mask, value == "true");
  }
  app_window_->SetWindowProperty(name, value);
}

void WebAppWayland::PlatformBack() {
  app_window_->PlatformBack();
}

void WebAppWayland::SetCursor(const std::string& cursor_arg,
                              int hotspot_x,
                              int hotspot_y) {
  app_window_->SetCursor(cursor_arg, hotspot_x, hotspot_y);
}

void WebAppWayland::SetKeyMask(const Json::Value& value) {
  unsigned int key_mask = 0;
  if (value.isArray()) {
    for (const auto& child : value) {
      key_mask |= GetKeyMask(child.asString());
    }
  }

  app_window_->SetKeyMask(static_cast<webos::WebOSKeyMask>(key_mask));
}

void WebAppWayland::SetKeyMask(webos::WebOSKeyMask key_mask) {
  app_window_->SetKeyMask(key_mask);
}

void WebAppWayland::FocusOwner() {
  app_window_->FocusWindowGroupOwner();
  LOG_DEBUG("FocusOwner [%s]", AppId().c_str());
}

void WebAppWayland::FocusLayer() {
  app_window_->FocusWindowGroupLayer();
  ApplicationDescription* desc = GetAppDescription();
  if (desc) {
    ApplicationDescription::WindowClientInfo client_info =
        desc->GetWindowClientInfo();
    LOG_DEBUG("FocusLayer(layer:%s) [%s]", client_info.layer.c_str(),
              AppId().c_str());
  }
}

void WebAppWayland::SetOpacity(float opacity) {
  app_window_->SetOpacity(opacity);
}

void WebAppWayland::Hide(bool forced_hide) {
  if (KeepAlive() || forced_hide) {
    OnStageDeactivated();
    app_window_->Hide();
    SetHiddenWindow(true);
  }
}

void WebAppWayland::Focus() {
  is_focused_ = true;
  if (!IsMinimized()) {
    Page()->SetFocus(true);
  }
}

void WebAppWayland::Unfocus() {
  is_focused_ = false;
  Page()->SetFocus(false);
}

void WebAppWayland::DoAttach() {
  // Do App and window things
  ApplicationDescription* app_desc = GetAppDescription();
  if (!app_desc->GroupWindowDesc().empty()) {
    SetupWindowGroup(app_desc);
  }

  app_window_->AttachWebContents(Page()->GetWebContents());
  // The attachWebContents causes visibilityState change to Visible (by default,
  // init) And now, should update the visibilityState to launching
  Page()->SetVisibilityState(
      WebPageBase::WebPageVisibilityState::kWebPageVisibilityStateLaunching);

  // Do Page things
  Page()->SetPageProperties();

  if (KeepAlive()) {
    Page()->SetKeepAliveWebApp(KeepAlive());
  }
}

void WebAppWayland::Raise() {
  bool was_minimized_state = IsMinimized();

  // There's no fullscreen event from LSM for below cases, so onStageActivated
  // should be called
  // 1. When overlay window is raised
  // 2. When there's only one keepAlive app, and this keepAlive app is closed
  // and is shown again
  if ((GetWindowType() == kWtOverlay) ||
      (KeepAlive() && !was_minimized_state)) {
    LOG_INFO(MSGID_WAM_DEBUG, 3, PMLOGKS("APP_ID", AppId().c_str()),
             PMLOGKS("INSTANCE_ID", InstanceId().c_str()),
             PMLOGKFV("PID", "%d", Page()->GetWebProcessPID()),
             "WebAppWayland::raise(); call onStageActivated");
    OnStageActivated();
  } else {
    LOG_INFO(MSGID_WAM_DEBUG, 3, PMLOGKS("APP_ID", AppId().c_str()),
             PMLOGKS("INSTANCE_ID", InstanceId().c_str()),
             PMLOGKFV("PID", "%d", Page()->GetWebProcessPID()),
             "WebAppWayland::raise(); call "
             "setWindowState(webos::NATIVE_WINDOW_FULLSCREEN)");
    app_window_->SetWindowHostState(webos::NATIVE_WINDOW_FULLSCREEN);
  }

  if (was_minimized_state) {
    // When resuming a web app from the launcher, that entry point is
    // reached. So, before changing the page visibility, the DOM has to be
    // resumed (if suspended - this is handled inside resumeWebPageAll()).
    // Otherwise, corresponding event will never be delivered to its
    // listener(s) (if any) on the JS layer.
    Page()->ResumeWebPageAll();
    Page()->SetVisibilityState(
        WebPageBase::WebPageVisibilityState::kWebPageVisibilityStateVisible);
  }
}

void WebAppWayland::GoBackground() {
  if (GetWindowType() == kWtOverlay) {
    LOG_INFO(MSGID_WAM_DEBUG, 3, PMLOGKS("APP_ID", AppId().c_str()),
             PMLOGKS("INSTANCE_ID", InstanceId().c_str()),
             PMLOGKFV("PID", "%d", Page()->GetWebProcessPID()),
             "WebAppWayland::goBackground(); windowType:OVERLAY; Try close; "
             "call doClose()");
    DoClose();
  } else {
    LOG_INFO(MSGID_WAM_DEBUG, 3, PMLOGKS("APP_ID", AppId().c_str()),
             PMLOGKS("INSTANCE_ID", InstanceId().c_str()),
             PMLOGKFV("PID", "%d", Page()->GetWebProcessPID()),
             "WebAppWayland::goBackground(); call "
             "setWindowState(webos::NATIVE_WINDOW_MINIMIZED)");
    app_window_->SetWindowHostState(webos::NATIVE_WINDOW_MINIMIZED);
  }
}

void WebAppWayland::WebPageLoadFinished() {
  if (GetHiddenWindow()) {
    return;
  }
  if (NeedReload()) {
    Page()->Reload();
    SetNeedReload(false);
    return;
  }

  DoPendingRelaunch();
}

void WebAppWayland::WebPageLoadFailed(int /*error_code*/) {
  // Do not load error page while preoload app launching.
  if (GetPreloadState() != kNonePreload) {
    CloseAppInternal();
  }
}

void WebAppWayland::DoClose() {
  if (ForceClose()) {
    LOG_INFO(MSGID_WAM_DEBUG, 3, PMLOGKS("APP_ID", AppId().c_str()),
             PMLOGKS("INSTANCE_ID", InstanceId().c_str()),
             PMLOGKFV("PID", "%d", Page()->GetWebProcessPID()),
             "WebAppWayland::doClose(); forceClose() TRUE; call "
             "forceCloseAppInternal() and return");
    ForceCloseAppInternal();
    return;
  }

  if (KeepAlive() && HideWindow()) {
    return;
  }

  LOG_INFO(MSGID_WAM_DEBUG, 3, PMLOGKS("APP_ID", AppId().c_str()),
           PMLOGKS("INSTANCE_ID", InstanceId().c_str()),
           PMLOGKFV("PID", "%d", Page()->GetWebProcessPID()),
           "WebAppWayland::doClose(); call closeAppInternal()");
  CloseAppInternal();
}

void WebAppWayland::StateAboutToChange(webos::NativeWindowState will_be) {
  if (will_be == webos::NATIVE_WINDOW_MINIMIZED) {
    LOG_INFO(MSGID_WAM_DEBUG, 3, PMLOGKS("APP_ID", AppId().c_str()),
             PMLOGKS("INSTANCE_ID", InstanceId().c_str()),
             PMLOGKFV("PID", "%d", Page()->GetWebProcessPID()),
             "WebAppWayland::stateAboutToChange; will be Minimized; suspend "
             "media and fire visibilitychange event");
    Page()->SuspendWebPageMedia();
    Page()->SetVisibilityState(
        WebPageBase::WebPageVisibilityState::kWebPageVisibilityStateHidden);
  }
}

void WebAppWayland::StateChanged(webos::NativeWindowState new_state) {
  if (IsClosing()) {
    LOG_INFO(MSGID_WINDOW_STATE_CHANGED, 1, PMLOGKS("APP_ID", AppId().c_str()),
             "In Closing; return;");
    return;
  }

  switch (new_state) {
    case webos::NATIVE_WINDOW_DEFAULT:
    case webos::NATIVE_WINDOW_MAXIMIZED:
    case webos::NATIVE_WINDOW_FULLSCREEN:
      LOG_INFO(MSGID_WINDOW_STATE_CHANGED, 1,
               PMLOGKS("APP_ID", AppId().c_str()),
               "To FullScreen; call onStageActivated");
      ApplyInputRegion();
      OnStageActivated();
      break;
    case webos::NATIVE_WINDOW_MINIMIZED:
      LOG_INFO(MSGID_WINDOW_STATE_CHANGED, 1,
               PMLOGKS("APP_ID", AppId().c_str()),
               "To Minimized; call onStageDeactivated");
      OnStageDeactivated();
      break;
    default:
      LOG_INFO(MSGID_WINDOW_STATE_CHANGED, 2,
               PMLOGKS("APP_ID", AppId().c_str()),
               PMLOGKFV("HOST_STATE", "%d", new_state),
               "Unknown state. Do not calling nothing anymore.");
      break;
  }
}

void WebAppWayland::ShowWindow() {
  if (preload_state_ != kNonePreload) {
    LOG_INFO(MSGID_WAM_DEBUG, 3, PMLOGKS("APP_ID", AppId().c_str()),
             PMLOGKS("INSTANCE_ID", InstanceId().c_str()),
             PMLOGKFV("PID", "%d", Page()->GetWebProcessPID()),
             "WebAppWayland::showWindow(); But Preloaded app; return");
    return;
  }

  SetHiddenWindow(false);

  OnStageActivated();
  added_to_window_mgr_ = true;
  WebAppBase::ShowWindow();
}

bool WebAppWayland::HideWindow() {
  if (Page()->IsLoadErrorPageFinish()) {
    return false;
  }

  LOG_INFO(MSGID_WAM_DEBUG, 3, PMLOGKS("APP_ID", AppId().c_str()),
           PMLOGKS("INSTANCE_ID", InstanceId().c_str()),
           PMLOGKFV("PID", "%d", Page()->GetWebProcessPID()),
           "WebAppWayland::hideWindow(); just hide this app");
  Page()->CloseVkb();
  Hide(true);
  added_to_window_mgr_ = false;
  return true;
}

void WebAppWayland::TitleChanged() {
  SetWindowProperty("subtitle", Page()->Title());
}

void WebAppWayland::FirstFrameVisuallyCommitted() {
  LOG_INFO(MSGID_WAM_DEBUG, 3, PMLOGKS("APP_ID", AppId().c_str()),
           PMLOGKS("INSTANCE_ID", InstanceId().c_str()),
           PMLOGKFV("PID", "%d", Page()->GetWebProcessPID()),
           "firstFrameVisuallyCommitted");
  // if preload_state_ != NONE_PRELOAD, then we must ignore the first frame
  // commit if getHiddenWindow() == true, then we have specifically requested
  // that the window is to be hidden, and therefore we have to do an explicit
  // show
  if (!GetHiddenWindow() && preload_state_ == kNonePreload) {
    LOG_INFO(MSGID_WAM_DEBUG, 3, PMLOGKS("APP_ID", AppId().c_str()),
             PMLOGKS("INSTANCE_ID", InstanceId().c_str()),
             PMLOGKFV("PID", "%d", Page()->GetWebProcessPID()),
             "Not hidden window, preload, call showWindow");
    if (GetAppDescription()->UsePrerendering()) {
      did_activate_stage_ = false;
    }
    ShowWindow();
  }
}

void WebAppWayland::PostEvent(WebOSEvent* ev) {
  app_window_->Event(ev);
}

void WebAppWayland::NavigationHistoryChanged() {
  if (!GetAppDescription()->BackHistoryAPIDisabled()) {
    // if backHistoryAPIDisabled is true, no chance to change this value
    SetWindowProperty(
        "_WEBOS_ACCESS_POLICY_KEYS_BACK",
        Page()->CanGoBack() ? "true" : /* send next back key to WAM */
            "false"); /* Do not send back key to WAM. LSM should handle it */
  }
}

void WebAppWayland::WebViewRecreated() {
  app_window_->AttachWebContents(Page()->GetWebContents());
  app_window_->RecreatedWebContents();
  Page()->SetPageProperties();
  if (KeepAlive()) {
    Page()->SetKeepAliveWebApp(KeepAlive());
  }
  Focus();
}

void WebAppWayland::DidSwapPageCompositorFrame() {
  if (!did_activate_stage_ && !GetHiddenWindow() &&
      preload_state_ == kNonePreload) {
    LOG_INFO(MSGID_WAM_DEBUG, 2, PMLOGKS("APP_ID", AppId().c_str()),
             PMLOGKFV("PID", "%d", Page()->GetWebProcessPID()),
             "Not hidden window, preload, activate stage");
    OnStageActivated();
    did_activate_stage_ = true;
  }
}

void WebAppWayland::DidResumeDOM() {
  Focus();
}

void InputManager::OnCursorVisibilityChanged(bool visible) {
  if (IsVisible() == visible) {
    return;
  }

  LOG_DEBUG(
      "InputManager::onCursorVisibilityChanged; Global Cursor visibility "
      "Changed to %s; send cursorStateChange event to all app, all frames",
      visible ? "true" : " false");
  SetVisible(visible);
  // send event about  cursorStateChange
  std::stringstream ss;
  const std::string str = visible ? "true" : "false";
  ss << "var cursorEvent=new CustomEvent('cursorStateChange', { detail: { "
        "'visibility' :"
     << str << "} });" << "cursorEvent.visibility = " << str << ";"
     << " if(document) document.dispatchEvent(cursorEvent);";

  // send javascript event : cursorStateChange with param to All app
  // if javascript has setTimeout() like webOSlaunch or webOSRelaunch, then app
  // can not get this event when app is in background because javascript is
  // freezed and timer is too, since app is in background, timer is never fired
  WebAppBase::OnCursorVisibilityChanged(ss.str());
}

void WebAppWayland::SendWebOSMouseEvent(const std::string& event_name) {
  if (event_name == "Enter" || event_name == "Leave") {
    // send webOSMouse event to app
    std::stringstream ss;
    ss << "console.log('[WAM] fires webOSMouse event : " << event_name << "');"
       << "var mouseEvent =new CustomEvent('webOSMouse', { detail: { type : '"
       << event_name << "' }});" << "document.dispatchEvent(mouseEvent);";
    LOG_DEBUG(
        "[%s] WebAppWayland::sendWebOSMouseEvent; dispatch webOSMouse; %s",
        AppId().c_str(), event_name.c_str());
    Page()->EvaluateJavaScript(ss.str());
  }
}

void WebAppWayland::DeleteSurfaceGroup() {
  app_window_->DetachWindowGroup();
}

void WebAppWayland::SetKeepAlive(bool keep_alive) {
  WebAppBase::SetKeepAlive(keep_alive);
  if (Page()) {
    Page()->SetKeepAliveWebApp(keep_alive);
  }
}

void WebAppWayland::MoveInputRegion(int height) {
  if (!enable_input_region_) {
    return;
  }

  if (height) {
    vkb_height_ = height;
  } else {
    vkb_height_ = -vkb_height_;
  }

  std::vector<gfx::Rect> new_region;
  for (gfx::Rect rect : input_region_) {
    rect.SetRect(rect.x(), rect.y() - vkb_height_, rect.width(), rect.height());
    new_region.push_back(rect);
  }
  input_region_.clear();
  input_region_ = std::move(new_region);
  app_window_->SetInputRegion(input_region_);
}

void WebAppWayland::KeyboardVisibilityChanged(bool visible, int height) {
  WebAppBase::KeyboardVisibilityChanged(visible, height);
  MoveInputRegion(height);
}

void WebAppWayland::SetUseVirtualKeyboard(const bool enable) {
  app_window_->SetUseVirtualKeyboard(enable);
}

void WebAppWayland::SetDisplayFirstActivateTimeoutMs(uint32_t timeout) {
  app_window_->SetFirstActivateTimeoutMs(timeout);
}
