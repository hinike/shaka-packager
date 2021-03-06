// Copyright 2015 Google Inc. All rights reserved.
//
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file or at
// https://developers.google.com/open-source/licenses/bsd

#ifndef MEDIA_FORMATS_WEBVTT_WEBVTT_MEDIA_PARSER_H_
#define MEDIA_FORMATS_WEBVTT_WEBVTT_MEDIA_PARSER_H_

#include <stdint.h>
#include <string>
#include <vector>

#include "packager/base/compiler_specific.h"
#include "packager/media/base/media_parser.h"

namespace shaka {
namespace media {

// If comment is not empty, then this is metadata and other fields must
// be empty.
// Data that can be multiline are vector of strings.
struct Cue {
  Cue();
  ~Cue();

  std::string identifier;
  uint64_t start_time;
  uint64_t duration;
  std::string settings;
  std::vector<std::string> payload;
  std::vector<std::string> comment;
};

/// Convert Cue to MediaSample.
/// @param cue data.
/// @return @a cue converted to a MediaSample.
std::shared_ptr<MediaSample> CueToMediaSample(const Cue& cue);

/// Convert MediaSample to Cue.
/// @param sample to be converted.
/// @return @a sample converted to Cue.
Cue MediaSampleToCue(const MediaSample& sample);

// WebVTT parser.
// The input may not be encrypted so decryption_key_source is ignored.
class WebVttMediaParser : public MediaParser {
 public:
  WebVttMediaParser();
  ~WebVttMediaParser() override;

  /// @name MediaParser implementation overrides.
  /// @{
  void Init(const InitCB& init_cb,
            const NewSampleCB& new_sample_cb,
            KeySource* decryption_key_source) override;
  bool Flush() override WARN_UNUSED_RESULT;
  bool Parse(const uint8_t* buf, int size) override WARN_UNUSED_RESULT;
  /// @}

 private:
  enum WebVttReadingState {
    kHeader,
    kMetadata,
    kCueIdentifierOrTimingOrComment,
    kCueTiming,
    kCuePayload,
    kComment,
    kParseError,
  };

  InitCB init_cb_;
  NewSampleCB new_sample_cb_;

  // All the unprocessed data passed to this parser.
  std::string data_;

  // The WEBVTT text + metadata header (global settings) for this webvtt.
  // One element per line.
  std::vector<std::string> header_;

  // This is set to what the parser is expecting. For example, if the parse is
  // expecting a kCueTiming, then the next line that it parses should be a
  // WebVTT timing line or an empty line.
  WebVttReadingState state_;

  Cue current_cue_;

  DISALLOW_COPY_AND_ASSIGN(WebVttMediaParser);
};

}  // namespace media
}  // namespace shaka

#endif  // MEDIA_FORMATS_WEBVTT_WEBVTT_MEDIA_PARSER_H_
