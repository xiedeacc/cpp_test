/*******************************************************************************
 * Copyright (c) 2024  xiedeacc.com.
 * All rights reserved.
 *******************************************************************************/

#include "config.pb.h"
#include "folly/Singleton.h"
#include "folly/init/Init.h"
#include "gflags/gflags.h"
#include "glog/logging.h"
#include "google/protobuf/util/json_util.h"

class ConfigManager {
 private:
  friend class folly::Singleton<ConfigManager>;
  ConfigManager() = default;

 public:
  static std::shared_ptr<ConfigManager> Instance() {
    return folly::Singleton<ConfigManager>::try_get();
  }

  bool Init() {
    std::string json = "x";
    google::protobuf::util::JsonParseOptions option;
    option.case_insensitive_enum_parsing = false;
    option.ignore_unknown_fields = true;
    if (!google::protobuf::util::JsonStringToMessage(json, &base_config_)
             .ok()) {
      LOG(ERROR) << "parse json error, content: " << json;
      return false;
    }
    return true;
  }

  std::string ToString() {
    google::protobuf::util::JsonPrintOptions option;
    std::string json;
    if (!google::protobuf::json::MessageToJsonString(base_config_, &json,
                                                     option)
             .ok()) {
      LOG(ERROR) << "to json string failed";
    }
    return json;
  }

 private:
  bazel_template::proto::BaseConfig base_config_;
};

static folly::Singleton<ConfigManager> config_manager;
int main(int argc, char **argv) {
  char *arr = new char[8];
  LOG(INFO) << "TEST";
  strcpy(arr, "test");
  LOG(INFO) << arr;

  folly::Init init(&argc, &argv, false);
  google::SetStderrLogging(google::INFO);
  google::ParseCommandLineFlags(&argc, &argv, false);
  ConfigManager::Instance()->Init();
  LOG(INFO) << ConfigManager::Instance()->ToString();
}
