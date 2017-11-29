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
#include "draco/animation/keyframe_animation_decoder.h"
#include "draco/animation/keyframe_animation_encoder.h"
#include "draco/core/draco_test_base.h"
#include "draco/core/draco_test_utils.h"

namespace draco {

class KeyframeAnimationEncodingTest : public ::testing::Test {
 protected:
  KeyframeAnimationEncodingTest() {}

  void CreateAnimationData(size_t num_frames, uint32_t num_components) {
    timestamps_.resize(num_frames);
    animation_data_.resize(num_frames * num_components);
    for (int i = 0; i < timestamps_.size(); ++i)
      timestamps_[i] = static_cast<draco::KeyframeAnimation::TimeStampType>(i);

    for (int i = 0; i < animation_data_.size(); ++i)
      animation_data_[i] = static_cast<float>(i);

    keyframe_animation_.SetAnimationData(
        num_frames, timestamps_, draco::DT_FLOAT32, num_components,
        reinterpret_cast<const char *>(&animation_data_[0]));
  }

  template <int num_components_t>
  void CompareAnimationData(const KeyframeAnimation &animation0,
                            const KeyframeAnimation &animation1) {
    ASSERT_EQ(animation0.num_frames(), animation1.num_frames());
    // Compare time stamp.
    const auto timestamp_att0 = animation0.timestamp();
    const auto timestamp_att1 = animation0.timestamp();
    for (int i = 0; i < animation0.num_frames(); ++i) {
      std::array<float, 1> att_value0;
      std::array<float, 1> att_value1;
      ASSERT_TRUE((timestamp_att0->GetValue<float, 1>(
          draco::AttributeValueIndex(i), &att_value0)));
      ASSERT_TRUE((timestamp_att1->GetValue<float, 1>(
          draco::AttributeValueIndex(i), &att_value1)));
      ASSERT_FLOAT_EQ(att_value0[0], att_value1[0]);
    }

    // Compare keyframe data.
    const auto keyframe_att0 = animation0.keyframe_data();
    const auto keyframe_att1 = animation1.keyframe_data();
    for (int i = 0; i < animation0.num_frames(); ++i) {
      std::array<float, num_components_t> att_value0;
      std::array<float, num_components_t> att_value1;
      ASSERT_TRUE((keyframe_att0->GetValue<float, num_components_t>(
          draco::AttributeValueIndex(i), &att_value0)));
      ASSERT_TRUE((keyframe_att1->GetValue<float, num_components_t>(
          draco::AttributeValueIndex(i), &att_value1)));
      for (int j = 0; j < att_value0.size(); ++j) {
        ASSERT_FLOAT_EQ(att_value0[j], att_value1[j]);
      }
    }
  }

  template <int num_components_t>
  void TestKeyframeAnimationEncoding(size_t num_frames,
                                     uint32_t num_components) {
    CreateAnimationData(num_frames, num_components);
    keyframe_animation_.SetAnimationData(
        num_frames, timestamps_, draco::DT_FLOAT32, num_components,
        reinterpret_cast<const char *>(&animation_data_[0]));

    // Encode
    draco::EncoderBuffer buffer;
    draco::KeyframeAnimationEncoder encoder;
    EncoderOptions options = EncoderOptions::CreateDefaultOptions();

    ASSERT_TRUE(
        encoder.EncodeKeyframeAnimation(keyframe_animation_, options, &buffer)
            .ok());

    draco::DecoderBuffer dec_decoder;
    draco::KeyframeAnimationDecoder decoder;
    DecoderBuffer dec_buffer;
    dec_buffer.Init(buffer.data(), buffer.size());

    std::unique_ptr<KeyframeAnimation> decoded_animation(
        new KeyframeAnimation());
    DecoderOptions dec_options;
    ASSERT_TRUE(
        decoder.Decode(dec_options, &dec_buffer, decoded_animation.get()).ok());

    CompareAnimationData<num_components_t>(keyframe_animation_,
                                           *decoded_animation);
  }

  draco::KeyframeAnimation keyframe_animation_;
  std::vector<draco::KeyframeAnimation::TimeStampType> timestamps_;
  std::vector<float> animation_data_;
};

TEST_F(KeyframeAnimationEncodingTest, OneComponent) {
  TestKeyframeAnimationEncoding<1>(1, 1);
}

TEST_F(KeyframeAnimationEncodingTest, ManyComponents) {
  TestKeyframeAnimationEncoding<100>(100, 100);
}

}  // namespace draco
