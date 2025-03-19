#define GLAD_GL_IMPLEMENTATION

#include "globals.hpp"
#include "SDL3_image/SDL_image.h"
#include "defer.hpp"
#include "glad/gl.h"
#include "opengl.hpp"
#include "result.hpp"
#include "utility.hpp"
#include <climits>

namespace OpenGL
{
    using glm::vec2;
    using glm::vec3;

    namespace
    {
        uint32_t shaderProgram = 0;

        constexpr const char* vertexShaderSource = R"(
            #version 130
            #extension GL_ARB_explicit_attrib_location : require

            vec2 getUvFromLookupTable(int index)
            {
                const vec2 uvTable[4] = vec2[](
                    vec2(0, 1),
                    vec2(1, 1),
                    vec2(0, 0),
                    vec2(1, 0)
                );

                return uvTable[index];
            }

            layout (location = 0) in vec2 aPos;
            layout (location = 1) in float aUvIdF;

            out vec2 uv;

            void main()
            {
                vec2 nPos = vec2(
                      aPos.x / (1920 / 2.0f) - 1.0f,
                    -(aPos.y / (1080 / 2.0f) - 1.0f)
                );
                gl_Position = vec4(nPos.x, nPos.y, 0, 1);

                uv = getUvFromLookupTable(int(aUvIdF));
            }
        )";

        constexpr const char* fragmentShaderSource = R"(
            #version 130

            in vec2 uv;

            out vec4 FragColor;
            uniform sampler2D textureSampler;
            uniform bool useTexture;
            uniform vec3 drawColor;

            void main()
            {
                if (useTexture)
                {
                    FragColor = texture(textureSampler, uv);
                }
                else
                {
                    FragColor = vec4(drawColor, 1);
                }
            }
        )";

        struct PrimitiveDescriptor
        {
            GLuint Id;
            GLenum RenderMode;
            GLsizei VertexCount;
        };

        void renderPrimitives(const PrimitiveDescriptor& desc)
        {
            glBindBuffer(GL_ARRAY_BUFFER, desc.Id);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
            glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 0);
            glDrawArrays(desc.RenderMode, 0, desc.VertexCount);
        }

        void checkShaderCompilationResult(const char* identifier, uint32_t shader)
        {
            GLint compileStatus = 0;
            glGetShaderiv(shader, GL_COMPILE_STATUS, &compileStatus);

            if (compileStatus == GL_FALSE)
            {
                GLint maxLength = 0;
                glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);
                
                std::vector<GLchar> errorLog(maxLength);
                glGetShaderInfoLog(shader, maxLength, &maxLength, errorLog.data());

                std::string_view errorMessage(errorLog.begin(), errorLog.end());
                Debug::log("[{} shader compilation failed]", identifier);
                Debug::log("{}", errorMessage);
            }
        }
    }

    void compileShaders()
    {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        uint32_t vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
        
        uint32_t fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
        
        glCompileShader(vertexShader);
        checkShaderCompilationResult("Vertex", vertexShader);
        
        glCompileShader(fragmentShader);
        checkShaderCompilationResult("Fragment", fragmentShader);

        shaderProgram = glCreateProgram();
        glAttachShader(shaderProgram, vertexShader);
        glAttachShader(shaderProgram, fragmentShader);
        glLinkProgram(shaderProgram);
        glUseProgram(shaderProgram);
    }

    void changeDrawColorTo(const SDL_FColor nColor)
    {
        int32_t drawColorLocation = glGetUniformLocation(shaderProgram, "drawColor");
        glUniform3f(drawColorLocation, nColor.r, nColor.g, nColor.b);
    }

    TextureGpu createTextureFromSurface(SDL_Surface*& surface)
    {
        uint32_t textureId = 0;
        glGenTextures(1, &textureId);
        glBindTexture(GL_TEXTURE_2D, textureId);

        glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA8, (*surface).w, (*surface).h, 0,
            GL_ABGR_EXT, GL_UNSIGNED_INT_8_8_8_8, (*surface).pixels
        );
        glGenerateMipmap(GL_TEXTURE_2D);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        uint32_t bufferId = 0;
        glGenBuffers(1, &bufferId);

        return {
            .Id = textureId,
            .BufferId = bufferId,
            .Width = (*surface).w,
            .Height = (*surface).h
        };
    }

    void renderTexture(const TextureGpu texture)
    {
        glBindTexture(GL_TEXTURE_2D, texture.Id);
        glBindBuffer(GL_ARRAY_BUFFER, texture.BufferId);
        glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(vec3), nullptr);
        glVertexAttribPointer(1, 1, GL_FLOAT, GL_FALSE, sizeof(vec3), (GLvoid*) sizeof(vec2));
        glUniform1i(glGetUniformLocation(shaderProgram, "useTexture"), 1);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    }

    void renderQuad(const uint32_t id)
    {
        renderPrimitives({
            .Id = id,
            .RenderMode = GL_TRIANGLE_STRIP,
            .VertexCount = 4
        });
    }

    void renderTriangles(const uint32_t id, const int32_t count)
    {
        renderPrimitives({
            .Id = id,
            .RenderMode = GL_TRIANGLES,
            .VertexCount = count,
        });        
    }

    void renderTrianglesPacked(const uint32_t id, const int32_t count)
    {
        renderPrimitives({
            .Id = id,
            .RenderMode = GL_TRIANGLE_STRIP,
            .VertexCount = count,
        });
    }

    void renderTrianglesHub(const uint32_t id, const int32_t count)
    {
        renderPrimitives({
            .Id = id,
            .RenderMode = GL_TRIANGLE_FAN,
            .VertexCount = count,
        });
    }

    uint32_t createPrimitives(std::span<const vec2> vertices)
    {
        uint32_t bufferId = 0;
        glGenBuffers(1, &bufferId);
        glBindBuffer(GL_ARRAY_BUFFER, bufferId);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(vec2), vertices.data(), GL_STATIC_DRAW);

        return bufferId;
    }

    uint32_t generateAndApplyVertexArray()
    {
        uint32_t vertexArrayId = 0;
        glGenVertexArrays(1, &vertexArrayId);
        glBindVertexArray(vertexArrayId);
        
        return vertexArrayId;
    }

    void defineTextureRenderLocation(TextureGpu texture, const Option::Inst<SDL_FRect>& area)
    {
        glBindTexture(GL_TEXTURE_2D, texture.Id);
        glBindBuffer(GL_ARRAY_BUFFER, texture.BufferId);

        if (area.isSome())
        {
            const std::array targetCoords = Utility::cornersOfRectangle(area.value());

            const std::array verticesWithUvId = { 
                vec3(targetCoords[0], 0),
                vec3(targetCoords[1], 1),
                vec3(targetCoords[2], 2),
                vec3(targetCoords[3], 3)
            };
            glBufferData(GL_ARRAY_BUFFER, verticesWithUvId.size() * sizeof(vec3), verticesWithUvId.data(), GL_STATIC_DRAW);
        }
        else
        {
            const auto properties = Globals::Properties.value();
            const auto width = static_cast<float>(properties.ScreenWidth);
            const auto height = static_cast<float>(properties.ScreenHeight);

            const std::array fullscreenVerticesWithUvId = {
                vec3(0, 0,          0),
                vec3(width, 0,      1),
                vec3(0, height,     2),
                vec3(width, height, 3)
            };
            glBufferData(GL_ARRAY_BUFFER, fullscreenVerticesWithUvId.size() * sizeof(vec3), fullscreenVerticesWithUvId.data(), GL_STATIC_DRAW);
        }

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
    }

    void clearScreen()
    {
        using Color::NBLACK;
        glClearColor(NBLACK.r, NBLACK.g, NBLACK.b, NBLACK.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }
}
