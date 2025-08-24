#include "shader.hpp"
#include "glm/glm.hpp"
#include "glm/gtc/integer.hpp"
#include "globals.hpp"
#include "debug.hpp"
#include "color.hpp"
#include "utility.hpp"
#include "seq/seq.hpp"
#include "defer.hpp"

namespace Shader
{
	namespace
	{
		using namespace OptionExtensions;
		using glm::vec2;
		using glm::u32vec3;

		constexpr uint32_t MAX_LOD_LEVEL = 32;

		const auto& Properties() { return Globals::Properties.value(); };

		extern "C"
		{
			uint8_t _binary____shader_vertex_spirv_start[];
			uint8_t _binary____shader_vertex_spirv_end[];
			uint8_t _binary____shader_fragment_spirv_start[];
			uint8_t _binary____shader_fragment_spirv_end[];
		}

		Option<CopyOperation> copyOperation = None<CopyOperation>();

		struct SwapchainTargetInfo
		{
			SDL_GPUGraphicsPipelineTargetInfo Info = {};
			SDL_GPUColorTargetDescription Storage = {};

			SwapchainTargetInfo(SDL_GPUDevice* const gpu, SDL_Window* const window)
			{
				Storage =
				{ 
					.format = SDL_GetGPUSwapchainTextureFormat(gpu, window),
					.blend_state =
					{
						.src_color_blendfactor = SDL_GPU_BLENDFACTOR_SRC_ALPHA,
						.dst_color_blendfactor = SDL_GPU_BLENDFACTOR_ONE_MINUS_SRC_ALPHA,
						.color_blend_op = SDL_GPU_BLENDOP_ADD,
						.src_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ONE,
						.dst_alpha_blendfactor = SDL_GPU_BLENDFACTOR_ZERO,
						.alpha_blend_op = SDL_GPU_BLENDOP_ADD,
						.enable_blend = true,
					}
				};
				
				Info.color_target_descriptions = &Storage;
				Info.num_color_targets = 1;
			}
		};

		struct PipelineConfigInfo
		{
			SDL_GPUVertexAttribute vao[1];
			SDL_GPUVertexBufferDescription vbo[1];
			SDL_GPUVertexInputState VertexInput = {};

			PipelineConfigInfo()
			{
				vbo[0] =
				{
					.slot = 0,
					.pitch = sizeof(vec2),
					.input_rate = SDL_GPU_VERTEXINPUTRATE_VERTEX,
					.instance_step_rate = 0,
				};

				vao[0] =
				{
					.location = 0,
					.buffer_slot = 0,
					.format = SDL_GPU_VERTEXELEMENTFORMAT_FLOAT2,
					.offset = 0,
				};

				VertexInput =
				{
					.vertex_buffer_descriptions = vbo,
					.num_vertex_buffers = 1,
					.vertex_attributes = vao,
					.num_vertex_attributes = 1,
				};
			}
		};

		SDL_GPUShader* createShaderFrom(SDL_GPUDevice* const gpu, uint8_t* const source, size_t length, SDL_GPUShaderStage stage)
		{
			SDL_GPUShaderCreateInfo shaderInfo =
			{
				.code_size = length,
				.code = source,
				.entrypoint = "main",
				.format = SDL_GPU_SHADERFORMAT_SPIRV,
				.stage = stage,
				.num_samplers = (stage == SDL_GPU_SHADERSTAGE_VERTEX) ? 0u : 1u,
				.num_storage_textures = 0u,
				.num_storage_buffers = 0u,
				.num_uniform_buffers = 1u,
			};

			return SDL_CreateGPUShader(gpu, &shaderInfo);
		}

		SDL_GPUSampler* createSamplerFrom(SDL_GPUDevice* const gpu)
		{
			SDL_GPUSamplerCreateInfo samplerInfo =
			{
				.min_filter = SDL_GPU_FILTER_LINEAR,
				.mag_filter = SDL_GPU_FILTER_LINEAR,
				.mipmap_mode = SDL_GPU_SAMPLERMIPMAPMODE_LINEAR,
				.address_mode_u = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
				.address_mode_v = SDL_GPU_SAMPLERADDRESSMODE_CLAMP_TO_EDGE,
				.max_anisotropy = 8,
				.min_lod = 0,
				.max_lod = MAX_LOD_LEVEL,
				.enable_anisotropy = true,
			};

			return SDL_CreateGPUSampler(gpu, &samplerInfo);
		}

		template<typename T>
		void uploadVertexDataToGpu(SDL_GPUBuffer* const buffer, std::span<const T> toUpload)
		{
			Debug::assert(copyOperation.isSome(), "No copy operation is in progress");

			SDL_GPUTransferBufferCreateInfo uploadInfo =
			{
				.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
				.size = static_cast<uint32_t>(toUpload.size_bytes()),
			};

			SDL_GPUTransferBuffer* upload = SDL_CreateGPUTransferBuffer(Properties().Gpu, &uploadInfo);

			{
				void* data = SDL_MapGPUTransferBuffer(Properties().Gpu, upload, false);
				DEFER(SDL_UnmapGPUTransferBuffer, Properties().Gpu, upload);

				SDL_memcpy(data, toUpload.data(), uploadInfo.size);
			}

			SDL_GPUTransferBufferLocation source =
			{
				.transfer_buffer = upload,
				.offset = 0,
			};

			SDL_GPUBufferRegion destination =
			{
				.buffer = buffer,
				.offset = 0,
				.size = uploadInfo.size,
			};

			SDL_UploadToGPUBuffer(copyOperation.value().Copy, &source, &destination, false);
		}
		
		template<typename T>
		void uploadTextureToGpu(SDL_GPUTexture* const texture, std::span<const T> toUpload, uint32_t width, uint32_t height)
		{
			Debug::assert(copyOperation.isSome(), "No copy operation is in progress");

			SDL_GPUTransferBufferCreateInfo uploadInfo =
			{
				.usage = SDL_GPU_TRANSFERBUFFERUSAGE_UPLOAD,
				.size = static_cast<uint32_t>(toUpload.size_bytes()),
			};

			SDL_GPUTransferBuffer* upload = SDL_CreateGPUTransferBuffer(Properties().Gpu, &uploadInfo);

			{
				void* data = SDL_MapGPUTransferBuffer(Properties().Gpu, upload, false);
				DEFER(SDL_UnmapGPUTransferBuffer, Properties().Gpu, upload);

				SDL_memcpy(data, toUpload.data(), uploadInfo.size);
			}

			SDL_GPUTextureTransferInfo source =
			{
				.transfer_buffer = upload,
				.offset = 0,
			};

			SDL_GPUTextureRegion destination =
			{
				.texture = texture,
				.w = width,
				.h = height,
				.d = 1,
			};

			SDL_UploadToGPUTexture(copyOperation.value().Copy, &source, &destination, false);
		}

		uint8_t* const vertexSource = _binary____shader_vertex_spirv_start;
		size_t vertexSourceLength = _binary____shader_vertex_spirv_end - _binary____shader_vertex_spirv_start;

		uint8_t* const fragmentSource = _binary____shader_fragment_spirv_start;
		size_t fragmentSourceLength = _binary____shader_fragment_spirv_end - _binary____shader_fragment_spirv_start;

		SDL_GPUGraphicsPipeline* pipeline = nullptr;

		SDL_GPUSampler* sampler = nullptr;

		SDL_GPUTexture* renderedTexture = nullptr;

		SDL_GPUTexture* backBufferTexture = nullptr;

		SDL_GPUColorTargetInfo clearScreen =
		{
			.clear_color = Color::NBLACK,
			.load_op = SDL_GPU_LOADOP_CLEAR,
			.store_op = SDL_GPU_STOREOP_RESOLVE,
		};
	}

	void init()
	{
		const auto& properties = Globals::Properties.value();
		SwapchainTargetInfo swapchainTarget(properties.Gpu, properties.Window);
		PipelineConfigInfo pipelineConfig;

		SDL_GPUGraphicsPipelineCreateInfo pipelineInfo =
		{
			.vertex_shader = createShaderFrom(properties.Gpu, vertexSource, vertexSourceLength, SDL_GPU_SHADERSTAGE_VERTEX),
			.fragment_shader = createShaderFrom(properties.Gpu, fragmentSource, fragmentSourceLength, SDL_GPU_SHADERSTAGE_FRAGMENT),
			.vertex_input_state = pipelineConfig.VertexInput,
			.primitive_type = SDL_GPU_PRIMITIVETYPE_TRIANGLESTRIP,
			.multisample_state = { .sample_count = SDL_GPU_SAMPLECOUNT_8 },
			.target_info = swapchainTarget.Info,
		};

		pipeline = SDL_CreateGPUGraphicsPipeline(properties.Gpu, &pipelineInfo);
		Debug::assert(pipeline, "SDL_CreateGPUGraphicsPipeline() failed, SDL: {}", SDL_GetError());

		sampler = createSamplerFrom(properties.Gpu);

		SDL_GPUTextureCreateInfo textureInfo =
		{
			.type = SDL_GPU_TEXTURETYPE_2D,
			.format = swapchainTarget.Storage.format,
			.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET,
			.width = properties.ScreenWidth,
			.height = properties.ScreenHeight,
			.layer_count_or_depth = 1,
			.num_levels = 1,
			.sample_count = SDL_GPU_SAMPLECOUNT_8,
		};

		renderedTexture = SDL_CreateGPUTexture(properties.Gpu, &textureInfo);

		textureInfo.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER;
		textureInfo.sample_count = SDL_GPU_SAMPLECOUNT_1;

		backBufferTexture = SDL_CreateGPUTexture(properties.Gpu, &textureInfo);
	}

	SDL_GPURenderPass* beginRender(SDL_GPUCommandBuffer* const commandBuffer, SDL_GPUTexture* const swapchainTexture)
	{
		clearScreen.texture = renderedTexture;
		clearScreen.resolve_texture = backBufferTexture;
		
		SDL_GPURenderPass* const render = SDL_BeginGPURenderPass(commandBuffer, &clearScreen, 1, nullptr);
		SDL_BindGPUGraphicsPipeline(render, pipeline);
		return render;
	}

	void changeDrawColorTo(SDL_GPUCommandBuffer* const commandBuffer, SDL_FColor nColor)
	{
		struct FuckYou
		{
			SDL_FColor DrawColor;
			uint32_t UseTexture;
		};

		FuckYou fword = { .DrawColor = nColor, .UseTexture = false };

		SDL_PushGPUFragmentUniformData(commandBuffer, 0, &fword, sizeof(FuckYou));
	}

	TextureInfo createTextureFromSurface(SDL_Surface* const surface)
	{
		uint32_t width = static_cast<uint32_t>((*surface).w);
		uint32_t height = static_cast<uint32_t>((*surface).h);

		SDL_GPUTextureCreateInfo textureCreateInfo =
		{
			.type = SDL_GPU_TEXTURETYPE_2D,
			.format = SDL_GPU_TEXTUREFORMAT_R8G8B8A8_UNORM,
			.usage = SDL_GPU_TEXTUREUSAGE_COLOR_TARGET | SDL_GPU_TEXTUREUSAGE_SAMPLER,
			.width = width,
			.height = height,
			.layer_count_or_depth = 1,
			.num_levels = static_cast<uint32_t>(glm::floor(glm::log2(glm::max(width, height))) + 1),
		};

		SDL_GPUBufferCreateInfo positionsInfo =
		{
			.usage = SDL_GPU_BUFFERUSAGE_VERTEX,
			.size = 4 * sizeof(vec2),
		};

		SDL_GPUTexture* const texture = SDL_CreateGPUTexture(Properties().Gpu, &textureCreateInfo);
		SDL_GPUBuffer* const texturePositions = SDL_CreateGPUBuffer(Properties().Gpu, &positionsInfo);

		return {
			.Metadata = surface,
			.Texture = texture,
			.TexturePositions = texturePositions,
		};
	}

	void renderTexture(SDL_GPUCommandBuffer* const commandBuffer, SDL_GPURenderPass* const render, const TextureInfo& textureInfo)
	{
		SDL_GPUTextureSamplerBinding samplerInfo =
		{
			.texture = textureInfo.Texture,
			.sampler = sampler,
		};

		SDL_GPUBufferBinding bufferInfo =
		{
			.buffer = textureInfo.TexturePositions,
			.offset = 0,
		};

		struct ScreenSize
		{
			uint32_t Width;
			uint32_t Height;
		};

		struct FuckYou
		{
			SDL_FColor DrawColor;
			uint32_t UseTexture;
		};

		ScreenSize screenSize = { .Width = 1920, .Height = 1080 };
		FuckYou fword = { .DrawColor = Color::NORANGE, .UseTexture = true };

		SDL_PushGPUVertexUniformData(commandBuffer, 0, &screenSize, sizeof(ScreenSize));
		SDL_PushGPUFragmentUniformData(commandBuffer, 0, &fword, sizeof(FuckYou));
		
		SDL_BindGPUVertexBuffers(render, 0, &bufferInfo, 1);
		SDL_BindGPUFragmentSamplers(render, 0, &samplerInfo, 1);
		SDL_DrawGPUPrimitives(render, 4, 1, 0, 0);
	}

	void uploadTextureToGpuWithAreaIfNeeded(const TextureInfo& textureInfo, const Option<SDL_FRect>& area)
	{
		const auto& metadata = *textureInfo.Metadata;
		uint32_t width = static_cast<uint32_t>(metadata.w);
		uint32_t height = static_cast<uint32_t>(metadata.h);
		size_t sizeInBytes = width * height * 4;
		std::span data(static_cast<const uint8_t*>(metadata.pixels), sizeInBytes);

		uploadTextureToGpu(textureInfo.Texture, data, width, height);
		uploadVertexDataToGpu(textureInfo.TexturePositions,
			area.isSome()
			? Utility::cornersOfRectangle(area.value()).asSpan()
			: Utility::cornersOfRectangle(SDL_FRect(0, 0, (float) Properties().ScreenWidth, (float) Properties().ScreenHeight)).asSpan()
		);
	}

	VertexInfo createAndUploadPrimitives(std::span<const vec2> vertices, bool emulatingTriangleFan)
	{
		Shader::beginUploadToGpu();
		const uint32_t vertexCount = static_cast<uint32_t>(vertices.size());

		SDL_GPUBufferCreateInfo bufferInfo =
		{
			.usage = SDL_GPU_BUFFERUSAGE_VERTEX,
			.size = vertexCount * sizeof(vec2),
		};
		
		SDL_GPUBuffer* const buffer = SDL_CreateGPUBuffer(Properties().Gpu, &bufferInfo);
		uploadVertexDataToGpu<vec2>(buffer, vertices);

		uint32_t indexCount = 0;
		SDL_GPUBuffer* indexBuffer = nullptr;

		if (emulatingTriangleFan)
		{
			const uint32_t indexGroups = vertexCount - 1;
			const auto indexData = Seq::init(indexGroups, [vertexCount](size_t i) { return u32vec3(0, i + 1, i + 2 != vertexCount ? i + 2 : 1); });

			SDL_GPUBufferCreateInfo indexBufferInfo =
			{
				.usage = SDL_GPU_BUFFERUSAGE_INDEX,
				.size = indexGroups * sizeof(u32vec3),
			};

			indexCount = indexGroups * 3;
			indexBuffer = SDL_CreateGPUBuffer(Properties().Gpu, &indexBufferInfo);
			uploadVertexDataToGpu<u32vec3>(indexBuffer, indexData);
		}

		Shader::endUploadToGpu();

		return {
			.VertexBuffer = { .Content = buffer, .Count = vertexCount },
			.IndexBuffer = { .Content = indexBuffer, .Count = indexCount },
		};
	}

	//VertexInfo createAndUploadPrimitives(std::initializer_list<vec2> vertices)
	//{
	//	return createAndUploadPrimitives(std::span(vertices));
	//}

	void renderTriangles(SDL_GPURenderPass* const render, const VertexInfo& buffers)
	{
		SDL_GPUBufferBinding bufferInfo =
		{
			.buffer = buffers.VertexBuffer.Content,
			.offset = 0,
		};

		SDL_BindGPUVertexBuffers(render, 0, &bufferInfo, 1);

		if (buffers.IndexBuffer.Content != nullptr)
		{
			SDL_GPUBufferBinding indexBufferInfo =
			{
				.buffer = buffers.IndexBuffer.Content,
				.offset = 0,
			};

			SDL_BindGPUIndexBuffer(render, &indexBufferInfo, SDL_GPU_INDEXELEMENTSIZE_32BIT);
			SDL_DrawGPUIndexedPrimitives(render, buffers.IndexBuffer.Count, 1, 0, 0, 0);
		}
		else
		{
			SDL_DrawGPUPrimitives(render, buffers.VertexBuffer.Count, 1, 0, 0);
		}
	}

	void finalizeRender(SDL_GPUCommandBuffer* const commandBuffer, SDL_GPUTexture* const swapchain)
	{
		SDL_GPUBlitRegion source =
		{
			.texture = backBufferTexture,
			.mip_level = 0,
			.layer_or_depth_plane = 0,
			.x = 0,
			.y = 0,
			.w = Properties().ScreenWidth,
			.h = Properties().ScreenHeight,
		};

		SDL_GPUBlitRegion destination =
		{
			.texture = swapchain,
			.mip_level = 0,
			.layer_or_depth_plane = 0,
			.x = 0,
			.y = 0,
			.w = Properties().ScreenWidth,
			.h = Properties().ScreenHeight,
		};

		SDL_GPUBlitInfo blitInfo =
		{
			.source = source,
			.destination = destination,
			.load_op = SDL_GPU_LOADOP_DONT_CARE,
			.flip_mode = SDL_FLIP_NONE,
			.filter = SDL_GPU_FILTER_LINEAR,
		};

		SDL_BlitGPUTexture(commandBuffer, &blitInfo);
	}

	auto beginUploadToGpu() -> void
	{
		Debug::assert(copyOperation.isNone(), "Previous copy operation has not yet been finalized");
		SDL_GPUCommandBuffer* const commandBuffer = SDL_AcquireGPUCommandBuffer(Properties().Gpu);

		copyOperation = Some<CopyOperation>({
			.CommandBuffer = commandBuffer,
			.Copy = SDL_BeginGPUCopyPass(commandBuffer),
		});
	}

	auto endUploadToGpu() -> void
	{
		Debug::assert(copyOperation.isSome(), "No copy operation is in progress");
		const auto& op = copyOperation.value();

		SDL_EndGPUCopyPass(op.Copy);
		SDL_SubmitGPUCommandBuffer(op.CommandBuffer);
		copyOperation = None<CopyOperation>();
	}
}