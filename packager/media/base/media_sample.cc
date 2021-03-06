// Copyright 2014 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#include "packager/media/base/media_sample.h"

#include <inttypes.h>

#include "packager/base/logging.h"
#include "packager/base/strings/stringprintf.h"

namespace shaka {
namespace media {

MediaSample::MediaSample(const uint8_t* data,
                         size_t size,
                         const uint8_t* side_data,
                         size_t side_data_size,
                         bool is_key_frame)
    : dts_(0),
      pts_(0),
      duration_(0),
      is_key_frame_(is_key_frame),
      is_encrypted_(false) {
  if (!data) {
    CHECK_EQ(size, 0u);
  }

  data_.assign(data, data + size);
  if (side_data)
    side_data_.assign(side_data, side_data + side_data_size);
}

MediaSample::MediaSample() : dts_(0),
                             pts_(0),
                             duration_(0),
                             is_key_frame_(false),
                             is_encrypted_(false) {}

MediaSample::~MediaSample() {}

// static
std::shared_ptr<MediaSample> MediaSample::CopyFrom(const uint8_t* data,
                                                   size_t data_size,
                                                   bool is_key_frame) {
  // If you hit this CHECK you likely have a bug in a demuxer. Go fix it.
  CHECK(data);
  return std::make_shared<MediaSample>(data, data_size, nullptr, 0u,
                                       is_key_frame);
}

// static
std::shared_ptr<MediaSample> MediaSample::CopyFrom(const uint8_t* data,
                                                   size_t data_size,
                                                   const uint8_t* side_data,
                                                   size_t side_data_size,
                                                   bool is_key_frame) {
  // If you hit this CHECK you likely have a bug in a demuxer. Go fix it.
  CHECK(data);
  return std::make_shared<MediaSample>(data, data_size, side_data,
                                       side_data_size, is_key_frame);
}

// static
std::shared_ptr<MediaSample> MediaSample::FromMetadata(const uint8_t* metadata,
                                                       size_t metadata_size) {
  return std::make_shared<MediaSample>(nullptr, 0, metadata, metadata_size,
                                       false);
}

// static
std::shared_ptr<MediaSample> MediaSample::CreateEmptyMediaSample() {
  return std::make_shared<MediaSample>();
}

// static
std::shared_ptr<MediaSample> MediaSample::CreateEOSBuffer() {
  return std::make_shared<MediaSample>(nullptr, 0, nullptr, 0, false);
}

std::string MediaSample::ToString() const {
  if (end_of_stream())
    return "End of stream sample\n";
  return base::StringPrintf(
      "dts: %" PRId64 "\n pts: %" PRId64 "\n duration: %" PRId64 "\n "
      "is_key_frame: %s\n size: %zu\n side_data_size: %zu\n",
      dts_,
      pts_,
      duration_,
      is_key_frame_ ? "true" : "false",
      data_.size(),
      side_data_.size());
}

}  // namespace media
}  // namespace shaka
