// Copyright 2017 The Draco Authors.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#include "draco/animation/keyframe_animation.h"

#include "draco/core/draco_test_base.h"

namespace {

class KeyframeAnimationTest : public ::testing::Test {
 protected:
  KeyframeAnimationTest() {}

  // TODO(zhafang): Create from binary animation data. e.g. glTF.
  void CreateAnimationData(int32_t num_frames, uint32_t num_components) {
    timestamp.resize(num_frames);
    animation_data.resize(num_frames * num_components);
    for (int i = 0; i < timestamp.size(); ++i)
      timestamp[i] = static_cast<draco::KeyframeAnimation::TimeStampType>(i);
    for (int i = 0; i < animation_data.size(); ++i)
      animation_data[i] = static_cast<float>(i);
    keyframe_animation.SetAnimationData(
        num_frames, timestamp, draco::DT_FLOAT32, num_components,
        reinterpret_cast<const char *>(&animation_data[0]));
  }

  template <int num_components_t>
  void CompareAnimationData(uint32_t num_components) {
    // Compare time stamp.
    const auto timestamp_att = keyframe_animation.timestamp();
    for (int i = 0; i < timestamp.size(); ++i) {
      std::array<float, 1> att_value;
      ASSERT_TRUE((timestamp_att->GetValue<float, 1>(
          draco::AttributeValueIndex(i), &att_value)));
      ASSERT_FLOAT_EQ(att_value[0], i);
    }

    // Compare keyframe data.
    const auto keyframe_att = keyframe_animation.keyframe_data();
    for (int i = 0; i < animation_data.size() / num_components; ++i) {
      std::array<float, num_components_t> att_value;
      ASSERT_TRUE((keyframe_att->GetValue<float, num_components_t>(
          draco::AttributeValueIndex(i), &att_value)));
      for (int j = 0; j < num_components; ++j) {
        ASSERT_FLOAT_EQ(att_value[j], i * num_components + j);
      }
    }
  }

  template <int num_components_t>
  void TestKeyframeAnimation(int32_t num_frames, uint32_t num_components) {
    CreateAnimationData(num_frames, num_components);
    keyframe_animation.SetAnimationData(
        num_frames, timestamp, draco::DT_FLOAT32, num_components,
        reinterpret_cast<const char *>(&animation_data[0]));

    CompareAnimationData<num_components_t>(num_components);
  }

  draco::KeyframeAnimation keyframe_animation;
  std::vector<draco::KeyframeAnimation::TimeStampType> timestamp;
  std::vector<float> animation_data;
};

TEST_F(KeyframeAnimationTest, OneComponent) { TestKeyframeAnimation<1>(10, 1); }

TEST_F(KeyframeAnimationTest, FourComponent) {
  TestKeyframeAnimation<4>(10, 4);
}

}  // namespace
