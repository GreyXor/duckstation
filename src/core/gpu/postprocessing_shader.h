// SPDX-FileCopyrightText: 2019-2023 Connor McLaughlin <stenzek@gmail.com>
// SPDX-License-Identifier: (GPL-3.0 OR CC-BY-NC-ND-4.0)

#pragma once

#include "common/rectangle.h"
#include "core/types.h"
#include "gpu_device.h"

#include <array>
#include <string>
#include <string_view>
#include <vector>

class GPUPipeline;
class GPUTexture;

class PostProcessingChain;
class PostProcessingShaderGen;

class PostProcessingShader
{
  friend PostProcessingChain;
  friend PostProcessingShaderGen;

public:
  struct Option
  {
    enum : u32
    {
      MAX_VECTOR_COMPONENTS = 4
    };

    enum class Type
    {
      Invalid,
      Bool,
      Int,
      Float
    };

    union Value
    {
      s32 int_value;
      float float_value;
    };
    static_assert(sizeof(Value) == sizeof(u32));

    using ValueVector = std::array<Value, MAX_VECTOR_COMPONENTS>;
    static_assert(sizeof(ValueVector) == sizeof(u32) * MAX_VECTOR_COMPONENTS);

    std::string name;
    std::string ui_name;
    std::string dependent_option;
    Type type;
    u32 vector_size;
    ValueVector default_value;
    ValueVector min_value;
    ValueVector max_value;
    ValueVector step_value;
    ValueVector value;
  };

  PostProcessingShader();
  PostProcessingShader(std::string name, std::string code);
  PostProcessingShader(const PostProcessingShader& copy);
  PostProcessingShader(PostProcessingShader& move);
  ~PostProcessingShader();

  PostProcessingShader& operator=(const PostProcessingShader& copy);
  PostProcessingShader& operator=(PostProcessingShader& move);

  ALWAYS_INLINE const std::string& GetName() const { return m_name; }
  ALWAYS_INLINE const std::string& GetCode() const { return m_code; }
  ALWAYS_INLINE const std::vector<Option>& GetOptions() const { return m_options; }
  ALWAYS_INLINE std::vector<Option>& GetOptions() { return m_options; }
  ALWAYS_INLINE bool HasOptions() const { return !m_options.empty(); }

  bool IsValid() const;

  const Option* GetOptionByName(const std::string_view& name) const;
  Option* GetOptionByName(const std::string_view& name);

  std::string GetConfigString() const;
  void SetConfigString(const std::string_view& str);

  bool LoadFromFile(std::string name, const char* filename);
  bool LoadFromString(std::string name, std::string code);

  bool ResizeOutput(GPUTexture::Format format, u32 width, u32 height);

private:
  struct CommonUniforms
  {
    float src_rect[4];
    float src_size[2];
    float resolution[2];
    float rcp_resolution[2];
    float window_resolution[2];
    float rcp_window_resolution[2];
    float original_size[2];
    float padded_original_size[2];
    float time;
    float padding;
  };

  void LoadOptions();

  ALWAYS_INLINE GPUPipeline* GetPipeline() const { return m_pipeline.get(); }
  ALWAYS_INLINE GPUTexture* GetOutputTexture() const { return m_output_texture.get(); }
  ALWAYS_INLINE GPUFramebuffer* GetOutputFramebuffer() const { return m_output_framebuffer.get(); }

  u32 GetUniformsSize() const;
  void FillUniformBuffer(void* buffer, u32 texture_width, s32 texture_height, s32 texture_view_x, s32 texture_view_y,
                         s32 texture_view_width, s32 texture_view_height, u32 window_width, u32 window_height,
                         s32 original_width, s32 original_height, float time) const;
  bool CompilePipeline(GPUTexture::Format target_format);

  std::string m_name;
  std::string m_code;
  std::vector<Option> m_options;

  std::unique_ptr<GPUPipeline> m_pipeline;
  std::unique_ptr<GPUTexture> m_output_texture;
  std::unique_ptr<GPUFramebuffer> m_output_framebuffer;
};
