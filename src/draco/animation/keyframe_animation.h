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
#ifndef DRACO_ANIMATION_KEYFRAME_ANIMATION_H_
#define DRACO_ANIMATION_KEYFRAME_ANIMATION_H_

#include <vector>

#include "draco/point_cloud/point_cloud.h"

namespace draco {

class KeyframeAnimation : public PointCloud {
 public:
  // Force time stamp to be float type.
  using TimeStampType = float;

  KeyframeAnimation();

  bool SetAnimationData(int32_t num_frames,
                        const std::vector<TimeStampType> &time_stamp,
                        DataType data_type, uint32_t num_components,
                        const char *data);

  PointAttribute *timestamp() { return attribute(TimestampId); }
  const PointAttribute *timestamp() const { return attribute(TimestampId); }

  PointAttribute *keyframe_data() { return attribute(KeyframeDataId); }
  const PointAttribute *keyframe_data() const {
    return attribute(KeyframeDataId);
  }

  // Number of frames should be equal to number of points in the point cloud.
  void set_num_frames(int32_t num_frames) { set_num_points(num_frames); }
  int32_t num_frames() const { return static_cast<int32_t>(num_points()); }

 private:
  // TODO(zhafang): Use metadata to mark attribute ids in case they change.
  enum KeyframeAnimationAttributeId { TimestampId = 0, KeyframeDataId };

  bool SetTimeStamp(const std::vector<TimeStampType> &time_stamp);

  bool SetKeyframeData(DataType data_type, uint32_t num_components,
                       const char *data);
};

}  // namespace draco

#endif  // DRACO_ANIMATION_KEYFRAME_ANIMATION_H_
