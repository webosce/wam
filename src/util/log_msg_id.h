// Copyright (c) 2014-2019 LG Electronics, Inc.
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

#ifndef UTIL_LOG_MSG_ID_H_
#define UTIL_LOG_MSG_ID_H_

// clang-format off
// Logging Message IDs

// Instrumentation for app launch timing logging
#define MSGID_APPLAUNCH_START        "APPLAUNCH_START" /** Start of app launch process */
#define MSGID_APP_LOADED              "APPLOADED" /** New App/Page load, gives APP_ID and page URL */

#define MSGID_WINDOW_CLOSED          "WINDOW_CLOSED" /* An application window closed by QCloseEvent */
#define MSGID_WINDOW_CLOSED_JS       "WINDOW_CLOSED_JS" /* Application window closed by javascript */
#define MSGID_WINDOW_FOCUSIN          "WINDOW_FOCUSIN" /* An application window is focused */
#define MSGID_WINDOW_FOCUSOUT          "WINDOW_FOCUSOUT" /* An application window is unfocused */
#define MSGID_WINDOW_STATECHANGE       "WINDOW_STATECHANGE" /* WebOSEvent::WindowStateChange event */
#define MSGID_PAGE_CLOSED            "PAGE_CLOSED" /* A page is closed (each window should have a page, noWindow apps will have two pages for the first visible window */
#define MSGID_PAGE_LOAD_FAILED       "PAGE_LOAD_FAILED" /* A page failed to load, report the app id and url it attempted to load */

#define MSGID_WINDOW_EVENT            "WINDOW_EVENT" /* WebApp Window Event */
#define MSGID_WINDOW_STATE_CHANGED     "WINDOW_STATE_CHANGED" /* Window state is changed */
#define MSGID_RESUME_ALL                     "RESUME_ALL" /* Resume JS and Media */
#define MSGID_SUSPEND_WEBPAGE_DELAYED        "SUSPEND_WEBPAGE_DELAYED" /* Delayed Suspending JS execution of WebPage */
#define MSGID_SUSPEND_WEBPAGE                "SUSPEND_WEBPAGE"  /* Suspending JS execution of  WebPage */
#define MSGID_SUSPEND_PAINTING_VISIBILITY_HIDDEN "SUSPEND_PAINTING_VISIBILITY_HIDDEN"  /* Suspending painting and set visibility : hidden */
#define MSGID_RESUME_WEBPAGE                 "RESUME_WEBPAGE"   /* Resuming JS execution of WebPage */
#define MSGID_SUSPEND_MEDIA                  "SUSPEND_MEDIA"     /* Suspend Media of WebPage */
#define MSGID_RESUME_MEDIA                   "RESUME_MEDIA"    /* Resume Media of WebPage */
#define MSGID_KEY_EVENT                      "KEY_EVENT" /* Received key event */
#define MSGID_MOUSE_BUTTON_EVENT             "MOUSE_BUTTON_EVENT" /* Received mouse button event */
#define MSGID_MOUSE_MOVE_EVENT               "MOUSE_MOVE_EVENT" /* Received mouse move event */
#define MSGID_POST_BUNDLE_MSG                "POST_BUNDLE_MSG" /* Posting WAM bundle message */
#define MSGID_HANDLE_BUNDLE_MSG              "HANDLE_BUNDLE_MSG" /* Handles WAM bundle message */
#define MSGID_KILL_APP                       "KILL_APP" /* Killing App */
#define MSGID_PAUSE_APP                      "PAUSE_APP" /* Pausing App */
#define MSGID_FORCE_CLOSE_KEEP_ALIVE_APP     "FORCE_CLOSE_KEEP_ALIVE_APP" /* Keep Alive App is closed by force */
#define MSGID_WEBPROC_CRASH         "WEBPROC_CRASH" /* Web process crashed */
#define MSGID_BACKKEY_HANDLE     "BACKKEY_HANDLE" /* About back key handling */
#define MSGID_PAGE_LOADING          "PAGE_LOADING" /* About page loading */
#define MSGID_LOAD          "LOAD" /* About page loading */
#define MSGID_PALMSYSTEM        "PALMSYSTEM" /* About MSGID_PALMSYSTEM */
#define MSGID_POST_RUNNING_APPS     "MSGID_POST_RUNNING_APPS" /* Post Running app Change */
#define MSGID_WAM_DEBUG     "GENERAL" /* General */
#define MSGID_LUNA_API      "LUNA_API" /* About luna api */
#define MSGID_DEEPLINKING      "DEEPLINKING" /* handle deeplinking launch/relaunch */
#define MSGID_VKB_EVENT     "VKB_EVENT" /* Received vkb event */

// All other message IDs
#define MSGID_APP_DESC_PARSE_FAIL       "APP_DESC_PARSE_FAIL" /** Failure to parse app description */
#define MSGID_APP_DESC_PARSE_OBJ        "APP_DESC_PARSE_OBJ" /** Failed to identify mandatory parts of AppDesc */
#define MSGID_REG_LS2_FAIL              "REG_LS2_FAIL" /** Failed to register to LS2 bus */
#define MSGID_REG_LS2_CAT_FAIL          "REG_LS2_CAT_FAIL" /** Failed to register category to LS2 bus */
#define MSGID_REG_LS2_ATTACH_FAIL       "REG_LS2_ATTACH_FAIL" /** Failed to attach to LS2 bus */
#define MSGID_UNREG_LS2_FAIL            "UNREG_LS2_FAIL" /** Failed to unregister LS2 service */
#define MSGID_LS2_CALL_FAIL             "LS2_CALL_FAIL" /** Failed to make LS2 call */
#define MSGID_LS2_CANCEL_NOT_ACTIVE     "LS2_CANCEL_NOT_ACTIVE" /** Failed to cancel a call because one wasn't active */
#define MSGID_LS2_CANCEL_FAIL           "LS2_CANCEL_FAIL" /** Failed to cancel a call for some other reason */
#define MSGID_PLUGIN_LOAD_FAIL          "PLUGIN_LOAD_FAIL" /** Couldn't load a plugin */
#define MSGID_BUNDLE_LOAD_FAIL          "BUNDLE_LOAD_FAIL" /** Couldn't load a bundle */
#define MSGID_LAUNCH_URL_BAD_APP_DESC   "LAUNCH_URL_BAD_APP_DESC" /** Received a bad application description to launchUrl */
#define MSGID_LOW_MEM_LAUNCH_FAIL       "LOW_MEM_LAUNCH_FAIL" /** Refused to launch an app because of low memory */
#define MSGID_LOW_MEM_NEW_PAGE_FAIL     "LOW_MEM_NEW_PAGE_FAIL" /** Refused to open a new window because of low memory */
#define MSGID_MEM_MGR_API_CALL_FAIL     "MEM_MGR_API_CALL_FAIL" /** Failed to call memory manager APIs */
#define MSGID_SIGNAL_REGISTRATION_FAIL  "SINGAL_REGISTRATION_FAIL" /** Failed to register a client to receive signals*/
#define MSGID_APP_MGR_API_CALL_FAIL     "APP_MGR_API_CALL_FAIL" /** Failed to call application manager APIs */
#define MSGID_MEMWATCH_APP_CLOSE        "MEMWATCH_APP_CLOSE" /** MemWatcher decided to close an app */
#define MSGID_PREPARE_FAIL              "PREPARE_FAIL" /** Failed to prepare window */
#define MSGID_TAKE_FAIL                 "TAKE_FAIL" /** Failed to take window */
#define MSGID_BAD_WINDOW_TYPE           "BAD_WINDOW_TYPE" /** Somehow got an unsupported window type */
#define MSGID_SETTING_SERVICE            "SETTING_SERVICE" /** Received a notification from setting service */
#define MSGID_RECEIVED_INVALID_SETTINGS "RECEIVED_INVALID_SETTINGS" /** Received invalid value from systemservice */
#define MSGID_APP_LAUNCH                "APP_LAUNCH" /** Sent when we get a request to launch an app */
#define MSGID_APP_RELAUNCH              "APP_RELAUNCH" /** Sent when we get a request to launch an app that is already running */
#define MSGID_SERVICE_CONNECT_FAIL      "SERVICE_CONNECT_FAIL" /* Failed to connect to settingsservice */
#define MSGID_DISPLAY_CONNECT_FAIL      "DISPLAY_CONNECT_FAIL" /* Failed to connect to display manager */
#define MSGID_MEMORY_CONNECT_FAIL       "MEMORY_CONNECT_FAIL" /* Failed to connect to memory manager */
#define MSGID_APPMANAGER_CONNECT_FAIL   "APPMANAGER_CONNECT_FAIL" /* Failed to connect to application manager */
#define MSGID_BOOTD_CONNECT_FAIL        "BOOTD_CONNECT_FAIL" /* Failed to connect to bootd */
#define MSGID_SECURITYMANAGER_CONNECT_FAIL        "SECURITYMANAGER_CONNECT_FAIL" /* Failed to connect to security manager */
#define MSGID_NETWORK_CONNECT_FAIL      "NETWORK_CONNECT_FAIL" /* Failed to connect to connection manager */
#define MSGID_INVALID_EVENT             "INVALID_EVENT" /* Received QEvents for a window with no app */
#define MSGID_BOOTD_SUBSCRIBE_FAIL      "BOOTD_SUBSCRIBE_FAIL" /* Failed to subscribe to bootd */
#define MSGID_ACTIVITY_MANAGER_CREATE_FAIL "ACTIVITY_MANAGER_CREATE_FAIL" /* Failed to call activitymanager create */
#define MSGID_WAM_INVALID_USER_PERMISSION  "WAM_INVALID_USER_PERMISSION" /* Failed to set valid User Privileges for webappmanager */

#define MSGID_APPID_HAS_UPPERCASE       "APPID_HAS_UPPERCASE" /** the application id has uppercase letters */

#define MSGID_ERROR_ERROR               "ERROR_PAGE_ERROR" /** Error loop -- failed to load error page! */
#define MSGID_CLOSE_CALL_FAIL           "CLOSE_CALL_FAIL" /** Failed to send closeByAppId call to sam */

#define MSGID_LOCALEINFO_READ_FAIL      "LOCALEINFO_FILE_READ_FAIL" /** Fail to read locale info from /var/luna/preferences/localInfo */

#define MSGID_EXECUTE_CLOSECALLBACK         "EXECUTE_CLOSECALLBACK" /** Execute close callback */
#define MSGID_CLEANRESOURCE_COMPLETED       "CLEANRESOURCE_COMPLETED" /** Complete clean resource by callback or unload event*/
#define MSGID_START_LAUNCHURL               "START_LAUNCHURL" /** Start LaunchUrl on WebAppManager */
#define MSGID_CLOSE_APP_INTERNAL            "CLOSE_APP_INTERNAL" /** Close App */
#define MSGID_WEBPAGE_LOAD                  "WEBPAGE_LOAD" /** Webpage load starts */
#define MSGID_WEBPAGE_LOAD_FAILED           "WEBPAGE_LOAD_FAILED" /** WebPage load failed */
#define MSGID_WEBPAGE_CLOSED                "WEBPAGE_CLOSED" /** WebPage instance deleted */
#define MSGID_WEBAPP_CLOSED                 "WEBAPP_CLOSED" /** WebApp instance deleted */
#define MSGID_WEBPAGE_RELAUNCH              "WEBPAGE_RELAUNCH" /** Relaunch WebPage */
#define MSGID_WEBAPP_STAGE_ACITVATED        "WEBAPP_STAGE_ACITVATED" /** WebApp is activate */
#define MSGID_WEBAPP_STAGE_DEACITVATED      "WEBAPP_STAGE_DEACITVATED" /** WebApp is deactivate */
#define MSGID_SETUP_LAUNCHEVENT             "SETUP_LAUNCHEVENT" /** Setup webOSLaunch event */
#define MSGID_SEND_RELAUNCHEVENT            "SEND_RELAUNCHEVENT" /** Send webOSRelaunch event */

#define MSGID_CREATE_SURFACEGROUP            "CREATE_SURFACEGROUP" /** Create surface group called by owner */
#define MSGID_DELETE_SURFACEGROUP            "DELETE_SURFACEGROUP" /** Delete surface group called by both owner and client */
#define MSGID_ATTACH_SURFACEGROUP            "ATTACH_SURFACEGROUP" /** attach surface group called by client */
#define MSGID_DETACH_SURFACEGROUP            "DETACH_SURFACEGROUP" /** detach surface group called by both owner and client */

#define MSGID_SERVICE_CALL            "SERVICE_CALL" /** Service call as a delegator of qtWebEngineProcess*/
#define MSGID_SERVICE_CALL_FAIL            "SERVICE_CALL_FAIL" /** Service call as a delegator is failed*/

#define MSGID_CONFIGD_CONNECT_FAIL      "CONFIGD_CONNECT_FAIL" /* Failed to connect to config-d */

#define MSGID_NETWORKSTATUS_INFO        "NETWORKSTATUS_INFO" /** Printing NetworkStatus Information*/

#define MSGID_NOTIFY_MEMORY_STATE            "NOTIFY_MEMORY_STATE" /** Send memory state*/

#define MSGID_TYPE_ERROR                  "DATA_TYPE_ERROR" /** Use a invalid data type **/
#define MSGID_FILE_ERROR                  "FILE_ERROR" /** Use a not existing file **/

#define MSGID_DL_ERROR                  "DL_ERROR" /** Dynamic load library error **/

#define MSGID_SET_PERMISSION      "SET_PERMISSION" /** Set permission*/
// clang-format on

#endif  // UTIL_LOG_MSG_ID_H_
