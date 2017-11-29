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

namespace draco {

KeyframeAnimation::KeyframeAnimation() {}

bool KeyframeAnimation::SetTimeStamp(
    const std::vector<TimeStampType> &time_stamp) {
  if (time_stamp.size() != num_frames())
    return false;
  // Add attribute for time stamp data.
  std::unique_ptr<PointAttribute> time_stamp_att =
      std::unique_ptr<PointAttribute>(new PointAttribute());
  time_stamp_att->Init(GeometryAttribute::GENERIC, NULL, 1, DT_FLOAT32, false,
                       sizeof(float), 0);
  time_stamp_att->SetIdentityMapping();
  time_stamp_att->Reset(num_frames());
  for (PointIndex i(0); i < num_frames(); ++i) {
    time_stamp_att->SetAttributeValue(time_stamp_att->mapped_index(i),
                                      &time_stamp[i.value()]);
  }
  this->SetAttribute(TimestampId, std::move(time_stamp_att));
  return true;
}

bool KeyframeAnimation::SetKeyframeData(DataType data_type,
                                        uint32_t num_components,
                                        const char *data) {
  if (data == nullptr)
    return false;
  // Add attribute for time stamp data.
  std::unique_ptr<PointAttribute> keyframe_att =
      std::unique_ptr<PointAttribute>(new PointAttribute());

  keyframe_att->Init(GeometryAttribute::GENERIC, NULL, num_components,
                     data_type, false, DataTypeLength(data_type), 0);
  keyframe_att->SetIdentityMapping();
  keyframe_att->Reset(num_frames());
  const size_t stride = num_components * DataTypeLength(data_type);
  for (PointIndex i(0); i < num_frames(); ++i) {
    keyframe_att->SetAttributeValue(keyframe_att->mapped_index(i),
                                    &data[i.value() * stride]);
  }
  this->SetAttribute(KeyframeDataId, std::move(keyframe_att));
  return true;
}

bool KeyframeAnimation::SetAnimationData(
    int32_t num_frames, const std::vector<TimeStampType> &time_stamp,
    DataType data_type, uint32_t num_components, const char *data) {
  set_num_frames(num_frames);
  if (!SetTimeStamp(time_stamp))
    return false;
  if (!SetKeyframeData(data_type, num_components, data))
    return false;

  return true;
}

}  // namespace draco
